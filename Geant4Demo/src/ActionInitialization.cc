#include "ActionInitialization.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization(const DetectorConstruction* detector)
    : fDetector(detector)
{}

void ActionInitialization::Build() const
{
    auto* primary = new PrimaryGeneratorAction();
    SetUserAction(primary);

    auto* runAction = new RunAction(fDetector, primary);
    SetUserAction(runAction);

    auto* eventAction = new EventAction(runAction);
    SetUserAction(eventAction);

    SetUserAction(new SteppingAction(fDetector, eventAction));
}
