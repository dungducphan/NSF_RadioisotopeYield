#include <DetectorConstruction.h>
#include <ParticleSD.h>

#include <G4IStore.hh>

DetectorConstruction::DetectorConstruction() :  G4VUserDetectorConstruction() {
    CheckOverlaps = true;

    TotalHeavyWaterThickness = 1 * mm;
    NumberOfHeavyWaterLayers = 10;
    HeavyWaterCellThickness = TotalHeavyWaterThickness / NumberOfHeavyWaterLayers;
    TotalShieldingThickness = 10 * mm;
    NumberOfShieldingLayers = 10;
    ShieldingCellThickness = TotalShieldingThickness / NumberOfShieldingLayers;
    TotalDetectorThickness = 0.1 * mm;
    NumberOfDetectorLayers = 1;
    DetectorCellThickness = TotalDetectorThickness / NumberOfDetectorLayers;
    WorldSize = TotalHeavyWaterThickness + TotalShieldingThickness + TotalDetectorThickness + 1 * cm;
}

DetectorConstruction::~DetectorConstruction() = default;

G4VPhysicalVolume *DetectorConstruction::Construct() {
    DefineMaterials();

    // World volume
    WorldSphere = std::make_unique<G4Sphere>("WorldSphere", 0, WorldSize, 0, 360. * deg, 0, 180. * deg);
    WorldLogicalVolume = std::make_unique<G4LogicalVolume>(WorldSphere.get(), WorldMaterial.get(), "WorldLogicalVolume");
    WorldPhysicalVolume = std::make_unique<G4PVPlacement>(nullptr, G4ThreeVector(), WorldLogicalVolume.get(), "WorldPhysicalVolume", nullptr, false, 0, CheckOverlaps);
    auto VAWorld = new G4VisAttributes();
    VAWorld->SetVisibility(false);
    WorldLogicalVolume->SetVisAttributes(VAWorld);

    // Heavy Water volumes
    auto VAHeavyWaterResidual = new G4VisAttributes();
    VAHeavyWaterResidual->SetForceSolid();
    VAHeavyWaterResidual->SetColour(0., 0., 1., 0.8);
    for (unsigned int i = 0; i < NumberOfHeavyWaterLayers; i++) {
        HeavyWaterResidualSphere.emplace_back(std::make_unique<G4Sphere>(Form("HeavyWaterCell_%02i", i), HeavyWaterCellThickness * (double) i, HeavyWaterCellThickness * (double) (i + 1), 0., 360. * deg, 0., 180. * deg));
        HeavyWaterResidualLogicalVolume.emplace_back(std::make_unique<G4LogicalVolume>(HeavyWaterResidualSphere[i].get(), HeavyWaterResidualMaterial.get(), Form("HeavyWaterLogicalCell_%02i", i)));
        HeavyWaterResidualLogicalVolume[i]->SetVisAttributes(VAHeavyWaterResidual);
        HeavyWaterResidualPhysicalVolume.emplace_back(std::make_unique<G4PVPlacement>(nullptr, G4ThreeVector(), HeavyWaterResidualLogicalVolume[i].get(), Form("HeavyWaterPhysicalCell_%02i", i), WorldLogicalVolume.get(), false, 0, CheckOverlaps));
    }

    // Shielding volumes
    auto VAShielding = new G4VisAttributes();
    VAShielding->SetForceSolid();
    VAShielding->SetColour(1., 0., 0., 0.3);
    for (unsigned int i = 0; i < NumberOfShieldingLayers; i++) {
        ShieldingSphere.emplace_back(std::make_unique<G4Sphere>(Form("ShieldingCell_%02i", i), TotalHeavyWaterThickness + ShieldingCellThickness * (double) i, TotalHeavyWaterThickness + ShieldingCellThickness * (double) (i + 1), 0., 360. * deg, 0., 180. * deg));
        ShieldingLogicalVolume.emplace_back(std::make_unique<G4LogicalVolume>(ShieldingSphere[i].get(), ShieldingMaterial.get(), Form("ShieldingLogicalCell_%02i", i)));
        ShieldingLogicalVolume[i]->SetVisAttributes(VAShielding);
        ShieldingPhysicalVolume.emplace_back(std::make_unique<G4PVPlacement>(nullptr, G4ThreeVector(), ShieldingLogicalVolume[i].get(), Form("ShieldingPhysicalCell_%02i", i), WorldLogicalVolume.get(), false, 0, CheckOverlaps));
    }

    // Detector volumes
    auto VADetector = new G4VisAttributes();
    VADetector->SetForceSolid();
    VADetector->SetColour(0., 1., 0., 0.1);
    for (unsigned int i = 0; i < NumberOfDetectorLayers; i++) {
        DetectorSphere.emplace_back(std::make_unique<G4Sphere>(Form("DetectorCell_%02i", i), TotalShieldingThickness + TotalHeavyWaterThickness + DetectorCellThickness * (double) i, TotalShieldingThickness + TotalHeavyWaterThickness + DetectorCellThickness * (double) (i + 1), 0., 360. * deg, 0., 180. * deg));
        DetectorLogicalVolume.emplace_back(std::make_unique<G4LogicalVolume>(DetectorSphere[i].get(), DetectorMaterial.get(), Form("DetectorLogicalCell_%02i", i)));
        DetectorLogicalVolume[i]->SetVisAttributes(VADetector);
        DetectorPhysicalVolume.emplace_back(std::make_unique<G4PVPlacement>(nullptr, G4ThreeVector(), DetectorLogicalVolume[i].get(), Form("DetectorPhysicalCell_%02i", i), WorldLogicalVolume.get(), false, 0, CheckOverlaps));
    }

    return WorldPhysicalVolume.get();
}

