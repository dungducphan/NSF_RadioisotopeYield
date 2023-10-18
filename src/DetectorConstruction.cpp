#include <DetectorConstruction.h>
#include <ParticleSD.h>

#include <G4IStore.hh>

DetectorConstruction::DetectorConstruction() :  G4VUserDetectorConstruction() {
    CheckOverlaps = true;

    VoidThickness = 0 * mm;
    TotalHeavyWaterThickness = 0.5 * mm;
    NumberOfHeavyWaterLayers = 5;
    HeavyWaterCellThickness = TotalHeavyWaterThickness / NumberOfHeavyWaterLayers;
    TotalShieldingThickness = 0.3 * mm;
    NumberOfShieldingLayers = 1;
    ShieldingCellThickness = TotalShieldingThickness / NumberOfShieldingLayers;
    TotalDetectorThickness = 0.1 * mm;
    NumberOfDetectorLayers = 1;
    DetectorCellThickness = TotalDetectorThickness / NumberOfDetectorLayers;
    WorldSize = TotalHeavyWaterThickness + TotalShieldingThickness + TotalDetectorThickness + 1 * cm;
}

DetectorConstruction::~DetectorConstruction() = default;

G4VPhysicalVolume *DetectorConstruction::GetWorldVolume() {
    return WorldPhysicalVolume;
}

G4VPhysicalVolume *DetectorConstruction::Construct() {
    DefineMaterials();

    // World volume
    WorldSphere = new G4Sphere("WorldSphere", 0, WorldSize, 0, 360. * deg, 0, 180. * deg);
    WorldLogicalVolume = new G4LogicalVolume(WorldSphere, WorldMaterial, "WorldLogicalVolume");
    WorldPhysicalVolume = new G4PVPlacement(nullptr, G4ThreeVector(), WorldLogicalVolume, "WorldPhysicalVolume", nullptr, false, 0, CheckOverlaps);
    auto VAWorld = new G4VisAttributes();
    VAWorld->SetVisibility(false);
    WorldLogicalVolume->SetVisAttributes(VAWorld);

    // Heavy Water volumes
    auto VAHeavyWaterResidual = new G4VisAttributes();
    VAHeavyWaterResidual->SetForceSolid();
    VAHeavyWaterResidual->SetColour(0., 0., 1., 0.8);
    for (unsigned int i = 0; i < NumberOfHeavyWaterLayers; i++) {
        HeavyWaterResidualSphere.emplace_back(new G4Sphere(Form("HeavyWaterCell_%02i", i), VoidThickness + HeavyWaterCellThickness * (double) i, VoidThickness + HeavyWaterCellThickness * (double) (i + 1), 0., 360. * deg, 0., 180. * deg));
        HeavyWaterResidualLogicalVolume.emplace_back(new G4LogicalVolume(HeavyWaterResidualSphere[i], HeavyWaterResidualMaterial, Form("HeavyWaterLogicalCell_%02i", i)));
        HeavyWaterResidualLogicalVolume[i]->SetVisAttributes(VAHeavyWaterResidual);
        HeavyWaterResidualPhysicalVolume.emplace_back(new G4PVPlacement(nullptr, G4ThreeVector(), HeavyWaterResidualLogicalVolume[i], Form("HeavyWaterPhysicalCell_%02i", i), WorldLogicalVolume, false, 0, CheckOverlaps));
    }

    // Shielding volumes
    auto VAShielding = new G4VisAttributes();
    VAShielding->SetForceSolid();
    VAShielding->SetColour(1., 0., 0., 0.3);
    for (unsigned int i = 0; i < NumberOfShieldingLayers; i++) {
        ShieldingSphere.emplace_back(new G4Sphere(Form("ShieldingCell_%02i", i), VoidThickness + TotalHeavyWaterThickness + ShieldingCellThickness * (double) i, VoidThickness + TotalHeavyWaterThickness + ShieldingCellThickness * (double) (i + 1), 0., 360. * deg, 0., 180. * deg));
        ShieldingLogicalVolume.emplace_back(new G4LogicalVolume(ShieldingSphere[i], ShieldingMaterial, Form("ShieldingLogicalCell_%02i", i)));
        ShieldingLogicalVolume[i]->SetVisAttributes(VAShielding);
        ShieldingPhysicalVolume.emplace_back(new G4PVPlacement(nullptr, G4ThreeVector(), ShieldingLogicalVolume[i], Form("ShieldingPhysicalCell_%02i", i), WorldLogicalVolume, false, 0, CheckOverlaps));
    }

    // Detector volumes
    auto VADetector = new G4VisAttributes();
    VADetector->SetForceSolid();
    VADetector->SetColour(0., 1., 0., 0.1);
    for (unsigned int i = 0; i < NumberOfDetectorLayers; i++) {
        DetectorSphere.emplace_back(new G4Sphere(Form("DetectorCell_%02i", i), VoidThickness + TotalShieldingThickness + TotalHeavyWaterThickness + DetectorCellThickness * (double) i, VoidThickness + TotalShieldingThickness + TotalHeavyWaterThickness + DetectorCellThickness * (double) (i + 1), 0., 360. * deg, 0., 180. * deg));
        DetectorLogicalVolume.emplace_back(new G4LogicalVolume(DetectorSphere[i], DetectorMaterial, Form("DetectorLogicalCell_%02i", i)));
        DetectorLogicalVolume[i]->SetVisAttributes(VADetector);
        DetectorPhysicalVolume.push_back(new G4PVPlacement(nullptr, G4ThreeVector(), DetectorLogicalVolume[i], Form("DetectorPhysicalCell_%02i", i), WorldLogicalVolume, false, 0, CheckOverlaps));
    }

    return WorldPhysicalVolume;
}

