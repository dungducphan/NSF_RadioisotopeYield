#include <DetectorConstruction.h>

#include <memory>

DetectorConstruction::DetectorConstruction() :  G4VUserDetectorConstruction() {
    CheckOverlaps = true;

    NISTManager.reset(G4NistManager::Instance());

    WorldSize = 100. * cm;
    ShieldingThickness = 1. * cm;
    DistanceFromInteractionPointToShielding = 0.25 * cm;
    ShieldingToDetectorBufferDistance = 1. * mm;
    DistanceFromInteractionPointToDetector = DistanceFromInteractionPointToShielding + ShieldingThickness + ShieldingToDetectorBufferDistance;
    DetectorThickness = WorldSize - 1 * mm - DistanceFromInteractionPointToDetector;
}

DetectorConstruction::~DetectorConstruction() = default;

G4VPhysicalVolume *DetectorConstruction::Construct() {
    // World volume
    WorldMaterial.reset(NISTManager->FindOrBuildMaterial("G4_Galactic"));
    WorldSphere = std::make_unique<G4Sphere>("WorldSphere", 0, WorldSize, 0, 360. * deg, 0, 180. * deg);
    WorldLogicalVolume = std::make_unique<G4LogicalVolume>(WorldSphere.get(), WorldMaterial.get(), "WorldLogicalVolume");
    WorldPhysicalVolume = std::make_unique<G4PVPlacement>(nullptr, G4ThreeVector(), WorldLogicalVolume.get(), "WorldPhysicalVolume", nullptr, false, 0, CheckOverlaps);
    auto VAWorld = new G4VisAttributes();
    VAWorld->SetForceSolid();
    VAWorld->SetColour(1., 1., 1., 0.2);
    WorldLogicalVolume->SetVisAttributes(VAWorld);

    // Shielding volume
    ShieldingMaterial.reset(NISTManager->FindOrBuildMaterial("G4_Pb"));
    ShieldingSphere = std::make_unique<G4Sphere>("ShieldingSphere", DistanceFromInteractionPointToShielding, DistanceFromInteractionPointToShielding + ShieldingThickness, 0., 360. * deg, 0., 180. * deg);
    ShieldingLogicalVolume = std::make_unique<G4LogicalVolume>(ShieldingSphere.get(), ShieldingMaterial.get(), "ShieldingLogicalVolume");
    ShieldingPhysicalVolume = std::make_unique<G4PVPlacement>(nullptr, G4ThreeVector(), ShieldingLogicalVolume.get(), "ShieldingPhysicalVolume", WorldLogicalVolume.get(), false, 0, CheckOverlaps);
    auto VAShielding = new G4VisAttributes();
    VAShielding->SetForceSolid();
    VAShielding->SetColour(0., 1., 0., 0.5);
    ShieldingLogicalVolume->SetVisAttributes(VAShielding);

    // Detector volume
    DetectorMaterial.reset(NISTManager->FindOrBuildMaterial("G4_Pb"));
    DetectorSphere = std::make_unique<G4Sphere>("DetectorSphere", DistanceFromInteractionPointToDetector, DistanceFromInteractionPointToDetector + DetectorThickness, 0., 360. * deg, 0., 180. * deg);
    DetectorLogicalVolume = std::make_unique<G4LogicalVolume>(DetectorSphere.get(), DetectorMaterial.get(), "DetectorLogicalVolume");
    DetectorPhysicalVolume = std::make_unique<G4PVPlacement>(nullptr, G4ThreeVector(), DetectorLogicalVolume.get(), "DetectorPhysicalVolume", WorldLogicalVolume.get(), false, 0, CheckOverlaps);
    auto VADetector = new G4VisAttributes();
    VADetector->SetForceSolid();
    VADetector->SetColour(1., 0., 0., 0.9);
    DetectorLogicalVolume->SetVisAttributes(VADetector);

    return WorldPhysicalVolume.get();
}

void DetectorConstruction::ConstructSDandField() {
    G4VUserDetectorConstruction::ConstructSDandField();
}
