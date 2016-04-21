//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: FragExpConstruction.cc,v 1.9 2006/06/29 17:47:19 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <iostream>

#include "FragSimDetConstruction.hh"
#include "SensitiveSSD.hh"
#include "SensitiveMultiFuncParamSphere.hh"
#include "G4PSSphereSurfaceCurrent.hh"
#include "PSPreStepEnergy.hh"
#include "Target.hh"
#include "TargetSensitivity.hh"
#include "TargetMountBuilder.hh"

#include "G4NistManager.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"

#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"

#include "DataBucket.h"
#include "TreeMaker.hh"
#include "FragSimDetMessenger.hh"

#include "globals.hh"


FragSimDetConstruction::FragSimDetConstruction()
:
  vacuum(0),
  aluminum(0),
  U235(0),
  U238(0),
  natU(0),
  natUMaterial(0),
  experimentalHall_log(0), 
  chamberWall_log(0), 
  experimentalHall_phys(0),
  chamberWall_phys(0),
  tr0(NULL),
  tr1(NULL),
  tr2(NULL),
  tr3(NULL),
  rotMatrix(NULL),
  tr_tar(NULL),
  SSD0("UpStreamTop",0),
  SSD1("DownStreamLeft",1),
  SSD2("UpStreamRight",2),
  SSD3("DownStreamBottom",3),
  tar(),
  fTarShift(0,0,0),
  fRot1Shift(0),
  fRot2Shift(0),
  multiFuncSphere(),
  fRot_center_transl(),
  fWholeTransl(0,0,0),
  fMessenger(new FragSimDetMessenger(this))
{
  DefineMaterials();
  G4cout << "Instance of FragSimDetConstruction constructed" << G4endl;
  //  SSD0.GetBuilder().PrintGeometryInfo();
//  SetupTransforms();

  DataBucket& db = ::TreeMaker::GetTreeMakerPointer()->GetDataBucket();
  db.GetDoubleMap()["target_offset_x"] = fTarShift.x();
  db.GetDoubleMap()["target_offset_y"] = fTarShift.y();
  db.GetDoubleMap()["target_offset_z"] = fTarShift.z();

  db.GetDoubleMap()["target_rot1_offset (deg)"] = fRot1Shift/CLHEP::deg;
  db.GetDoubleMap()["target_rot2_offset (deg)"] = fRot2Shift/CLHEP::deg;

  db.GetDoubleMap()["global_assembly_shift_x"] = fWholeTransl.x();
  db.GetDoubleMap()["global_assembly_shift_y"] = fWholeTransl.y();
  db.GetDoubleMap()["global_assembly_shift_z"] = fWholeTransl.z();


}

FragSimDetConstruction::~FragSimDetConstruction()
{
  G4cout << "Instance of FragSimDetConstruction destructed" << G4endl;
}

void
FragSimDetConstruction::DefineMaterials()
{
  G4String name;
  G4String symbol;
  G4double n, a, z, density, abundance;
  G4int    ncomponents;
 
  G4NistManager* man = G4NistManager::Instance();
  
  vacuum   = man->FindOrBuildMaterial("G4_Galactic");
  aluminum = man->FindOrBuildMaterial("G4_Al");
  
  // define an Element from isotopes, by relative abundance 
  U235 = new G4Isotope(name="U235", z=92, n=235, a=235.01*g/mole);
  U238 = new G4Isotope(name="U238", z=92, n=238, a=238.03*g/mole);
  
  natU  = new G4Element(name="natural Uranium", symbol="U", ncomponents=2);
  natU->AddIsotope(U235, abundance=  0.73*perCent);
  natU->AddIsotope(U238, abundance= 99.27*perCent);
  natUMaterial = new G4Material(name="natural Uranium",
			  density = 19.1*g/cm3,
			  1);
  natUMaterial->AddElement(natU,abundance=100.*perCent);
  
  elCr = new G4Element("elCr", "Cr", 24, 51.996*g/mole);
  elFe = new G4Element("elFe", "Fe", 26, 55.85*g/mole);
  stSteel = new G4Material("Stainless Steel", 8.0*g/cm3, 2);
  stSteel->AddElement(elCr, 0.11);
  stSteel->AddElement(elFe, 0.89);

}

