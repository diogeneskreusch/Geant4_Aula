#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;

class DetectorConstruction final : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    ~DetectorConstruction() override = default;

    G4VPhysicalVolume* Construct() override;

    G4LogicalVolume* GetDetectorLogicalVolume() const { return fDetectorLV; }
    G4double GetLeadThickness() const { return fLeadThickness; }

private:
    G4LogicalVolume* fDetectorLV = nullptr;  // Definido em Construct().
    G4double fLeadThickness = 0.;            // Unidade interna do Geant4.
};

#endif
