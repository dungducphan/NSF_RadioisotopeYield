#include <PrimaryGenerator.h>

PrimaryGenerator::PrimaryGenerator(const double &energyInkeV) : G4VUserPrimaryGeneratorAction() {
    GeneralParticleSource = new G4GeneralParticleSource();
    GeneralParticleSource->SetNumberOfParticles(400);
    G4SingleParticleSource *deuteronSrc = GeneralParticleSource->GetCurrentSource();
    G4ParticleDefinition *deuteronDef = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
    deuteronSrc->SetParticleDefinition(deuteronDef);
    deuteronSrc->GetPosDist()->SetPosDisType("Point"); // Point, Beam, Plane, Surface, Volume
    deuteronSrc->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., 0.));
    deuteronSrc->GetPosDist()->ConfineSourceToVolume("NULL");
    deuteronSrc->GetAngDist()->SetAngDistType("iso");
    deuteronSrc->GetEneDist()->SetEnergyDisType("Gauss");
    deuteronSrc->GetEneDist()->SetMonoEnergy(2.45 * MeV);
    deuteronSrc->GetEneDist()->SetBeamSigmaInE(0.1 * MeV);
}

PrimaryGenerator::~PrimaryGenerator() = default;

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    GeneralParticleSource->GeneratePrimaryVertex(anEvent);
}


