#include <thread>

#include <G4MTRunManager.hh>
#include <QGSP_BIC_AllHP.hh>
#include <G4SteppingVerbose.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>

#include <DetectorConstruction.h>
#include <ActionInitialization.h>
#include <BiasingPhysics.h>

int main(int argc, char **argv) {
    G4UIExecutive *ui = nullptr;

    double energyInkeV = 0.0;
    int noOfEvents = 0;
    if (argc == 3) {
        ui = new G4UIExecutive(argc, argv);
        energyInkeV = std::stod(argv[1]);
        noOfEvents = std::stoi(argv[2]);
    } else if (argc == 4) {
        energyInkeV = std::stod(argv[1]);
        noOfEvents = std::stoi(argv[2]);
    } else {
        G4cout << "Usage: " << argv[0] << " <Primary Particle Energy in keV> <Number of Events> [optional: --batchMode]" << G4endl;
        return 0;
    }

    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);

    auto runManager = new G4MTRunManager();
    auto detectorConstruction = new DetectorConstruction();
    runManager->SetUserInitialization(detectorConstruction);
    auto physicsList = new QGSP_BIC_AllHP();
    runManager->SetUserInitialization(physicsList);
    runManager->SetUserInitialization(new ActionInitialization(detectorConstruction, energyInkeV));

#if(DEBUG)
    runManager->SetNumberOfThreads(1);
#else
    runManager->SetNumberOfThreads((G4int) std::thread::hardware_concurrency() - 4);
#endif
    runManager->Initialize();

    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();

    G4UImanager *UIManager = G4UImanager::GetUIpointer();

    if (!ui) {
          runManager->BeamOn(noOfEvents);
    } else {
        UIManager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
}