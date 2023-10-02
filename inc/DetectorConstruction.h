#pragma once

#include <utility>
#include <tuple>
#include <fstream>


#include <G4VUserDetectorConstruction.hh>
#include <G4Sphere.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4SDManager.hh>
#include <G4NistManager.hh>
#include <G4VisAttributes.hh>

#include <TMath.h>

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

private:
    bool CheckOverlaps;

    G4double WorldSize;
    std::unique_ptr<G4Material> WorldMaterial;
    std::unique_ptr<G4Box> WorldBox;
    std::unique_ptr<G4LogicalVolume> WorldLogicalVolume;
    std::unique_ptr<G4PVPlacement> WorldPhysicalVolume;

    G4double ShieldingThickness;
    G4double DistanceFromInteractionPointToShielding;
    std::unique_ptr<G4Material> ShieldingMaterial;
    std::unique_ptr<G4Sphere> ShieldingSphere;
    std::unique_ptr<G4LogicalVolume> ShieldingLogicalVolume;
    std::unique_ptr<G4PVPlacement> ShieldingPhysicalVolume;

    G4double ShieldingToDetectorBufferDistance;
    G4double DistanceFromInteractionPointToDetector;
    G4double DetectorThickness;
    std::unique_ptr<G4Sphere> DetectorSphere;
    std::unique_ptr<G4LogicalVolume> DetectorLogicalVolume;
    std::unique_ptr<G4PVPlacement> DetectorPhysicalVolume;

    std::unique_ptr<G4NistManager> NISTManager;
};