G4Transform3D*
FragSimDetConstruction::GetTargetTransform(void) const
{
  return tr_tar;
}

void
FragSimDetConstruction::SetupTransforms(void)
{

  G4double detUpStreamTop_x =  0.00*cm;
  G4double detUpStreamTop_y =  3.45*cm;
  G4double detUpStreamTop_z =  -0.11*cm - 8.56*cm;
  G4ThreeVector detUpStreamTopPos(detUpStreamTop_x,
  			    detUpStreamTop_y,
  			    detUpStreamTop_z);
  G4RotationMatrix detUpStreamTopRotMatrix;
  detUpStreamTopRotMatrix.rotateZ(180.0*deg);
  detUpStreamTopRotMatrix.rotateY(  0.0*deg);
  detUpStreamTopRotMatrix.rotateX( -22.5*deg);
  
  tr0 = new G4Transform3D(detUpStreamTopRotMatrix,detUpStreamTopPos);

  G4double detDownStreamLeft_x =  3.45*cm;
  G4double detDownStreamLeft_y =  0.00*cm;
  G4double detDownStreamLeft_z =  0.00*cm;

  G4ThreeVector detDownStreamLeftPos(detDownStreamLeft_x,
  			       detDownStreamLeft_y,
  			       detDownStreamLeft_z);
  G4RotationMatrix detDownStreamLeftRotMatrix;
  detDownStreamLeftRotMatrix.rotateZ( 90.0*deg);
  detDownStreamLeftRotMatrix.rotateX(180.0*deg);
  detDownStreamLeftRotMatrix.rotateY(-22.5*deg);
  
  tr1 = new G4Transform3D(detDownStreamLeftRotMatrix,detDownStreamLeftPos);

  G4double detUpStreamRight_x = -3.45*cm;
  G4double detUpStreamRight_y =  0.00*cm;
  G4double detUpStreamRight_z =  -0.11*cm - 8.56*cm;
  G4ThreeVector detUpStreamRightPos(detUpStreamRight_x,
  			      detUpStreamRight_y,
  			      detUpStreamRight_z);
  G4RotationMatrix detUpStreamRightRotMatrix;
  detUpStreamRightRotMatrix.rotateZ(270.0*deg);
  // detUpStreamRightRotMatrix.rotateX(180.0*deg);
  detUpStreamRightRotMatrix.rotateX(  0.0*deg);
  detUpStreamRightRotMatrix.rotateY(-22.5*deg);
  
  tr2 = new G4Transform3D(detUpStreamRightRotMatrix,detUpStreamRightPos);

  G4double detDownStreamBottom_x =  0.00*cm;
  G4double detDownStreamBottom_y = -3.45*cm;
  G4double detDownStreamBottom_z =  0.00*cm;
  G4ThreeVector detDownStreamBottomPos(detDownStreamBottom_x,
  			         detDownStreamBottom_y,
  			         detDownStreamBottom_z);
  G4RotationMatrix detDownStreamBottomRotMatrix;
  detDownStreamBottomRotMatrix.rotateY(180.0*deg);
  detDownStreamBottomRotMatrix.rotateX(-22.5*deg);

  
  tr3 = new G4Transform3D(detDownStreamBottomRotMatrix,detDownStreamBottomPos);

}


