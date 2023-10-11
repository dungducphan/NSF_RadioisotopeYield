#include <G4MTRunManager.hh>
#include <QGSP_BIC_AllHP.hh>
#include <G4SteppingVerbose.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>
#include <G4HadronPhysicsQGSP_BIC_AllHP.hh>

#include <DetectorConstruction.h>
#include <ActionInitialization.h>

int main(int argc, char **argv) {
    G4UIExecutive *ui = nullptr;
    if (argc == 1) ui = new G4UIExecutive(argc, argv);

    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);

    auto runManager = new G4MTRunManager();

    runManager->SetUserInitialization(new DetectorConstruction());
    auto physicsList = new QGSP_BIC_AllHP();
    G4HadronPhysicsQGSP_BIC_AllHP* inelasticHadronPhys = (G4HadronPhysicsQGSP_BIC_AllHP*) physicsList->GetPhysics("hInelastic QGSP_BIC_AllHP");
    if (inelasticHadronPhys) {
        std::cout << "Found Inelastic Hadronic Physics Model." << std::endl;
    }
    runManager->SetUserInitialization(physicsList);
    runManager->SetUserInitialization(new ActionInitialization());

    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();

    G4UImanager *UIManager = G4UImanager::GetUIpointer();

    if (!ui) {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UIManager->ApplyCommand(command + fileName);
    } else {
        UIManager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
}