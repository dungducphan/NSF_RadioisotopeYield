#pragma once

#include <G4VUserActionInitialization.hh>

#include <PrimaryGenerator.h>
#include <RunAction.h>
#include <SteppingAction.h>
#include <ActionInitialization.h>

class DetectorConstruction;

class ActionInitialization : public G4VUserActionInitialization {
public:
    ActionInitialization(DetectorConstruction* detector, const double &energyInkeV);
    ~ActionInitialization() override;

    void BuildForMaster() const override;
    void Build() const override;

    double PrimaryEnergyInkeV;
    DetectorConstruction* Detector = nullptr;
};
