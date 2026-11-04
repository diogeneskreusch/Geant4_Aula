#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4Gamma.hh"
#include "G4LogicalVolume.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"

SteppingAction::SteppingAction(const DetectorConstruction* detector,
                               EventAction* eventAction)
    : fDetector(detector), fEventAction(eventAction)
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    const auto* pre = step->GetPreStepPoint();
    const auto* volume = pre->GetPhysicalVolume();
    if (!volume) return; // Ex.: particula fora da geometria.

    if (volume->GetLogicalVolume() != fDetector->GetDetectorLogicalVolume())
        return;

    // (1) Energia depositada no Si por TODAS as particulas.
    fEventAction->AddEnergyDeposit(step->GetTotalEnergyDeposit());

    // (2) Chegada de gamma: primeiro step no detector, na fronteira.
    //     Inclui gammas primarios ou secundarios, espalhados ou nao.
    //     Um eventual retorno ao detector nao conta outro evento.
    if (step->GetTrack()->GetParticleDefinition() == G4Gamma::GammaDefinition()
        && pre->GetStepStatus() == fGeomBoundary) {
        fEventAction->MarkGammaArrival();
    }
}