G4AssemblyVolume* FragSimDetConstruction::BuildTargetAssembly()
{
    G4AssemblyVolume * target_assembly = new G4AssemblyVolume;

    // Target
    G4double target_x          =  -0.44*cm;
    G4double target_y          =  -0.01*cm;
    G4double target_z          =  -1.67*cm;

    G4ThreeVector targetPos(target_x, target_y, target_z);
    targetPos += fTarShift;
//    fRot1Shift = 0*deg;
//    fRot2Shift = 0*deg;

    rotMatrix = new G4RotationMatrix;
    rotMatrix->rotateX(90.0*deg);
    rotMatrix->rotateZ(45.0*deg + fRot2Shift);
    rotMatrix->rotateY(-45.0*deg);// + fRot1Shift);

    fMod45_tar_rot = *rotMatrix;
    fMod45_tar_rot.rotateY(45.0*deg);
    fMod45_tar_rot.rotateZ(-fRot2Shift);
    fMod45_tar_rot.rotateY(-45.0*deg);


    DataBucket& db = ::TreeMaker::GetTreeMakerPointer()->GetDataBucket();
    db.GetDoubleMap()["target_rot1_offset (deg)"] = fRot1Shift/CLHEP::deg;
    db.GetDoubleMap()["target_rot2_offset (deg)"] = fRot2Shift/CLHEP::deg;

    tr_tar = new G4Transform3D(*rotMatrix, targetPos);

    // Build 238U Target
    G4double target_thickness  =  tar.GetThickness(); //0.10*cm;
    G4double target_length     =  tar.GetLength();    //4.83*cm;
    G4double target_width      =  tar.GetWidth();   //2.30*cm;

    G4Box* target_box = new G4Box("target_box",
                            target_length/2.,
                            target_width/2.,
                            target_thickness/2.);

    target_log = new G4LogicalVolume(target_box,
                                     natUMaterial,
                                     "target_log");
    G4VisAttributes target_attr(true);
    target_attr.SetColor(0./255., 0./255., 255./255.);
    target_log->SetVisAttributes(target_attr);

//    target_phys = new G4PVPlacement(rotMatrix, tr_tar->getTranslation(),target_log,
 //                             "target",experimentalHall_log,
 //                             false,0,false);

    target_assembly->AddPlacedVolume(target_log, *tr_tar);


    G4ThreeVector mount_transl;
    G4ThreeVector target_ins_bot_trans;
    G4ThreeVector target_ins_top_trans;

    ComputeOffsets(mount_transl, target_ins_bot_trans, target_ins_top_trans);

    //  G4ThreeVector mount_transl (-1.1825*cm, -1.74302*cm, -3.231*cm);
    G4RotationMatrix mount_rot;
    mount_rot.rotateY(-90.0*deg);// + fRot1Shift);
    G4Transform3D *tr_mount = new G4Transform3D(mount_rot, mount_transl);
    TargetMountBuilder tmb;

    target_assembly->AddPlacedAssembly(tmb.GetAssembly(), *tr_mount);

    // sheet metal inserts used to clamp the target

    //  G4ThreeVector target_ins_bot_trans (-16.1128,11.2735,-30.6587);
    G4Box* target_ins_bot_sol = new G4Box("target_ins_bot_sol",
                                          1.27*cm,
                                          1.27*cm,
                                          0.0635*cm);

    target_ins_bot_lv = new G4LogicalVolume(target_ins_bot_sol,
                                            stSteel,
                                            "TargetInsertBottom_LV");

    G4VisAttributes target_ins_attr(true);
    target_ins_attr.SetColor(255.0/255., 100.0/255., 255./255.);
    target_ins_bot_lv->SetVisAttributes(target_ins_attr);

    target_assembly->AddPlacedVolume(target_ins_bot_lv, target_ins_bot_trans, &fMod45_tar_rot); //rotMatrix);


    G4Box* target_ins_top_sol = new G4Box("target_ins_top_sol",1.27*cm, 1.27*cm, 0.0635*cm);
    target_ins_top_lv = new G4LogicalVolume(target_ins_top_sol,
                                            stSteel,
                                            "TargetInsertTop_LV");

    // The following translation was computed by the compute_target_positions program

    target_ins_top_lv->SetVisAttributes(target_ins_attr);
    target_assembly->AddPlacedVolume(target_ins_top_lv, target_ins_top_trans, rotMatrix);

    G4Box* al_spacer_sol = new G4Box("al_spacer_sol",
                                     1.27*cm,
                                     1.27*cm,
                                     0.5*0.25*2.54*cm);

    G4LogicalVolume *al_spacer_lv = new G4LogicalVolume(al_spacer_sol,
                                                        aluminum,
                                                        "al_spacer_lv");

    // The following measurements are calculated in the file
    // 45DegTargetHolderGeoPoints.ods
    G4ThreeVector center_mount_back(0.44958*cm, 0.635*cm, 2.73169*cm);
    G4ThreeVector al_spacer_transl = center_mount_back;
    al_spacer_transl += G4ThreeVector(0, 0, al_spacer_sol->GetZHalfLength());

    G4RotationMatrix rot_al_spacer = mount_rot;
    al_spacer_transl *= rot_al_spacer;
    al_spacer_transl += mount_transl;

    G4Transform3D al_spacer_tr(rot_al_spacer, al_spacer_transl);

//    G4PVPlacement* al_spacer_pv = new G4PVPlacement(rot_al_spacer,
//                                          al_spacer_transl,
//                                          al_spacer_lv,
//                                          "al_spacer",
//                                          experimentalHall_log,
//                                         false, 0, true);
    G4VisAttributes al_spacer_attr(true);
    al_spacer_attr.SetColor(255.0/255., 100.0/255., 70.0/255.);
    al_spacer_lv->SetVisAttributes(al_spacer_attr);

    target_assembly->AddPlacedVolume(al_spacer_lv, al_spacer_tr);


    fRot_center_transl = center_mount_back;
    fRot_center_transl += G4ThreeVector(0, 0, 2.0*al_spacer_sol->GetZHalfLength());
    fRot_center_transl += G4ThreeVector(0.25*2.54*cm, 0, 0.5*2.54*cm);
    fRot_center_transl *= mount_rot;
    fRot_center_transl += mount_transl;
    //fRot_center_transl += fTarShift;

    return target_assembly;

}

