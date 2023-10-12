#pragma once

#include <G4VPhysicsConstructor.hh>

class BiasingPhysics : public G4VPhysicsConstructor {
public:
    BiasingPhysics();

    void ConstructParticle() override;
    void ConstructProcess() override;
};
