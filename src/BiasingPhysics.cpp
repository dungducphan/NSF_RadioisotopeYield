#include <BiasingPhysics.h>

#include <G4HadronicProcess.hh>
#include <G4ProcessVector.hh>
#include <G4ProcessManager.hh>

BiasingPhysics::BiasingPhysics() : G4VPhysicsConstructor() {}

void BiasingPhysics::ConstructParticle() {}

void BiasingPhysics::ConstructProcess() {
     BiasingDeuteronHPCrossSection();
}

void BiasingPhysics::BiasingDeuteronHPCrossSection() {
    G4HadronicProcess* theHadronicInelasticProc = nullptr;
    G4ProcessVector* pDeuteronProcessVec = G4Deuteron::Deuteron()->GetProcessManager()->GetProcessList();
    for (size_t i = 0; i < pDeuteronProcessVec->size(); i++) {
        if ((*pDeuteronProcessVec)[i]->GetProcessName() == "dInelastic") {
            theHadronicInelasticProc = dynamic_cast<G4HadronicProcess*>((*pDeuteronProcessVec)[i]);
            break;
        }
    }
    if (theHadronicInelasticProc) {
        theHadronicInelasticProc->BiasCrossSectionByFactor(1E3);
    }
}
