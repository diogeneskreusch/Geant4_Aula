#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ThreeVector.hh"

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    auto* nist = G4NistManager::Instance();
    auto* air = nist->FindOrBuildMaterial("G4_AIR");
    auto* aluminum = nist->FindOrBuildMaterial("G4_Al");
    auto* lead = nist->FindOrBuildMaterial("G4_Pb");
    auto* silicon = nist->FindOrBuildMaterial("G4_Si");
    const G4bool checkOverlaps = true;

    auto* solidWorld = new G4Box("WorldSolid", 20.*cm, 20.*cm, 20.*cm);
    auto* logicWorld = new G4LogicalVolume(solidWorld, air, "WorldLV");
    auto* physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld,
                                        "WorldPV", nullptr, false, 0, checkOverlaps);
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

    const G4double alThickness = 2.0*cm;
    auto* solidAl = new G4Box("AlSolid", 3.*cm, 3.*cm, alThickness/2.);
    auto* logicAl = new G4LogicalVolume(solidAl, aluminum, "AlLV");
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,-3.*cm), logicAl,
                      "AlPV", logicWorld, false, 0, checkOverlaps);

    const G4double leadThickness = 1.0*cm;
    auto* solidPb = new G4Box("PbSolid", 3.*cm, 3.*cm, leadThickness/2.);
    auto* logicPb = new G4LogicalVolume(solidPb, lead, "PbLV");
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,1.*cm), logicPb,
                      "PbPV", logicWorld, false, 0, checkOverlaps);

    auto* solidSi = new G4Box("SiSolid", 3.*cm, 3.*cm, 0.5*mm);
    auto* logicSi = new G4LogicalVolume(solidSi, silicon, "SiLV");
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,10.*cm), logicSi,
                      "SiPV", logicWorld, false, 0, checkOverlaps);

    auto* alVis = new G4VisAttributes(G4Colour(0.24,0.68,0.82));
    alVis->SetForceSolid(true);
    logicAl->SetVisAttributes(alVis);
    auto* pbVis = new G4VisAttributes(G4Colour(0.66,0.71,0.78));
    pbVis->SetForceSolid(true);
    logicPb->SetVisAttributes(pbVis);
    auto* siVis = new G4VisAttributes(G4Colour(0.20,0.80,0.48));
    siVis->SetForceSolid(true);
    logicSi->SetVisAttributes(siVis);

    return physWorld;
}
