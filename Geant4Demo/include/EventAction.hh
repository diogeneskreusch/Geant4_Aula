#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;
class RunAction;

class EventAction final : public G4UserEventAction {
public:
    explicit EventAction(RunAction* runAction);
    ~EventAction() override = default;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

    void MarkGammaArrival() { fGammaArrived = true; }
    void AddEnergyDeposit(G4double amount) { fEnergyDeposit += amount; }

private:
    RunAction* fRunAction = nullptr;
    G4bool fGammaArrived = false;
    G4double fEnergyDeposit = 0.;
};

#endif
