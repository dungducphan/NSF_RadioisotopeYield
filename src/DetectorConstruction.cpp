#include <DetectorConstruction.h>

#include <memory>

DetectorConstruction::DetectorConstruction() :  G4VUserDetectorConstruction() {
    CheckOverlaps = true;

    NISTManager.reset(G4NistManager::Instance());

    WorldSize = 10. * cm;
    ShieldingThickness = 1. * cm;
    DistanceFromInteractionPointToShielding = 0.5 * cm;
    ShieldingToDetectorBufferDistance = 1. * mm;
    DistanceFromInteractionPointToDetector = DistanceFromInteractionPointToShielding + ShieldingThickness + ShieldingToDetectorBufferDistance;
    DetectorThickness = 100 * micrometer;
}

DetectorConstruction::~DetectorConstruction() = default;

G4VPhysicalVolume *DetectorConstruction::Construct() {
    // World volume
    WorldMaterial.reset(NISTManager->FindOrBuildMaterial("G4_Galactic"));
    WorldBox = std::make_unique<G4Box>("WorldBox", 0.5 * WorldSize, 0.5 * WorldSize, 0.5 * WorldSize);
    WorldLogicalVolume = std::make_unique<G4LogicalVolume>(WorldBox.get(), WorldMaterial.get(), "WorldLogicalVolume");
    WorldPhysicalVolume = std::make_unique<G4PVPlacement>(nullptr, G4ThreeVector(), WorldLogicalVolume.get(), "WorldPhysicalVolume", nullptr, false, 0, CheckOverlaps);
    auto VAWorld = new G4VisAttributes();
    VAWorld->SetVisibility(false);
    WorldLogicalVolume->SetVisAttributes(VAWorld);

    // Shielding volume
    ShieldingMaterial.reset(NISTManager->FindOrBuildMaterial("G4_Pb"));
    ShieldingSphere = std::make_unique<G4Sphere>("ShieldingSphere", DistanceFromInteractionPointToShielding, DistanceFromInteractionPointToShielding + ShieldingThickness, 0., 360. * deg, 0., 180. * deg);
    ShieldingLogicalVolume = std::make_unique<G4LogicalVolume>(ShieldingSphere.get(), ShieldingMaterial.get(), "ShieldingLogicalVolume");
    ShieldingPhysicalVolume = std::make_unique<G4PVPlacement>(nullptr, G4ThreeVector(), ShieldingLogicalVolume.get(), "ShieldingPhysicalVolume", WorldLogicalVolume.get(), false, 0, CheckOverlaps);
    auto VAShielding = new G4VisAttributes();
    VAShielding->SetColour(G4Colour::Grey());
    ShieldingLogicalVolume->SetVisAttributes(VAShielding);

    // Detector volume
    DetectorSphere = std::make_unique<G4Sphere>("DetectorSphere", DistanceFromInteractionPointToDetector, DistanceFromInteractionPointToDetector + DetectorThickness, 0., 360. * deg, 0., 180. * deg);
    DetectorLogicalVolume = std::make_unique<G4LogicalVolume>(DetectorSphere.get(), WorldMaterial.get(), "DetectorLogicalVolume");
    DetectorPhysicalVolume = std::make_unique<G4PVPlacement>(nullptr, G4ThreeVector(), DetectorLogicalVolume.get(), "DetectorPhysicalVolume", WorldLogicalVolume.get(), false, 0, CheckOverlaps);
    auto VADetector = new G4VisAttributes();
    VADetector->SetColour(G4Colour::Red());
    DetectorLogicalVolume->SetVisAttributes(VADetector);

    return WorldPhysicalVolume.get();
}

void DetectorConstruction::ConstructSDandField() {
    G4VUserDetectorConstruction::ConstructSDandField();
}
