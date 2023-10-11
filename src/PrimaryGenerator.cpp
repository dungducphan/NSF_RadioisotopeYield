#include <PrimaryGenerator.h>

PrimaryGenerator::PrimaryGenerator() : G4VUserPrimaryGeneratorAction() {
    GeneralParticleSource = std::make_unique<G4GeneralParticleSource>();
    GeneralParticleSource->SetNumberOfParticles(1000);
    G4SingleParticleSource *deuteronSrc = GeneralParticleSource->GetCurrentSource();
    G4ParticleDefinition *deuteronDef = G4ParticleTable::GetParticleTable()->FindParticle("deuteron");
    deuteronSrc->SetParticleDefinition(deuteronDef);
    deuteronSrc->GetPosDist()->SetPosDisType("Point"); // Point, Beam, Plane, Surface, Volume
    deuteronSrc->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., 0.));
    deuteronSrc->GetPosDist()->ConfineSourceToVolume("NULL");
    deuteronSrc->GetAngDist()->SetAngDistType("iso");
    deuteronSrc->GetEneDist()->SetEnergyDisType("Gauss");
    deuteronSrc->GetEneDist()->SetMonoEnergy(2.5 * MeV);
    deuteronSrc->GetEneDist()->SetBeamSigmaInE(0.08 * MeV);
}

PrimaryGenerator::~PrimaryGenerator() = default;

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    GeneralParticleSource->GeneratePrimaryVertex(anEvent);
}


