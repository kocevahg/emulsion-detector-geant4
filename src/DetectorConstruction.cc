#include "DetectorConstruction.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"

namespace B1
{

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  G4bool checkOverlaps = true;
  
//////////////////////////////////////////////////////////////////////////////
  // World
//////////////////////////////////////////////////////////////////////////////

  G4double world_size = 1 * m;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  auto solidWorld = new G4Box("World", 0.5 * world_size, 0.5 * world_size, 0.5 * world_size);
  auto logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", 
                                     nullptr, false, 0, checkOverlaps);

//////////////////////////////////////////////////////////////////////////////
  // Steel Box
//////////////////////////////////////////////////////////////////////////////

  G4double boxX=12.808*cm;
  G4double boxY=10.308*cm;
  G4double boxZ=1.1238*cm;
  G4double boxThickness=0.15*cm;
  
  G4Material* al_mat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  
  auto solidBox = new G4Box("Steel Box", 0.5*boxX,0.5*boxY,0.5*boxZ);
  auto logicBox = new G4LogicalVolume(solidBox,al_mat,"Steel Box");
  auto physBox = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), logicBox, "Steel Box", 
                    logicWorld, false, 0, checkOverlaps);
  
  G4Colour grey(0.5,0.5,0.5);
  G4VisAttributes* VisAtt_box=new G4VisAttributes(grey);
  VisAtt_box->SetVisibility(true);
  //VisAtt_box->SetForceSolid(true);
  VisAtt_box->SetForceWireframe(true);
  logicBox->SetVisAttributes(VisAtt_box);
  
 //////////////////////////////////////////////////////////////////////////////
  // Rubber Mat
 //////////////////////////////////////////////////////////////////////////////
 
  G4double rubberX=12.508*cm;
  G4double rubberY=10.008*cm;
  G4double rubberZ=0.3*cm;
  
  //material (Polyisoprene - C5H8)
  G4Material* rubber_mat = new G4Material("NaturalRubber", 0.92*g/cm3, 2);
  rubber_mat->AddElement(nist->FindOrBuildElement("C"), 5);  
  rubber_mat->AddElement(nist->FindOrBuildElement("H"), 8);  
  
  // Rubber Mat Right
  auto solidRubberPlateRight = new G4Box("Right Rubber Plate", 0.5*rubberX,0.5*rubberY,0.5*rubberZ);
  auto logicRubberPlateRight = new G4LogicalVolume(solidRubberPlateRight, rubber_mat,"Right Rubber Plate");
  auto physRubberPlateRight = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0.5*boxZ-boxThickness-0.5*rubberZ), logicRubberPlateRight, "Right Rubber Plate", 
                    logicBox, false, 0, checkOverlaps);
  
  // Rubber Mat Left
  auto solidRubberPlateLeft = new G4Box("Left Rubber Plate", 0.5*rubberX,0.5*rubberY,0.5*rubberZ);
  auto logicRubberPlateLeft = new G4LogicalVolume(solidRubberPlateLeft, rubber_mat,"Left Rubber Plate");
  auto physRubberPlateLeft = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -0.5*boxZ+boxThickness+0.5*rubberZ), logicRubberPlateLeft, "Left Rubber Plate", 
                    logicBox, false, 0, checkOverlaps);
  
  G4Colour lavander(0.29,0.0,0.51);
  G4VisAttributes* VisAtt_rubber=new G4VisAttributes(lavander);
  VisAtt_rubber->SetVisibility(true);
  VisAtt_rubber->SetForceSolid(true);
  //VisAtt_rubber->SetForceWireframe(true);
  logicRubberPlateRight->SetVisAttributes(VisAtt_rubber);
  logicRubberPlateLeft->SetVisAttributes(VisAtt_rubber);
  
//////////////////////////////////////////////////////////////////////////////
  // Packet
//////////////////////////////////////////////////////////////////////////////
G4double rubberLeftPosZ = -0.5*boxZ + boxThickness + 0.5*rubberZ;

G4double packetX = 12.508*cm;
G4double packetY = 10.008*cm;
G4double packetZ = 0.0373*cm;
G4double packetThickness = 0.004*cm;

G4Material* polyethylene = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

G4double emulsionThickness = 0.0044*cm;
G4double emulsionX = 12.5*cm;
G4double emulsionY = 10.0*cm;
G4double emulsionZ = emulsionThickness;

G4Element* elAg = nist->FindOrBuildElement("Ag");
G4Element* elBr = nist->FindOrBuildElement("Br");

