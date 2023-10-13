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

    void CreateImportanceStore();

    void DefineMaterials();

private:
    bool CheckOverlaps;

    std::unique_ptr<G4NistManager> NISTManager;

    G4double WorldSize;
    G4double TotalHeavyWaterThickness;
    G4double TotalShieldingThickness;
    G4double TotalDetectorThickness;
    G4double HeavyWaterCellThickness;
    G4double ShieldingCellThickness;
    G4double DetectorCellThickness;
    G4int    NumberOfHeavyWaterLayers;
    G4int    NumberOfShieldingLayers;
    G4int    NumberOfDetectorLayers;


    std::unique_ptr<G4Material> WorldMaterial;
    std::unique_ptr<G4Material> HeavyWaterResidualMaterial;
    std::unique_ptr<G4Material> ShieldingMaterial;
    std::unique_ptr<G4Material> DetectorMaterial;

    std::unique_ptr<G4Sphere> WorldSphere;
    std::unique_ptr<G4LogicalVolume> WorldLogicalVolume;
    std::unique_ptr<G4PVPlacement> WorldPhysicalVolume;

    std::vector<std::unique_ptr<G4Sphere>> HeavyWaterResidualSphere;
    std::vector<std::unique_ptr<G4LogicalVolume>> HeavyWaterResidualLogicalVolume;
    std::vector<std::unique_ptr<G4PVPlacement>> HeavyWaterResidualPhysicalVolume;

    std::vector<std::unique_ptr<G4Sphere>> ShieldingSphere;
    std::vector<std::unique_ptr<G4LogicalVolume>> ShieldingLogicalVolume;
    std::vector<std::unique_ptr<G4PVPlacement>> ShieldingPhysicalVolume;

    std::vector<std::unique_ptr<G4Sphere>> DetectorSphere;
    std::vector<std::unique_ptr<G4LogicalVolume>> DetectorLogicalVolume;
    std::vector<std::unique_ptr<G4PVPlacement>> DetectorPhysicalVolume;
};