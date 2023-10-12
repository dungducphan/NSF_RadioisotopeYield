#include <BiasingPhysics.h>


#include <G4HadronicProcess.hh>
#include <G4ProcessVector.hh>
#include <G4ProcessManager.hh>


BiasingPhysics::BiasingPhysics() : G4VPhysicsConstructor() {}

void BiasingPhysics::ConstructParticle() {}

void BiasingPhysics::ConstructProcess() {
    G4HadronicProcess* theHadronicInelasticProc = nullptr;
    G4ProcessVector* pDeuteronProcessVec = G4Deuteron::Deuteron()->GetProcessManager()->GetProcessList();
    std::cout << pDeuteronProcessVec->size() << std::endl;
    for (size_t i = 0; i < pDeuteronProcessVec->size(); i++) {
        std::cout << (*pDeuteronProcessVec)[i]->GetProcessName() << std::endl;
        if ((*pDeuteronProcessVec)[i]->GetProcessName() == "dInelastic") {
            std::cout << "Found ParticleHPInelastic process!" << std::endl;
            theHadronicInelasticProc = dynamic_cast<G4HadronicProcess*>((*pDeuteronProcessVec)[i]);
            break;
        }
    }
    if (theHadronicInelasticProc) {
        theHadronicInelasticProc->BiasCrossSectionByFactor(1E12);
        std::cout << "Scaled x-sec!" << std::endl;
    }
}