G4Material* elAgBr = new G4Material("AgBr", 6.473*g/cm3, 2);
elAgBr->AddElement(elAg, 1);  
elAgBr->AddElement(elBr, 1); 

G4Material* gelatin = new G4Material("Gelatin", 1.35*g/cm3, 4);
gelatin->AddElement(nist->FindOrBuildElement("C"), 5);   
gelatin->AddElement(nist->FindOrBuildElement("H"), 8);   
gelatin->AddElement(nist->FindOrBuildElement("O"), 2);   
gelatin->AddElement(nist->FindOrBuildElement("N"), 1);

G4Material* emulsion_mat = new G4Material("AgBr_Gelatin", (0.85*6.473 + 0.15*1.35)*g/cm3, 2, kStateSolid);
emulsion_mat->AddMaterial(elAgBr, 0.85);    
emulsion_mat->AddMaterial(gelatin, 0.15);

G4double PlasticPlateThickness = 0.0205*cm;
G4double PlasticPlateX = 12.5*cm;
G4double PlasticPlateY = 10.0*cm;
G4double PlasticPlateZ = PlasticPlateThickness;

G4Material* plastic_mat = nist->FindOrBuildMaterial("G4_POLYSTYRENE");


    G4Colour lgrey(0.7, 0.7, 0.8);
    G4VisAttributes* VisAtt_packet = new G4VisAttributes(lgrey);
    VisAtt_packet->SetVisibility(true);
    VisAtt_packet->SetForceWireframe(true);
	
    G4Colour hotpink(1.0,0.41,0.71);
    G4VisAttributes* VisAtt_emulsion = new G4VisAttributes(hotpink);
    VisAtt_emulsion->SetVisibility(true);
    VisAtt_emulsion->SetForceSolid(true);
	
	G4Colour white(1., 1., 1.);
    G4VisAttributes* VisAtt_plastic = new G4VisAttributes(white);
    VisAtt_plastic->SetVisibility(true);
    VisAtt_plastic->SetForceSolid(true);
	

//starting position after left rubber mat
G4double startZ = rubberLeftPosZ + 0.5*rubberZ + 0.5*packetZ;

for (G4int i = 0; i < 6; i++) {
    G4String packetName = "Packet_" + std::to_string(i+1);
    
    auto solidPacket = new G4Box(packetName, 0.5*packetX, 0.5*packetY, 0.5*packetZ);
    auto logicPacket = new G4LogicalVolume(solidPacket, polyethylene, packetName);
    
	G4double packetPosZ = startZ + i * packetZ;
	
	auto physPacket = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, packetPosZ), 
                        logicPacket, packetName, logicBox, false, i, checkOverlaps);
	
	logicPacket->SetVisAttributes(VisAtt_packet);
	
    //////////////////////////////////////////////////////////////////////////////
    // Emulsion (for each packet)
    //////////////////////////////////////////////////////////////////////////////
	
    auto solidEmulsion = new G4Box("Emulsion", 0.5*emulsionX, 0.5*emulsionY, 0.5*emulsionZ);
    auto logicEmulsion = new G4LogicalVolume(solidEmulsion, emulsion_mat, "Emulsion");
    
    auto physEmulsion1 = new G4PVPlacement(nullptr,G4ThreeVector(0, 0, 0.5*packetZ - 0.5*emulsionThickness - packetThickness), 
                         logicEmulsion, "Emulsion1_" + std::to_string(i), logicPacket, false, 0, checkOverlaps);
    
    auto physEmulsion2 = new G4PVPlacement(nullptr,G4ThreeVector(0, 0, -0.5*packetZ + 0.5*emulsionThickness + packetThickness), 
                         logicEmulsion, "Emulsion2_" + std::to_string(i), logicPacket, false, 1, checkOverlaps);
	
	logicEmulsion->SetVisAttributes(VisAtt_emulsion);
	
	//////////////////////////////////////////////////////////////////////////////
    // Plastic Plate (for each packet)
    //////////////////////////////////////////////////////////////////////////////
	
    auto solidPlasticPlate = new G4Box("Plastic Plate", 0.5*PlasticPlateX, 0.5*PlasticPlateY, 0.5*PlasticPlateZ);
    auto logicPlasticPlate = new G4LogicalVolume(solidPlasticPlate, plastic_mat, "Plastic Plate");
    auto physPlasticPlate = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), 
    logicPlasticPlate, "Plastic Plate_" + std::to_string(i), logicPacket, false, 0, checkOverlaps);
	
    logicPlasticPlate->SetVisAttributes(VisAtt_plastic);

    }
  
  
	

  return physWorld; 
}
}

// namespace B1
