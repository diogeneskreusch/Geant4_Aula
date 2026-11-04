#include "EventAction.hh"
#include "RunAction.hh"

EventAction::EventAction(RunAction* runAction)
    : fRunAction(runAction)
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
    fGammaArrived = false;
    fEnergyDeposit = 0.;
}

void EventAction::EndOfEventAction(const G4Event*)
{
    // Cada evento contribui uma unica vez para a contagem de chegada.
    // Eventos sem deposicao contribuem com Edep = 0.
    fRunAction->RecordEvent(fGammaArrived, fEnergyDeposit);
}
