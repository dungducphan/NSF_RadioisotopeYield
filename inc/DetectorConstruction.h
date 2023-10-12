#pragma once

#include <utility>
#include <tuple>
#include <fstream>
#include <memory>

#include <G4VUserDetectorConstruction.hh>
#include <G4Sphere.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4SDManager.hh>
#include <G4NistManager.hh>
#include <G4Isotope.hh>
#include <G4VisAttributes.hh>

#include <TMath.h>

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

    [[nodiscard]] G4LogicalVolume* GetScoringVolume() const {
        return ScoringVolume.get();
    }

    void DefineMaterials();

private:
    bool CheckOverlaps;

    std::unique_ptr<G4NistManager> NISTManager;

    G4double WorldSize;
    std::unique_ptr<G4Material> WorldMaterial;
    std::unique_ptr<G4Sphere> WorldSphere;
    std::unique_ptr<G4LogicalVolume> WorldLogicalVolume;
    std::unique_ptr<G4PVPlacement> WorldPhysicalVolume;

    G4double HeavyWaterResidualThickness;
    G4double DistanceFromInteractionPointToHeavyWaterResidual;
    std::unique_ptr<G4Material> HeavyWaterResidualMaterial;
    std::unique_ptr<G4Sphere> HeavyWaterResidualSphere;
    std::unique_ptr<G4LogicalVolume> HeavyWaterResidualLogicalVolume;
    std::unique_ptr<G4PVPlacement> HeavyWaterResidualPhysicalVolume;

    G4double ShieldingThickness;
    G4double DistanceFromInteractionPointToShielding;
    std::unique_ptr<G4Material> ShieldingMaterial;
    std::unique_ptr<G4Sphere> ShieldingSphere;
    std::unique_ptr<G4LogicalVolume> ShieldingLogicalVolume;
    std::unique_ptr<G4PVPlacement> ShieldingPhysicalVolume;

    G4double ShieldingToDetectorBufferDistance;
    G4double DistanceFromInteractionPointToDetector;
    G4double DetectorThickness;
    std::unique_ptr<G4Material> DetectorMaterial;
    std::unique_ptr<G4Sphere> DetectorSphere;
    std::unique_ptr<G4LogicalVolume> DetectorLogicalVolume;
    std::unique_ptr<G4PVPlacement> DetectorPhysicalVolume;

protected:
    std::unique_ptr<G4LogicalVolume> ScoringVolume;
};