#pragma once

#include <utility>
#include <tuple>
#include <fstream>
#include <memory>

#include <G4VUserDetectorConstruction.hh>
#include <G4Sphere.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4SDManager.hh>
#include <G4NistManager.hh>
#include <G4Isotope.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4PVReplica.hh>
#include <G4MultiFunctionalDetector.hh>
#include <G4VPrimitiveScorer.hh>
#include <G4THitsMap.hh>
#include <G4VIStore.hh>
#include <G4IStore.hh>

#include <TString.h>
#include <TMath.h>

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

    G4VIStore* CreateImportanceStore();
    G4VPhysicalVolume* GetWorldVolume();

    void DefineMaterials();

    bool CheckOverlaps;

    G4NistManager* NISTManager;

    G4double WorldSize;
    G4double TotalHeavyWaterThickness;
    G4double TotalShieldingThickness;
    G4double TotalDetectorThickness;
    G4double HeavyWaterCellThickness;
    G4double ShieldingCellThickness;
    G4double VoidThickness;
    G4int    NumberOfHeavyWaterLayers;
    G4int    NumberOfShieldingLayers;

    G4Material* WorldMaterial;
    G4Material* HeavyWaterResidualMaterial;
    G4Material* ShieldingMaterial;
    G4Material* DetectorMaterial;

    G4Sphere* WorldSphere;
    G4LogicalVolume* WorldLogicalVolume;
    G4VPhysicalVolume* WorldPhysicalVolume;

    std::vector<G4Sphere*> HeavyWaterResidualSphere;
    std::vector<G4LogicalVolume*> HeavyWaterResidualLogicalVolume;
    std::vector<G4VPhysicalVolume*> HeavyWaterResidualPhysicalVolume;

    std::vector<G4Sphere*> ShieldingSphere;
    std::vector<G4LogicalVolume*> ShieldingLogicalVolume;
    std::vector<G4VPhysicalVolume*> ShieldingPhysicalVolume;

    G4Sphere* DetectorSphere;
    G4LogicalVolume* DetectorLogicalVolume;
    G4VPhysicalVolume* DetectorPhysicalVolume;
};