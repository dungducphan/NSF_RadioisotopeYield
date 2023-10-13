#include <DetectorConstruction.h>
#include <ParticleSD.h>

DetectorConstruction::DetectorConstruction() :  G4VUserDetectorConstruction() {
    CheckOverlaps = true;

    WorldSize = 2. * cm;

    HeavyWaterResidualThickness = 1 * mm;
    DistanceFromInteractionPointToHeavyWaterResidual = 0.15 * cm;
    ShieldingThickness = 0.5 * cm;
    DistanceFromInteractionPointToShielding = DistanceFromInteractionPointToHeavyWaterResidual + HeavyWaterResidualThickness;
    DetectorThickness = 100 * micrometer;
    DistanceFromInteractionPointToDetector = DistanceFromInteractionPointToShielding + ShieldingThickness;
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

    // Heavy Water volume
    HeavyWaterResidualSphere = std::make_unique<G4Sphere>("HeavyWaterResidualSphere", DistanceFromInteractionPointToHeavyWaterResidual, DistanceFromInteractionPointToHeavyWaterResidual + HeavyWaterResidualThickness, 0., 360. * deg, 0., 180. * deg);
    HeavyWaterResidualLogicalVolume = std::make_unique<G4LogicalVolume>(HeavyWaterResidualSphere.get(), HeavyWaterResidualMaterial.get(), "HeavyWaterResidualLogicalVolume");
    HeavyWaterResidualPhysicalVolume = std::make_unique<G4PVPlacement>(nullptr, G4ThreeVector(), HeavyWaterResidualLogicalVolume.get(), "HeavyWaterResidualPhysicalVolume", WorldLogicalVolume.get(), false, 0, CheckOverlaps);
    auto VAHeavyWaterResidual = new G4VisAttributes();
    VAHeavyWaterResidual->SetForceSolid();
    VAHeavyWaterResidual->SetColour(0., 0., 1., 0.8);
    HeavyWaterResidualLogicalVolume->SetVisAttributes(VAHeavyWaterResidual);

    // Shielding volume
    ShieldingSphere = std::make_unique<G4Sphere>("ShieldingSphere", DistanceFromInteractionPointToShielding, DistanceFromInteractionPointToShielding + ShieldingThickness, 0., 360. * deg, 0., 180. * deg);
    ShieldingLogicalVolume = std::make_unique<G4LogicalVolume>(ShieldingSphere.get(), ShieldingMaterial.get(), "ShieldingLogicalVolume");
    ShieldingPhysicalVolume = std::make_unique<G4PVPlacement>(nullptr, G4ThreeVector(), ShieldingLogicalVolume.get(), "ShieldingPhysicalVolume", WorldLogicalVolume.get(), false, 0, CheckOverlaps);
    auto VAShielding = new G4VisAttributes();
    VAShielding->SetVisibility(false);
    ShieldingLogicalVolume->SetVisAttributes(VAShielding);

    // Detector volume
    DetectorSphere = std::make_unique<G4Sphere>("DetectorSphere", DistanceFromInteractionPointToDetector, DistanceFromInteractionPointToDetector + DetectorThickness, 0., 360. * deg, 0., 180. * deg);
    DetectorLogicalVolume = std::make_unique<G4LogicalVolume>(DetectorSphere.get(), DetectorMaterial.get(), "DetectorLogicalVolume");
    DetectorPhysicalVolume = std::make_unique<G4PVPlacement>(nullptr, G4ThreeVector(), DetectorLogicalVolume.get(), "DetectorPhysicalVolume", WorldLogicalVolume.get(), false, 0, CheckOverlaps);
    auto VADetector = new G4VisAttributes();
    VADetector->SetForceSolid();
    VADetector->SetColour(1., 1., 1., 0.3);
    DetectorLogicalVolume->SetVisAttributes(VADetector);

    return WorldPhysicalVolume.get();
}

void DetectorConstruction::ConstructSDandField() {
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    ParticleSD* aSensitiveDetector = new ParticleSD("GammaSD");
    SDman->AddNewDetector(aSensitiveDetector);

    // Detect the radioisotopes in the detector
    SetSensitiveDetector(DetectorLogicalVolume.get(), aSensitiveDetector);
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
