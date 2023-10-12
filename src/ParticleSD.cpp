#include <ParticleSD.h>

ParticleSD::ParticleSD(const G4String& name) : G4VSensitiveDetector(name) {}

ParticleSD::~ParticleSD() = default;

G4bool ParticleSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {
    auto particle = aStep->GetTrack()->GetDefinition();
    if (particle->IsGeneralIon()) {
        G4cout << "Registered a nucleus: " << particle->GetParticleName() << G4endl;
        G4cout << "Lifetime: " << particle->GetPDGLifeTime() / picosecond << " picoseconds." << G4endl;
    }

    return true;
}
