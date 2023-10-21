#pragma once

#include <G4UserSteppingAction.hh>
#include <G4Step.hh>
#include <G4AnalysisManager.hh>

class DetectorConstruction;

class SteppingAction  : public G4UserSteppingAction {
public:
    explicit SteppingAction(DetectorConstruction* detector);
    ~SteppingAction() override;

    void UserSteppingAction(const G4Step*) override;
    static void SaveParticleData(const G4Step* aStep) ;

private:
    DetectorConstruction* Detector = nullptr;
};