void FragSimDetConstruction::ComputeOffsets(G4ThreeVector &mount, G4ThreeVector &bot, G4ThreeVector &top)
{

    G4ThreeVector p0n( tar.GetLength()/2.0, tar.GetWidth()/2.0, -tar.GetThickness()/2.0);
    G4ThreeVector p1n( tar.GetLength()/2.0,-tar.GetWidth()/2.0, -tar.GetThickness()/2.0);
    G4ThreeVector p2n(-tar.GetLength()/2.0,-tar.GetWidth()/2.0, -tar.GetThickness()/2.0);
    G4ThreeVector p3n(-tar.GetLength()/2.0, tar.GetWidth()/2.0, -tar.GetThickness()/2.0);

    G4ThreeVector p0p( tar.GetLength()/2.0, tar.GetWidth()/2.0, tar.GetThickness()/2.0);
    G4ThreeVector p1p( tar.GetLength()/2.0,-tar.GetWidth()/2.0, tar.GetThickness()/2.0);
    G4ThreeVector p2p(-tar.GetLength()/2.0,-tar.GetWidth()/2.0, tar.GetThickness()/2.0);
    G4ThreeVector p3p(-tar.GetLength()/2.0, tar.GetWidth()/2.0, tar.GetThickness()/2.0);

    //    rMatrix.print(G4cout);

    G4RotationMatrix rrotMatrix = *rotMatrix;//->inverse();
    rrotMatrix.print(G4cout);
    G4cout << "phi=" << rrotMatrix.getPhi()/deg << G4endl;
    G4cout << "theta=" << rrotMatrix.getTheta()/deg << G4endl;
    G4cout << "psi=" << rrotMatrix.getPsi()/deg << G4endl;

    G4ThreeVector p0np = rrotMatrix*p0n;
    G4ThreeVector p1np = rrotMatrix*p1n;
    G4ThreeVector p2np = rrotMatrix*p2n;
    G4ThreeVector p3np = rrotMatrix*p3n;

    G4ThreeVector p0pp = rrotMatrix*p0p;
    G4ThreeVector p1pp = rrotMatrix*p1p;
    G4ThreeVector p2pp = rrotMatrix*p2p;
    G4ThreeVector p3pp = rrotMatrix*p3p;

    G4cout << "p0n: " << p0n << " --> " << p0np << G4endl;
    G4cout << "p1n: " << p1n << " --> " << p1np << G4endl;
    G4cout << "p2n: " << p2n << " --> " << p2np << G4endl;
    G4cout << "p3n: " << p3n << " --> " << p3np << G4endl;

    G4cout << '\n' << G4endl;

    G4cout << "p0p: " << p0p << " --> " << p0pp << G4endl;
    G4cout << "p1p: " << p1p << " --> " << p1pp << G4endl;
    G4cout << "p2p: " << p2p << " --> " << p2pp << G4endl;
    G4cout << "p3p: " << p3p << " --> " << p3pp << G4endl;

    G4cout << "\nCenter of the bottom of the target" << G4endl;
    G4ThreeVector tar_bottom_center = 0.5*(p2pp+p3pp);
    G4cout << tar_bottom_center << G4endl;

    // to compute the offsets
    // we know that the target is shifted 1.38 cm in the negative x direction
    // of the rotated coordinate system. This was computed in the Notes/TargetOffsetCalcs.ods
    // file through the measurement of images of the target in the mount.

    G4double tar_offset = 1.38*cm;
    G4ThreeVector xp = -1.0*tar_offset*fMod45_tar_rot.colX();
//    G4ThreeVector xp = -1.0*tar_offset*rrotMatrix.colX();

    G4cout << "The target is actually offset from the base"
            << "\nof the mount slit. The base of the slit then should be"
            << "\ndisplaced by the following vector" << G4endl;
    G4cout << xp << " (mm)" << G4endl;

    G4cout << "\nThe displacement of the target from the is defined in the FragSimDetConstruction class."
            << "\nThe mount is positioned relative to the position of the target. An absolute"
            << "\nposition is acquired by adding the recently computed offset to the bottom center"
            << "\nof the target." << G4endl;
    G4cout << "\nCenter of mount slit = " << tar_bottom_center+xp << " (mm)" << G4endl;

    G4double tar_overhang = 0.25*cm;     //<-- Calculated in TargetOffsetCalcs.ods
                                         // this is the average value of two methods
    G4double mount_slit_width = 2.54*cm;
    G4cout << "\nThe target overhangs the edge of the mount as well. To account for this overhang"
            << "\nthe mount and sh metal plates are shifted in the +y' direction of the rotated coord"
            << "\nsystem."
            << G4endl;
    G4double yp_shift = (0.5*(mount_slit_width - tar.GetWidth()) + tar_overhang);
    // this shouldn't matter whether it is the fMod45_tar_rot or the rrotMatrix
    G4ThreeVector yp = yp_shift*rrotMatrix.colY();
    G4cout << "\nOverhang   = " << tar_overhang/cm << " cm";
    G4cout << "\nShift in yp= " << yp_shift/cm << " cm" << G4endl;

    G4cout << "\nAlso we know that the target is offset by the thickness of the sheet metal"
            << "\nin the new z direction. Therefore, there must be a shift downwards"
            << "\nin the mount along the new z-direction by 0.127 cm."
            << G4endl;

    G4double sh_thickness = 0.127*cm;
    G4ThreeVector zp = sh_thickness*fMod45_tar_rot.colZ();
//    G4ThreeVector zp = sh_thickness*rrotMatrix.colZ();

    // The following measurements were computed using the 232Th_Inventor_Model_measurements.ods file
    // in the Ubuntu\ One/AnalysisLog folder.
//    G4double target_x          =  -0.44*cm;
//    G4double target_y          =  -0.01*cm;
//    G4double target_z          =  -1.67*cm;

//    G4ThreeVector mnt_shift = tar_bottom_center + xp + yp + zp;



    G4ThreeVector targetPos = tr_tar->getTranslation();//(target_x, target_y, target_z);
    G4ThreeVector mount_transl = targetPos + tar_bottom_center + xp + yp + zp;
    G4cout << "\n\nThe target offset was " << targetPos << " (mm)" << G4endl;
    G4cout << "\n\nSlit center should then be at the following location: "
            << mount_transl << " (mm)" << G4endl;
//    mount_transl += fTarShift;
    mount = mount_transl;

    G4cout << "\n\nThe sheet metal inserts used to clamp the target in "
            <<  "\nplace are positioned with the same rotation matrix as "
            <<  "\nthe target but with the following translations"
            << G4endl;

    G4double sh_length = 2.54*cm;
    // The way to compute this is by determining the offset from the center of the
    // target to align the bottom of the insert with the bottom of the target.
    //  These are only aligned in for the 238U data.

    // Prior to rotation and translation, the centers of both the target and the
    // sheet metal will be aligned. The difference can then be computed as :
    G4double sh_shift = 0.5*(tar.GetLength()-sh_length);

    ///////////////////////////////////////////////////////////////////////////////
    // ----------------------------------------------------------------------------
    // Compute shift for the top plate
    //
    // The shift will be in the rotated x direction
    G4ThreeVector sh_bot_shift_xp = -1.0*(sh_shift + tar_offset) * rrotMatrix.colX();
    G4double bsheet_shift_offset = 0*cm;
    G4double bsheet_shift = bsheet_shift_offset;
    sh_bot_shift_xp += bsheet_shift*rrotMatrix.colX();
    sh_bot_shift_xp += 0.47*cm*fMod45_tar_rot.colX();

    // the next is the actual offset of the sheet metal plate from the bottom of the slit
    // the calculation is found in TargetOffSetCalcs.ods
    G4ThreeVector sh_bot_shift_yp = -0.5*yp; // <-- fxd + fxd2
                                     //0 * yp; // <-- fxd3
                                     //-0.85*yp;//0.0*cm*rrotMatrix.colY();

    G4ThreeVector sh_bot_shift_zp = 0.5*tar.GetThickness()*rrotMatrix.colZ();
    sh_bot_shift_zp += 0.5*sh_thickness*fMod45_tar_rot.colZ();

    G4ThreeVector sh_bot_shift = sh_bot_shift_xp + sh_bot_shift_yp + sh_bot_shift_zp;
    sh_bot_shift += targetPos;
    bot = sh_bot_shift;
    ///////////////////////////////////////////////////////////////////////////////
    // ----------------------------------------------------------------------------
    // Compute shift for the bottom plate
    //
    G4ThreeVector sh_top_shift_xp = -1.0*(sh_shift+tar_offset)*rrotMatrix.colX();
    // the next is the actual offset of the sheet metal plate from the bottom of the slit
    // the calculation is found in TargetOffSetCalcs.ods
    sh_top_shift_xp += 0.3*cm*rrotMatrix.colX();

    G4ThreeVector sh_top_shift_yp = 0.0*cm * rrotMatrix.colY();  //    G4ThreeVector  sh_top_shift_yp = 0.5*yp;
    G4ThreeVector sh_top_shift_zp = -0.5*(tar.GetThickness()+sh_thickness)*rrotMatrix.colZ();
    G4ThreeVector sh_top_shift = sh_top_shift_xp + sh_top_shift_yp + sh_top_shift_zp;
//    sh_top_shift += fTarShift;
    sh_top_shift += targetPos;
    top = sh_top_shift;
    /////////////////////////////////////////////////////////////////////////////////
    // ------------------------------------------------------------------------------
    // Print results
    //
    G4cout << "\nRotation (rot1): " << fRot1Shift/deg << " degrees"
            << "\nRotation (rot2): " << fRot2Shift/deg << " degrees"
            << "\nBot. sh. shift): " << bsheet_shift_offset/cm << " (cm)"
            << "\n--------------------------------------------" << G4endl;
    G4cout << "\nmount_transl   : " << mount_transl << " (mm)" << G4endl;
    G4cout << "\nins_bot_transl : " << sh_bot_shift << " (mm)" << G4endl;
    G4cout << "\nins_top_transl : " << sh_top_shift << " (mm)" << G4endl;

}

