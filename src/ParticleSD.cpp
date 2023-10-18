#include <ParticleSD.h>

ParticleSD::ParticleSD(const G4String& name) : G4VSensitiveDetector(name) {
    DeuteronDefinition = G4ParticleTable::GetParticleTable()->FindParticle("deuteron");
}

ParticleSD::~ParticleSD() = default;

G4bool ParticleSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {
    auto particle = aStep->GetTrack()->GetDefinition();
    if (particle != DeuteronDefinition) {
        return false;
    }

    std::cout << "--------------------------------------------------------------------------Weight of deuteron: " << aStep->GetTrack()->GetWeight() << std::endl;

    auto prePoint = aStep->GetPreStepPoint();
    G4double energy = prePoint->GetKineticEnergy();

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(0, energy / MeV);
    man->AddNtupleRow();

    aStep->GetTrack()->SetTrackStatus(fStopAndKill);

    return true;
}
