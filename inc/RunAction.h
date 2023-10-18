#pragma once

#include <G4UserRunAction.hh>
#include <G4AnalysisManager.hh>
#include <G4MTRunManager.hh>
#include <G4Run.hh>
#include <G4LogicalVolume.hh>
#include <G4SystemOfUnits.hh>

class RunAction : public G4UserRunAction {
public:
    explicit RunAction(const double &energyInMeV);
    ~RunAction() override;

    void BeginOfRunAction(const G4Run* run) override;
    void EndOfRunAction(const G4Run* run) override;

    double PrimaryEnergyInMeV;
};
