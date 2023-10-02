#include <RunAction.h>

RunAction::RunAction() : G4UserRunAction() {
    AnalysisManager.reset(G4AnalysisManager::Instance());
}

RunAction::~RunAction() = default;

void RunAction::BeginOfRunAction(const G4Run* run) {
    G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
}

void RunAction::EndOfRunAction(const G4Run* run) {
    G4cout << "### Run " << run->GetRunID() << " end." << G4endl;
}
