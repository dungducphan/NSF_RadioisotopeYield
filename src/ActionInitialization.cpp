#include <ActionInitialization.h>

ActionInitialization::ActionInitialization(const double &energyInMeV) : G4VUserActionInitialization(), PrimaryEnergyInMeV(energyInMeV) {}

ActionInitialization::~ActionInitialization() = default;

void ActionInitialization::BuildForMaster() const {
    SetUserAction(new RunAction(PrimaryEnergyInMeV));
}

void ActionInitialization::Build() const {
    SetUserAction(new PrimaryGenerator(PrimaryEnergyInMeV));
    SetUserAction(new RunAction(PrimaryEnergyInMeV));
}