void DetectorConstruction::ConstructSDandField() {
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    ParticleSD* aSensitiveDetector = new ParticleSD("GammaSD");
    SDman->AddNewDetector(aSensitiveDetector);

    // Detect the radioisotopes in the detector
    for (unsigned int i = 0; i < NumberOfDetectorLayers; i++)
        SetSensitiveDetector(DetectorLogicalVolume[i].get(), aSensitiveDetector);
}

void DetectorConstruction::DefineMaterials() {
    NISTManager.reset(G4NistManager::Instance());

    WorldMaterial.reset(NISTManager->FindOrBuildMaterial("G4_Galactic"));
    ShieldingMaterial.reset(NISTManager->FindOrBuildMaterial("G4_POLYETHYLENE"));
    DetectorMaterial.reset(NISTManager->FindOrBuildMaterial("G4_Rh"));

    auto Deuteron  = new G4Isotope("Deuteron", 1, 2, 2.0141018* CLHEP::g / CLHEP::mole);
    auto Deuterium = new G4Element("Deuterium","elD", 1);
    Deuterium->AddIsotope(Deuteron, 1);
    auto Oxygen  = NISTManager->FindOrBuildElement(8);
    HeavyWaterResidualMaterial = std::make_unique<G4Material>("HeavyWater", 1.11*g/cm3, 2);
    HeavyWaterResidualMaterial->AddElement(Deuterium, 2);
    HeavyWaterResidualMaterial->AddElement(Oxygen, 1);
}

void DetectorConstruction::CreateImportanceStore() {
    G4IStore *istore = G4IStore::GetInstance();

    istore->AddImportanceGeometryCell(1, *WorldPhysicalVolume.get(), 0);

    for (unsigned int i = 0; i < NumberOfHeavyWaterLayers; i++)
        istore->AddImportanceGeometryCell(TMath::Power(2, (double) i + 1), *HeavyWaterResidualPhysicalVolume[i].get(), i + 1);

    for (unsigned int i = 0; i < NumberOfShieldingLayers; i++)
        istore->AddImportanceGeometryCell(TMath::Power(2, (double) NumberOfHeavyWaterLayers + (double) i + 1), *ShieldingPhysicalVolume[i].get(), NumberOfHeavyWaterLayers + i + 1);
}