void FragSimDetConstruction::SetTargetShift(const G4ThreeVector &transl)
{
    fTarShift = transl;

    DataBucket& db = ::TreeMaker::GetTreeMakerPointer()->GetDataBucket();
    db.GetDoubleMap()["target_offset_x"] = fTarShift.x();
    db.GetDoubleMap()["target_offset_y"] = fTarShift.y();
    db.GetDoubleMap()["target_offset_z"] = fTarShift.z();


}

void FragSimDetConstruction::SetShiftOfWholeAssembly(const G4ThreeVector &transl)
{
    fWholeTransl = transl;

    DataBucket& db = ::TreeMaker::GetTreeMakerPointer()->GetDataBucket();
    db.GetDoubleMap()["global_assembly_shift_x"] = fWholeTransl.x();
    db.GetDoubleMap()["global_assembly_shift_y"] = fWholeTransl.y();
    db.GetDoubleMap()["global_assembly_shift_z"] = fWholeTransl.z();


}



void FragSimDetConstruction::SetRot1Shift(const G4double& rot)
{
    fRot1Shift = rot;

    DataBucket& db = ::TreeMaker::GetTreeMakerPointer()->GetDataBucket();
    db.GetDoubleMap()["target_rot1_offset (deg)"] = fRot1Shift/CLHEP::deg;
}

