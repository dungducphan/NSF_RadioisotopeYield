#include <PrimaryGenerator.h>

PrimaryGenerator::PrimaryGenerator(const double &energyInkeV) : G4VUserPrimaryGeneratorAction() {
    GeneralParticleSource = new G4GeneralParticleSource();
    GeneralParticleSource->SetNumberOfParticles(400);
    G4SingleParticleSource *deuteronSrc = GeneralParticleSource->GetCurrentSource();
    G4ParticleDefinition *deuteronDef = G4ParticleTable::GetParticleTable()->FindParticle("deuteron");
    deuteronSrc->SetParticleDefinition(deuteronDef);
    deuteronSrc->GetPosDist()->SetPosDisType("Point"); // Point, Beam, Plane, Surface, Volume
    deuteronSrc->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., 0.));
    deuteronSrc->GetPosDist()->ConfineSourceToVolume("NULL");
    deuteronSrc->GetAngDist()->SetAngDistType("iso");
    deuteronSrc->GetEneDist()->SetEnergyDisType("Mono");
    deuteronSrc->GetEneDist()->SetMonoEnergy(energyInkeV * keV);
}

PrimaryGenerator::~PrimaryGenerator() = default;

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    GeneralParticleSource->GeneratePrimaryVertex(anEvent);
}


