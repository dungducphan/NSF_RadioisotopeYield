#include <PrimaryGenerator.h>

PrimaryGenerator::PrimaryGenerator() : G4VUserPrimaryGeneratorAction() {
    GeneralParticleSource = std::make_unique<G4GeneralParticleSource>();

    // FIXME: Define specification for the particle source
}

PrimaryGenerator::~PrimaryGenerator() = default;

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    GeneralParticleSource->GeneratePrimaryVertex(anEvent);
}