void DetectorConstruction::ConstructSDandField() {
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    auto aSensitiveDetector = new ParticleSD("IonSD");
    SDman->AddNewDetector(aSensitiveDetector);

    // Detect the radioisotopes in the detector
    for (unsigned int i = 0; i < NumberOfDetectorLayers; i++)
        SetSensitiveDetector(DetectorLogicalVolume[i], aSensitiveDetector);
}

void DetectorConstruction::DefineMaterials() {
    NISTManager = G4NistManager::Instance();

    WorldMaterial = NISTManager->FindOrBuildMaterial("G4_Galactic");
    ShieldingMaterial = NISTManager->FindOrBuildMaterial("G4_Pb");
    DetectorMaterial = NISTManager->FindOrBuildMaterial("G4_Rh");

    auto Deuteron  = new G4Isotope("Deuteron", 1, 2, 2.0141018* CLHEP::g / CLHEP::mole);
    auto Deuterium = new G4Element("Deuterium","elD", 1);
    Deuterium->AddIsotope(Deuteron, 1);
    auto Oxygen  = NISTManager->FindOrBuildElement(8);
    HeavyWaterResidualMaterial = new G4Material("HeavyWater", 1.11*g/cm3, 2);
    HeavyWaterResidualMaterial->AddElement(Deuterium, 2);
    HeavyWaterResidualMaterial->AddElement(Oxygen, 1);
}

G4VIStore* DetectorConstruction::CreateImportanceStore() {
    G4IStore *istore = G4IStore::GetInstance();

    for (unsigned int i = 0; i < NumberOfHeavyWaterLayers; i++)
        istore->AddImportanceGeometryCell(TMath::Power(2, (double) i), *HeavyWaterResidualPhysicalVolume[i]);

    for (unsigned int i = 0; i < NumberOfShieldingLayers; i++)
        istore->AddImportanceGeometryCell(TMath::Power(2, (double) NumberOfHeavyWaterLayers + (double) i - 1), *ShieldingPhysicalVolume[i]);

    for (unsigned int i = 0; i < NumberOfDetectorLayers; i++)
        istore->AddImportanceGeometryCell(TMath::Power(2, (double) (NumberOfHeavyWaterLayers + NumberOfShieldingLayers - 2)), *DetectorPhysicalVolume[i]);

    istore->AddImportanceGeometryCell(TMath::Power(2, (double) (NumberOfHeavyWaterLayers + NumberOfShieldingLayers - 2)), *WorldPhysicalVolume);

    return istore;
}
