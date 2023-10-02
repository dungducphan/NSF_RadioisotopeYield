#pragma once

#include <G4VUserActionInitialization.hh>

#include <PrimaryGenerator.h>
#include <RunAction.h>

class ActionInitialization : public G4VUserActionInitialization {
public:
    ActionInitialization();
    ~ActionInitialization() override;

    void BuildForMaster() const override;
    void Build() const override;
};
