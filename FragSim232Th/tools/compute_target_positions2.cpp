#include <cmath>
#include <iostream>
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "Target.hh"


int main()
{
    G4double fRot1Shift = 10*deg;
    G4double fRot2Shift = 5*deg;
    G4double rot1 = fRot1Shift;
    G4double rot2 = fRot2Shift;

    G4RotationMatrix rMatrix;
    rMatrix.rotateX(90.0*deg);
    rMatrix.rotateZ(45.0*deg + fRot2Shift);
    rMatrix.rotateY(-45.0*deg + fRot1Shift);

    Target tar;

    G4ThreeVector p0n( tar.GetLength()/2.0, tar.GetWidth()/2.0, -tar.GetThickness()/2.0);
    G4ThreeVector p1n( tar.GetLength()/2.0,-tar.GetWidth()/2.0, -tar.GetThickness()/2.0);
    G4ThreeVector p2n(-tar.GetLength()/2.0,-tar.GetWidth()/2.0, -tar.GetThickness()/2.0);
    G4ThreeVector p3n(-tar.GetLength()/2.0, tar.GetWidth()/2.0, -tar.GetThickness()/2.0);

    G4ThreeVector p0p( tar.GetLength()/2.0, tar.GetWidth()/2.0, tar.GetThickness()/2.0);
    G4ThreeVector p1p( tar.GetLength()/2.0,-tar.GetWidth()/2.0, tar.GetThickness()/2.0);
    G4ThreeVector p2p(-tar.GetLength()/2.0,-tar.GetWidth()/2.0, tar.GetThickness()/2.0);
    G4ThreeVector p3p(-tar.GetLength()/2.0, tar.GetWidth()/2.0, tar.GetThickness()/2.0);

//    G4RotationMatrix rMatrix;
//    G4RotationMatrix rM1; rM1.rotateX(90.0*deg);
//    G4RotationMatrix rM2; rM2.rotateY(45.0*deg + rot1);
//    G4RotationMatrix rM3; rM3.rotateZ(-45.0*deg + rot2);
//    rMatrix = rM1*rM3*rM2;


    //    rMatrix.print(std::cout);

    G4RotationMatrix rotMatrix = rMatrix.inverse();
    rotMatrix.print(std::cout);
    std::cout << "phi=" << rotMatrix.getPhi()/deg << std::endl;
    std::cout << "theta=" << rotMatrix.getTheta()/deg << std::endl;
    std::cout << "psi=" << rotMatrix.getPsi()/deg << std::endl;
    
    G4ThreeVector p0np = rotMatrix*p0n;
    G4ThreeVector p1np = rotMatrix*p1n;
    G4ThreeVector p2np = rotMatrix*p2n;
    G4ThreeVector p3np = rotMatrix*p3n;

    G4ThreeVector p0pp = rotMatrix*p0p;
    G4ThreeVector p1pp = rotMatrix*p1p;
    G4ThreeVector p2pp = rotMatrix*p2p;
    G4ThreeVector p3pp = rotMatrix*p3p;

    std::cout << "p0n: " << p0n << " --> " << p0np << std::endl;
    std::cout << "p1n: " << p1n << " --> " << p1np << std::endl;
    std::cout << "p2n: " << p2n << " --> " << p2np << std::endl;
    std::cout << "p3n: " << p3n << " --> " << p3np << std::endl;

    std::cout << '\n' << std::endl;

    std::cout << "p0p: " << p0p << " --> " << p0pp << std::endl;
    std::cout << "p1p: " << p1p << " --> " << p1pp << std::endl;
    std::cout << "p2p: " << p2p << " --> " << p2pp << std::endl;
    std::cout << "p3p: " << p3p << " --> " << p3pp << std::endl;

    std::cout << "\nCenter of the bottom of the target" << std::endl;
    G4ThreeVector tar_bottom_center = 0.5*(p2np+p3np);
    std::cout << tar_bottom_center << std::endl;

    // to compute the offsets
    // we know that the target is shifted 1.38 cm in the negative x direction
    // of the rotated coordinate system. This was computed in the Notes/TargetOffsetCalcs.ods
    // file through the measurement of images of the target in the mount.

    G4double tar_offset = 1.38*cm;
    G4ThreeVector xp = rotMatrix.colX();
    xp *= -1.0*tar_offset;
    std::cout << "The target is actually offset from the base"
            << "\nof the mount slit. The base of the slit then should be"
            << "\ndisplaced by the following vector" << std::endl;
    std::cout << xp << " (mm)" << std::endl;

    std::cout << "\nThe displacement of the target from the is defined in the FragSimDetConstruction class."
            << "\nThe mount is positioned relative to the position of the target. An absolute"
            << "\nposition is acquired by adding the recently computed offset to the bottom center"
            << "\nof the target." << std::endl;
    std::cout << "\nCenter of mount slit = " << tar_bottom_center+xp << " (mm)" << std::endl;

    G4double tar_overhang = 0.25*cm;     //<-- Calculated in TargetOffsetCalcs.ods
                                         // this is the average value of two methods
    G4double mount_slit_width = 2.54*cm;
    std::cout << "\nThe target overhangs the edge of the mount as well. To account for this overhang"
            << "\nthe mount and sh metal plates are shifted in the +y' direction of the rotated coord"
            << "\nsystem."
            << std::endl;
    G4double yp_shift =-1.0*(0.5*(mount_slit_width - tar.GetWidth()) + tar_overhang);
    G4ThreeVector yp = yp_shift*rotMatrix.colY();
    std::cout << "\nOverhang   = " << tar_overhang/cm << " cm";
    std::cout << "\nShift in yp= " << yp_shift/cm << " cm" << std::endl;

    std::cout << "\nAlso we know that the target is offset by the thickness of the sheet metal"
            << "\nin the new z direction. Therefore, there must be a shift downwards"
            << "\nin the mount along the new z-direction by 0.127 cm."
            << std::endl;

    G4double sh_thickness = 0.127*cm;
    G4ThreeVector zp = rotMatrix.colZ();
    zp *= -1.0*sh_thickness;

    // The following measurements were computed using the 232Th_Inventor_Model_measurements.ods file
    // in the Ubuntu\ One/AnalysisLog folder.
    G4double target_x          =  -0.44*cm;
    G4double target_y          =  -0.01*cm;
    G4double target_z          =  -1.67*cm;

    G4ThreeVector targetPos(target_x, target_y, target_z);
    G4ThreeVector mount_transl = targetPos + tar_bottom_center + xp + yp + zp;
    std::cout << "\n\nThe target offset was " << targetPos << " (mm)" << std::endl;
    std::cout << "\n\nSlit center should then be at the following location: "
            << mount_transl << " (mm)" << std::endl;


    std::cout << "\n\nThe sheet metal inserts used to clamp the target in "
            <<  "\nplace are positioned with the same rotation matrix as "
            <<  "\nthe target but with the following translations"
            << std::endl;

    G4double sh_length = 2.54*cm;
    // The way to compute this is by determining the offset from the center of the
    // target to align the bottom of the insert with the bottom of the target.
    //  These are only aligned in for the 238U data.

    // Prior to rotation and translation, the centers of both the target and the
    // sheet metal will be aligned. The difference can then be computed as :
    G4double sh_shift = 0.5*(tar.GetLength()-sh_length);

    ///////////////////////////////////////////////////////////////////////////////
    // ---------------------------------------------------------------------------- 
    // Compute shift for the top plat
    //
    // The shift will be in the rotated x direction
    G4ThreeVector sh_bot_shift_xp = -1.0*(sh_shift + tar_offset) * rotMatrix.colX();
    G4double bsheet_shift_offset = 0*cm;
    G4double bsheet_shift = 0.47*cm + bsheet_shift_offset;
    sh_bot_shift_xp += bsheet_shift*rotMatrix.colX();
    
    // the next is the actual offset of the sheet metal plate from the bottom of the slit
    // the calculation is found in TargetOffSetCalcs.ods
    G4ThreeVector sh_bot_shift_yp = 0.0*cm * rotMatrix.colY();//    sh_bot_shift += -0.5*yp;

    G4ThreeVector sh_bot_shift_zp = -0.5*(tar.GetThickness()+sh_thickness)*rotMatrix.colZ();

    G4ThreeVector sh_bot_shift = sh_bot_shift_xp + sh_bot_shift_yp + sh_bot_shift_zp;


    ///////////////////////////////////////////////////////////////////////////////
    // ----------------------------------------------------------------------------
    // Compute shift for the bottom plate
    //
    G4ThreeVector sh_top_shift_xp = -1.0*(sh_shift+tar_offset)*rotMatrix.colX();
    // the next is the actual offset of the sheet metal plate from the bottom of the slit
    // the calculation is found in TargetOffSetCalcs.ods
    sh_top_shift_xp += 0.3*cm*rotMatrix.colX();

	G4ThreeVector sh_top_shift_yp = 0.0*cm * rotMatrix.colY();  //    G4ThreeVector  sh_top_shift_yp = 0.5*yp;
    G4ThreeVector sh_top_shift_zp = 0.5*(tar.GetThickness()+sh_thickness)*rotMatrix.colZ();
    G4ThreeVector sh_top_shift = sh_top_shift_xp + sh_top_shift_yp + sh_top_shift_zp;
  

    /////////////////////////////////////////////////////////////////////////////////
    // ------------------------------------------------------------------------------
    // Print results
    //  
    std::cout << "\nRotation (rot1): " << rot1/deg << " degrees"
            << "\nRotation (rot2): " << rot2/deg << " degrees"
            << "\nBot. sh. shift): " << bsheet_shift_offset/cm << " (cm)"
            << "\n--------------------------------------------" << std::endl;
    std::cout << "\nmount_transl   : " << mount_transl << " (mm)" << std::endl;
    std::cout << "\nins_bot_transl : " << sh_bot_shift + targetPos << " (mm)" << std::endl;
    std::cout << "\nins_top_transl : " << sh_top_shift + targetPos << " (mm)" << std::endl;


    return 0;
}
