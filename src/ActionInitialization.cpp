#include <ActionInitialization.h>

ActionInitialization::ActionInitialization(const double &energyInkeV) : G4VUserActionInitialization(), PrimaryEnergyInkeV(energyInkeV) {}

ActionInitialization::~ActionInitialization() = default;

void ActionInitialization::BuildForMaster() const {
    SetUserAction(new RunAction(PrimaryEnergyInkeV));
}

void ActionInitialization::Build() const {
    SetUserAction(new PrimaryGenerator(PrimaryEnergyInkeV));
    SetUserAction(new RunAction(PrimaryEnergyInkeV));
}
