#pragma once

#include <G4UserRunAction.hh>
#include <G4AnalysisManager.hh>
#include <G4MTRunManager.hh>
#include <G4Run.hh>
#include <G4LogicalVolume.hh>
#include <G4SystemOfUnits.hh>

class RunAction : public G4UserRunAction {
public:
    RunAction();
    ~RunAction() override;

    void BeginOfRunAction(const G4Run* run) override;
    void EndOfRunAction(const G4Run* run) override;

private:
    std::unique_ptr<G4AnalysisManager> AnalysisManager;
};
