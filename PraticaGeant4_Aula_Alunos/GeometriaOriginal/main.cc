#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "FTFP_BERT.hh"
#include "G4RunManager.hh"
#include "G4ScoringManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4ios.hh"

int main(int argc, char** argv)
{
    auto* runManager = new G4RunManager;

    // Ativa os comandos /score utilizados nas macros.
    G4ScoringManager::GetScoringManager();

    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new FTFP_BERT());
    runManager->SetUserInitialization(new ActionInitialization());

    // O suporte de visualizacao esta presente no executavel.
    auto* visManager = new G4VisExecutive;
    visManager->Initialize();

    auto* uiManager = G4UImanager::GetUIpointer();
    G4int status = 0;

    if (argc > 1) {
        // Modo batch: ./build/PraticaGeant4 macros/run.mac
        status = uiManager->ApplyCommand(G4String("/control/execute ") + argv[1]);
    } else {
        // Modo visual: ./build/PraticaGeant4
        auto* ui = new G4UIExecutive(argc, argv, "qt");
        status = uiManager->ApplyCommand("/control/execute macros/vis.mac");
        if (status == 0) ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
    return status == 0 ? 0 : 1;
}
