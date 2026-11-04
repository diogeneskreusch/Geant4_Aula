#include "RunAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>

RunAction::RunAction(const DetectorConstruction* detector,
                     const PrimaryGeneratorAction* primary)
    : fDetector(detector), fPrimary(primary)
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
    fNEvents = 0;
    fNGammaArrivals = 0;
    fSumEdepKeV = 0.;
    fSumEdep2KeV2 = 0.;
}

void RunAction::RecordEvent(G4bool gammaArrived, G4double energyDeposit)
{
    ++fNEvents;
    if (gammaArrived) ++fNGammaArrivals;

    const G4double eKeV = energyDeposit/keV;
    fSumEdepKeV += eKeV;
    fSumEdep2KeV2 += eKeV*eKeV;
}

void RunAction::EndOfRunAction(const G4Run* run)
{
    if (fNEvents == 0) {
        G4cout << "Nenhum evento foi processado neste run." << G4endl;
        return;
    }

    const G4double n = static_cast<G4double>(fNEvents);
    const G4double fraction = static_cast<G4double>(fNGammaArrivals)/n;
    const G4double seFraction = std::sqrt(fraction*(1. - fraction)/n);
    const G4double meanEdepKeV = fSumEdepKeV/n;

    // Erro padrao da media: s / sqrt(N), com s^2 amostral.
    G4double seMeanEdepKeV = 0.;
    if (fNEvents > 1) {
        const G4double sampleVariance = std::max(
            0., (fSumEdep2KeV2 - n*meanEdepKeV*meanEdepKeV)/(n - 1.));
        seMeanEdepKeV = std::sqrt(sampleVariance/n);
    }

    const G4double energyKeV = fPrimary->GetParticleGun()->GetParticleEnergy()/keV;
    const G4double leadCm = fDetector->GetLeadThickness()/cm;

    G4cout << "\n===============================================\n"
           << " RESULTADOS - RUN " << run->GetRunID() << "\n"
           << "-----------------------------------------------\n"
           << " E(gamma)                 : " << energyKeV << " keV\n"
           << " Espessura Pb             : " << leadCm << " cm\n"
           << " Eventos simulados        : " << fNEvents << "\n"
           << " Eventos com chegada gamma: " << fNGammaArrivals << "\n"
           << std::setprecision(8)
           << " Fracao de chegada        : " << fraction
           << " +/- " << seFraction << " (1 sigma)\n"
           << " Edep media por evento    : " << meanEdepKeV
           << " +/- " << seMeanEdepKeV << " keV (1 sigma)\n"
           << "===============================================\n" << G4endl;

    // Acrescenta uma linha por run; nao sobrescreve resultados anteriores.
    const char* outputName = "resultados.csv";
    std::ifstream previous(outputName);
    const bool needsHeader = !previous.good()
        || previous.peek() == std::ifstream::traits_type::eof();
    previous.close();

    std::ofstream out(outputName, std::ios::app);
    if (!out) {
        G4cerr << "AVISO: nao foi possivel gravar " << outputName << G4endl;
        return;
    }
    if (needsHeader) {
        out << "run_id,energia_keV,espessura_Pb_cm,eventos,"
            << "eventos_com_gamma,fracao_gamma,erro_fracao_1sigma,"
            << "edep_media_keV,erro_edep_media_keV,"
            << "soma_edep_keV,soma_edep_quadrado_keV2\n";
    }
    out << std::setprecision(12)
        << run->GetRunID() << ',' << energyKeV << ',' << leadCm << ','
        << fNEvents << ',' << fNGammaArrivals << ',' << fraction << ','
        << seFraction << ',' << meanEdepKeV << ',' << seMeanEdepKeV << ','
        << fSumEdepKeV << ',' << fSumEdep2KeV2 << '\n';
    G4cout << "Resultados acrescentados em: " << outputName << G4endl;
}
