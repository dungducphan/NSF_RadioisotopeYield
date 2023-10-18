#include <PrimaryGenerator.h>

PrimaryGenerator::PrimaryGenerator(const double &energyInMeV) : G4VUserPrimaryGeneratorAction() {
    GeneralParticleSource = new G4GeneralParticleSource();
    GeneralParticleSource->SetNumberOfParticles(4800);
    G4SingleParticleSource *deuteronSrc = GeneralParticleSource->GetCurrentSource();
    G4ParticleDefinition *deuteronDef = G4ParticleTable::GetParticleTable()->FindParticle("deuteron");
    deuteronSrc->SetParticleDefinition(deuteronDef);
    deuteronSrc->GetPosDist()->SetPosDisType("Point"); // Point, Beam, Plane, Surface, Volume
    deuteronSrc->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., 0.));
    deuteronSrc->GetPosDist()->ConfineSourceToVolume("NULL");
    deuteronSrc->GetAngDist()->SetAngDistType("iso");
    deuteronSrc->GetEneDist()->SetEnergyDisType("Mono");
    deuteronSrc->GetEneDist()->SetMonoEnergy(energyInMeV * MeV);
}

PrimaryGenerator::~PrimaryGenerator() = default;

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    GeneralParticleSource->GeneratePrimaryVertex(anEvent);
}


