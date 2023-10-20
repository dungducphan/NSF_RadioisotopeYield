#include <RunAction.h>

#include <TString.h>

RunAction::RunAction(const double &energyInkeV) : G4UserRunAction(), PrimaryEnergyInkeV(energyInkeV) {
    G4AnalysisManager* AnalysisManager = G4AnalysisManager::Instance();
    if (G4Threading::IsMultithreadedApplication()) AnalysisManager->SetNtupleMerging(true);

    AnalysisManager->CreateNtuple("DeuteronNTuple", "DeuteronNTuple");
    AnalysisManager->CreateNtupleDColumn("PDGCode");
    AnalysisManager->CreateNtupleDColumn("Energy");
    AnalysisManager->CreateNtupleDColumn("Weight");
    AnalysisManager->FinishNtuple(0);
}

RunAction::~RunAction() = default;

void RunAction::BeginOfRunAction(const G4Run* run) {
    G4AnalysisManager* AnalysisManager = G4AnalysisManager::Instance();
    AnalysisManager->OpenFile(Form("DeuteronData_With_%05i_keV_Deuterons.root", (int) PrimaryEnergyInkeV));
}

void RunAction::EndOfRunAction(const G4Run* run) {
    G4AnalysisManager* AnalysisManager = G4AnalysisManager::Instance();
    AnalysisManager->Write();
    AnalysisManager->CloseFile();
}
