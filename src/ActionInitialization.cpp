#include <ActionInitialization.h>

ActionInitialization::ActionInitialization(DetectorConstruction* detector, const double &energyInkeV)
: G4VUserActionInitialization(),
  Detector(detector),
  PrimaryEnergyInkeV(energyInkeV) {
}

ActionInitialization::~ActionInitialization() = default;

void ActionInitialization::BuildForMaster() const {
    SetUserAction(new RunAction(PrimaryEnergyInkeV));
}

void ActionInitialization::Build() const {
    SetUserAction(new PrimaryGenerator(PrimaryEnergyInkeV));
    SetUserAction(new RunAction(PrimaryEnergyInkeV));
    SetUserAction(new SteppingAction(Detector));
}
