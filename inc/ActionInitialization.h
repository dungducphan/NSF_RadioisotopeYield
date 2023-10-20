#pragma once

#include <G4VUserActionInitialization.hh>

#include <PrimaryGenerator.h>
#include <RunAction.h>

class ActionInitialization : public G4VUserActionInitialization {
public:
    explicit ActionInitialization(const double &energyInkeV);
    ~ActionInitialization() override;

    void BuildForMaster() const override;
    void Build() const override;

    double PrimaryEnergyInkeV;
};
