#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction()
{
    // EXERCICIO: altere 1.0 para 3.0, recompile e execute novamente.
    fLeadThickness = 1.0 * cm;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    auto* nist = G4NistManager::Instance();
    auto* air = nist->FindOrBuildMaterial("G4_AIR");
    auto* lead = nist->FindOrBuildMaterial("G4_Pb");
    auto* silicon = nist->FindOrBuildMaterial("G4_Si");

    // World de 40 cm de lado. G4Box recebe MEIAS DIMENSOES.
    auto* worldSolid = new G4Box("WorldSolid", 20.*cm, 20.*cm, 20.*cm);
    auto* worldLV = new G4LogicalVolume(worldSolid, air, "WorldLV");
    auto* worldPV = new G4PVPlacement(nullptr, G4ThreeVector(),
                                      worldLV, "WorldPV", nullptr,
                                      false, 0, true);

    // Blindagem: largura 6 cm, altura 6 cm, espessura editavel.
    auto* pbSolid = new G4Box("PbSolid", 3.*cm, 3.*cm, fLeadThickness/2.);
    auto* pbLV = new G4LogicalVolume(pbSolid, lead, "PbLV");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0),
                      pbLV, "PbPV", worldLV, false, 0, true);

    // Detector de silicio: 6 x 6 cm, espessura 1 mm, centro z = +8 cm.
    auto* siSolid = new G4Box("SiSolid", 3.*cm, 3.*cm, 0.5*mm);
    fDetectorLV = new G4LogicalVolume(siSolid, silicon, "DetectorLV");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 8.*cm),
                      fDetectorLV, "DetectorPV", worldLV, false, 0, true);

    // Aparencia somente para visualizacao. Nao afeta a fisica.
    worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());
    auto* pbVis = new G4VisAttributes(G4Colour(0.65, 0.70, 0.75));
    pbVis->SetForceWireframe(true); // Permite visualizar trajetorias no Pb.
    pbLV->SetVisAttributes(pbVis);
    auto* siVis = new G4VisAttributes(G4Colour(0.15, 0.78, 0.85, 0.5));
    siVis->SetForceSolid(true);
    fDetectorLV->SetVisAttributes(siVis);

    return worldPV;
}
