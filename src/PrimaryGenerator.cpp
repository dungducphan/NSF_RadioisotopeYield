#include <PrimaryGenerator.h>

PrimaryGenerator::PrimaryGenerator() : G4VUserPrimaryGeneratorAction() {
    GeneralParticleSource = std::make_unique<G4GeneralParticleSource>();
    GeneralParticleSource->SetNumberOfParticles(1000);

    GeneralParticleSource->AddaSource(0.99);
    G4SingleParticleSource *deuteronSrc = GeneralParticleSource->GetCurrentSource();
    G4ParticleDefinition *deuteronDef = G4ParticleTable::GetParticleTable()->FindParticle("deuteron");
    deuteronSrc->SetParticleDefinition(deuteronDef);
    deuteronSrc->GetPosDist()->SetPosDisType("Point"); // Point, Beam, Plane, Surface, Volume
    deuteronSrc->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., 0.));
    deuteronSrc->GetPosDist()->ConfineSourceToVolume("NULL");
    deuteronSrc->GetAngDist()->SetAngDistType("iso");
//    deuteronSrc->GetAngDist()->SetMinTheta(0. * rad);
//    deuteronSrc->GetAngDist()->SetMaxTheta(TMath::Pi() * rad);
//    deuteronSrc->GetAngDist()->SetMinPhi(0 * rad);
//    deuteronSrc->GetAngDist()->SetMaxPhi(TMath::Pi() * 2. * rad);
    deuteronSrc->GetEneDist()->SetEnergyDisType("Gauss");
    deuteronSrc->GetEneDist()->SetMonoEnergy(2.5 * MeV);
    deuteronSrc->GetEneDist()->SetBeamSigmaInE(0.08 * MeV);

//    GeneralParticleSource->AddaSource(0.01);
//    G4SingleParticleSource *neutronSrc = GeneralParticleSource->GetCurrentSource();
//    G4ParticleDefinition *neutronDef = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
//    neutronSrc->SetParticleDefinition(neutronDef);
//    neutronSrc->GetPosDist()->SetPosDisType("Point");
//    neutronSrc->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., 0.));
//    neutronSrc->GetPosDist()->ConfineSourceToVolume("NULL");
//    neutronSrc->GetAngDist()->SetAngDistType("iso");
////    neutronSrc->GetAngDist()->SetMinTheta(0. * rad);
////    neutronSrc->GetAngDist()->SetMaxTheta(TMath::Pi() * rad);
////    neutronSrc->GetAngDist()->SetMinPhi(0 * rad);
////    neutronSrc->GetAngDist()->SetMaxPhi(TMath::Pi() * 2. * rad);
//    neutronSrc->GetEneDist()->SetEnergyDisType("Gauss"); // Mono, Lin, Pow, Exp, Gaus, Brem, BBody, Cdg (cosmic diffuse gamma), User, Arb, Epn (energy per nucleon)
//    neutronSrc->GetEneDist()->SetMonoEnergy(2.5 * MeV);
//    neutronSrc->GetEneDist()->SetBeamSigmaInE(0.08 * MeV);
}

PrimaryGenerator::~PrimaryGenerator() = default;

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    GeneralParticleSource->GeneratePrimaryVertex(anEvent);
}


