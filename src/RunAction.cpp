#include <RunAction.h>

RunAction::RunAction() : G4UserRunAction() {
    AnalysisManager.reset(G4AnalysisManager::Instance());
    AnalysisManager->SetVerboseLevel(1);
    AnalysisManager->SetNtupleMerging(true);
    AnalysisManager->SetFileName("output");
    AnalysisManager->CreateNtuple("output", "output");
    AnalysisManager->CreateNtupleDColumn("energy");
    AnalysisManager->CreateNtupleDColumn("time");
    AnalysisManager->CreateNtupleDColumn("x");
    AnalysisManager->CreateNtupleDColumn("y");
    AnalysisManager->CreateNtupleDColumn("z");
    AnalysisManager->CreateNtupleDColumn("px");
    AnalysisManager->CreateNtupleDColumn("py");
    AnalysisManager->CreateNtupleDColumn("pz");
    AnalysisManager->CreateNtupleDColumn("theta");
    AnalysisManager->CreateNtupleDColumn("phi");
    AnalysisManager->CreateNtupleDColumn("weight");
    AnalysisManager->FinishNtuple();
}

RunAction::~RunAction() = default;

void RunAction::BeginOfRunAction(const G4Run* run) {
    G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
    AnalysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run* run) {
    AnalysisManager->Write();
    AnalysisManager->CloseFile();
    G4cout << "### Run " << run->GetRunID() << " end." << G4endl;
}
