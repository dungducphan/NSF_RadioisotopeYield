#include <DetectorConstruction.h>

DetectorConstruction::DetectorConstruction() :  G4VUserDetectorConstruction() {
    CheckOverlaps = true;

    VoidThickness = 0 * mm;
    TotalHeavyWaterThickness = 1 * mm;
    NumberOfHeavyWaterLayers = 5;
    HeavyWaterCellThickness = TotalHeavyWaterThickness / NumberOfHeavyWaterLayers;
    TotalShieldingThickness = 4 * mm;
    NumberOfShieldingLayers = 4;
    ShieldingCellThickness = TotalShieldingThickness / NumberOfShieldingLayers;
    TotalDetectorThickness = 1 * mm;
    WorldSize = TotalHeavyWaterThickness + TotalShieldingThickness + TotalDetectorThickness + 1 * mm;
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

    // Detector volume
    auto VADetector = new G4VisAttributes();
    VADetector->SetForceSolid();
    VADetector->SetColour(0., 1., 0., 0.1);
    DetectorSphere = new G4Sphere("DetectorSphere", VoidThickness + TotalShieldingThickness + TotalHeavyWaterThickness, VoidThickness + TotalShieldingThickness + TotalHeavyWaterThickness + TotalDetectorThickness, 0., 360. * deg, 0., 180. * deg);
    DetectorLogicalVolume = new G4LogicalVolume(DetectorSphere, DetectorMaterial, "DetectorLogicalVolume");
    DetectorPhysicalVolume = new G4PVPlacement(nullptr, G4ThreeVector(), DetectorLogicalVolume, "DetectorPhysicalVolume", WorldLogicalVolume, false, 0, CheckOverlaps);
    DetectorLogicalVolume->SetVisAttributes(VADetector);

    return WorldPhysicalVolume;
}

void DetectorConstruction::ConstructSDandField() {
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

    istore->AddImportanceGeometryCell(TMath::Power(2, (double) (NumberOfHeavyWaterLayers + NumberOfShieldingLayers - 2)), *DetectorPhysicalVolume);
    istore->AddImportanceGeometryCell(TMath::Power(2, (double) (NumberOfHeavyWaterLayers + NumberOfShieldingLayers - 2)), *WorldPhysicalVolume);

    return istore;
}