void FragSimDetConstruction::SetRot2Shift(const G4double& rot)
{
    fRot2Shift = rot;

    DataBucket& db = ::TreeMaker::GetTreeMakerPointer()->GetDataBucket();
    db.GetDoubleMap()["target_rot2_offset (deg)"] = fRot2Shift/CLHEP::deg;
}


G4VPhysicalVolume* 
FragSimDetConstruction::Construct()
{

    SetupTransforms();
  //------------------------------------------------------ volumes

  //------------------------------ experimental hall (world volume)
  //------------------------------ beam line along x axis
  G4double expHall_x = 0.5*m;
  G4double expHall_y = 0.5*m;
  G4double expHall_z = 0.5*m;
  G4Box* experimentalHall_box = new G4Box("expHall_box",
				  expHall_x,
				  expHall_y,
				  expHall_z);
  
  experimentalHall_log = new G4LogicalVolume(experimentalHall_box,
                                             vacuum,"expHall_log");
  experimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),
				    experimentalHall_log,"expHall",
				    0,false,0);
  experimentalHall_log->SetVisAttributes(new G4VisAttributes(false));

  // // Build aluminum chamber wall
  // G4double innerChamberRadius = 17.8*cm;
  // G4double outerChamberRadius = 22.9*cm;
  // G4double chamberLength      = 35.6*cm;
  
  // G4Tubs* chamberWall_tub = new G4Tubs("chamberWall_tub",
  // 			         innerChamberRadius,
  // 			         outerChamberRadius,
  // 			         chamberLength,
  // 			         0,
  // 			         2*pi);
  // chamberWall_log = new G4LogicalVolume(chamberWall_tub,
  // 				aluminum,"chamberWall_log");
  // chamberWall_phys = new G4PVPlacement(0,G4ThreeVector(),
  // 			         experimentalHall_log,"chamberWall",
  // 			         0,false,0);

  
  //------------------------------------------------------------------
  
  //-----Sensitive Detectors:
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
      
  SDman->AddNewDetector(SSD0.GetSensitivity());
  SDman->AddNewDetector(SSD1.GetSensitivity());
  SDman->AddNewDetector(SSD2.GetSensitivity());
  SDman->AddNewDetector(SSD3.GetSensitivity());

  G4AssemblyVolume *whole_assembly = new G4AssemblyVolume;
  whole_assembly->AddPlacedAssembly(SSD0.GetADetector(),*tr0);
  whole_assembly->AddPlacedAssembly(SSD1.GetADetector(),*tr1);
  whole_assembly->AddPlacedAssembly(SSD2.GetADetector(),*tr2);
  whole_assembly->AddPlacedAssembly(SSD3.GetADetector(),*tr3);

