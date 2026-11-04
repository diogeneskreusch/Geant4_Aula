#ifndef RUN_ACTION_HH
#define RUN_ACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"

class DetectorConstruction;
class PrimaryGeneratorAction;
class G4Run;

class RunAction final : public G4UserRunAction {
public:
    RunAction(const DetectorConstruction* detector,
              const PrimaryGeneratorAction* primary);
    ~RunAction() override = default;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run* run) override;

    void RecordEvent(G4bool gammaArrived, G4double energyDeposit);

private:
    const DetectorConstruction* fDetector = nullptr;
    const PrimaryGeneratorAction* fPrimary = nullptr;

    G4long fNEvents = 0;
    G4long fNGammaArrivals = 0;
    G4double fSumEdepKeV = 0.;
    G4double fSumEdep2KeV2 = 0.;
};

#endif
