#include <G4MTRunManager.hh>
#include <QGSP_BIC_AllHP.hh>
#include <G4SteppingVerbose.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>
#include <G4GeometrySampler.hh>
#include <G4ImportanceBiasing.hh>

#include <DetectorConstruction.h>
#include <ActionInitialization.h>
#include <BiasingPhysics.h>

int main(int argc, char **argv) {
    G4UIExecutive *ui = nullptr;

    double energyInMeV = 0.0;
    if (argc == 2) {
        ui = new G4UIExecutive(argc, argv);
        energyInMeV = std::stod(argv[1]);
    } else if (argc == 3) {
        energyInMeV = std::stod(argv[1]);
    } else {
        G4cout << "Usage: " << argv[0] << " <Primary Particle Energy in MeV> [optional: macro_file]" << G4endl;
        return 0;
    }

    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);

    auto runManager = new G4MTRunManager();

    auto detectorConstruction = new DetectorConstruction();
    runManager->SetUserInitialization(detectorConstruction);
    auto physicsList = new QGSP_BIC_AllHP();
    if (energyInMeV >= 6 && energyInMeV <= 16) {
        G4GeometrySampler mgs(detectorConstruction->GetWorldVolume(),"deuteron");
        physicsList->RegisterPhysics(new G4ImportanceBiasing(&mgs));
    }
    runManager->SetUserInitialization(physicsList);
    runManager->SetUserInitialization(new ActionInitialization(energyInMeV));

    runManager->SetNumberOfThreads(24);
    runManager->Initialize();
    if (energyInMeV >= 6 && energyInMeV <= 16) {
        detectorConstruction->CreateImportanceStore();
    }

    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();

    G4UImanager *UIManager = G4UImanager::GetUIpointer();

    if (!ui) {
        G4String command = "/control/execute ";
        G4String fileName = argv[2];
        UIManager->ApplyCommand(command + fileName);
    } else {
        UIManager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
}