//  SSD0.GetADetector()->MakeImprint(experimentalHall_log, *tr0);
//  SSD1.GetADetector()->MakeImprint(experimentalHall_log, *tr1);
//  SSD2.GetADetector()->MakeImprint(experimentalHall_log, *tr2);
//  SSD3.GetADetector()->MakeImprint(experimentalHall_log, *tr3);



  fTarget_assembly = BuildTargetAssembly();

  TargetSensitivity *tar_sens = new TargetSensitivity("target",4);
  SDman->AddNewDetector(tar_sens);
  target_log->SetSensitiveDetector(tar_sens);

//  G4RotationMatrix no_rot;
//  G4ThreeVector no_transl;
  G4ThreeVector tar_assembly_disp = -1.0*fRot_center_transl;

  G4AssemblyVolume* temp_rot_assembly = new G4AssemblyVolume();
  temp_rot_assembly->AddPlacedAssembly(fTarget_assembly,tar_assembly_disp,0);

  G4RotationMatrix tar_assembly_rot;
  tar_assembly_rot.rotateY(fRot1Shift);
  G4Transform3D *tar_trans = new G4Transform3D(tar_assembly_rot, -1.0*tar_assembly_disp);

  whole_assembly->AddPlacedAssembly(temp_rot_assembly,*tar_trans);
//  temp_rot_assembly->MakeImprint(experimentalHall_log, *tar_trans);


  G4RotationMatrix whole_rot;
  G4Transform3D *whole_trans = new G4Transform3D(whole_rot,fWholeTransl);

  whole_assembly->MakeImprint(experimentalHall_log, *whole_trans);


  return experimentalHall_phys;
}

