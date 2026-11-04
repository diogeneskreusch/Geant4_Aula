// Ponto de entrada da aplicacao didatica Geant4.
// Compilacao e execucao: consulte README.md.

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"

#include "FTFP_BERT.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"

#ifdef DEMO_WITH_VIS
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#endif

int main(int argc, char** argv)
{
    if (argc > 2) {
        G4cerr << "Uso: " << argv[0] << " [arquivo.mac]" << G4endl;
        return 2;
    }

    // Modo sequencial para simplificar a logica de scoring na primeira aula.
    auto* runManager = new G4RunManager();

    auto* detector = new DetectorConstruction();
    runManager->SetUserInitialization(detector);
    runManager->SetUserInitialization(new FTFP_BERT());
    runManager->SetUserInitialization(new ActionInitialization(detector));

    auto* uiManager = G4UImanager::GetUIpointer();
    int exitCode = 0;

    if (argc == 2) {
        // Modo batch: o macro contem /run/initialize e /run/beamOn.
        const G4String command = "/control/execute ";
        const auto status = uiManager->ApplyCommand(command + argv[1]);
        if (status != 0) {
            G4cerr << "Falha ao executar o macro: " << argv[1]
                   << " (codigo " << status << ")" << G4endl;
            exitCode = 1;
        }
    } else {
#ifdef DEMO_WITH_VIS
        // Modo interativo: o arquivo vis.mac fica no diretorio de trabalho.
        auto* visManager = new G4VisExecutive();
        visManager->Initialize();

        auto* ui = new G4UIExecutive(argc, argv);
        const auto status = uiManager->ApplyCommand("/control/execute vis.mac");
        if (status != 0) {
            G4cerr << "Nao foi possivel executar vis.mac (codigo "
                   << status << ")" << G4endl;
        }
        ui->SessionStart();
        delete ui;
        delete visManager;
#else
        G4cerr << "Versao sem visualizacao. Execute com um macro: "
               << argv[0] << " run.mac" << G4endl;
        exitCode = 2;
#endif
    }

    // O RunManager gerencia e libera as classes registradas.
    delete runManager;
    return exitCode;
}
