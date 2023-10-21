#include <SteppingAction.h>
#include <DetectorConstruction.h>

SteppingAction::SteppingAction(DetectorConstruction* det) : Detector(det) {}

SteppingAction::~SteppingAction() = default;

void SteppingAction::UserSteppingAction(const G4Step* aStep) {
    G4VPhysicalVolume* postVolume = aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume();

    if (postVolume == Detector->DetectorPhysicalVolume) {
        // G4cout << aStep->GetTrack()->GetParticleDefinition()->GetParticleName() << " : " << aStep->GetTrack()->GetDynamicParticle()->GetPDGcode() << G4endl;
        SaveParticleData(aStep);
        aStep->GetTrack()->SetTrackStatus(fStopAndKill);
    } else return;
}

void SteppingAction::SaveParticleData(const G4Step *aStep) {
    G4AnalysisManager* AnalysisManager = G4AnalysisManager::Instance();
    AnalysisManager->FillNtupleDColumn(0, aStep->GetTrack()->GetDynamicParticle()->GetPDGcode());
    AnalysisManager->FillNtupleDColumn(1, aStep->GetPreStepPoint()->GetKineticEnergy());
    AnalysisManager->FillNtupleDColumn(2, aStep->GetTrack()->GetWeight());
    AnalysisManager->AddNtupleRow(0);
}
