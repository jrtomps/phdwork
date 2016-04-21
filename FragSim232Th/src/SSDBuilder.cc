// SSDBuilder.cc
//
// Jeromy Tompkins
// 8/17/2011
//
//

#include <iostream>
#include <iomanip>
#include <map>
#include "globals.hh"
#include "G4String.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Trap.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "SSDBuilder.hh"

const G4double SSDBuilder::flongWidth   =  11.59*cm;
const G4double SSDBuilder::fshortWidth  =   4.40*cm;
const G4double SSDBuilder::fthickness   =   0.24*cm;
const G4double SSDBuilder::flength      =   9.27*cm;
const G4double SSDBuilder::fobtuseAngle = 110.85*deg;
const G4double SSDBuilder::facuteAngle  =  69.15*deg;

const G4double SSDBuilder::fdielectricBoardOffset_X = -0.704*cm;//-0.43*cm;
const G4double SSDBuilder::fdielectricBoardOffset_Y =  0.00*cm;
const G4double SSDBuilder::fdielectricBoardOffset_Z =  0.00*cm;

const G4double SSDBuilder::fcutoutLength      = 5.0*cm;
const G4double SSDBuilder::fcutoutLongWidth   = 5.0*cm;
const G4double SSDBuilder::fcutoutShortWidth  = 3.0*cm;

const G4double SSDBuilder::fcutoutOffset_X    = -1.41*cm;
const G4double SSDBuilder::fcutoutOffset_Y    =  0.43*cm;
const G4double SSDBuilder::fcutoutOffset_Z    =  0.00*cm;

const G4double SSDBuilder::fsiliconThickness     = 0.03*cm;
const G4double SSDBuilder::fsiliconLongWidth     = 8.26*cm;
const G4double SSDBuilder::fsiliconShortWidth_0  = 3.37*cm;
const G4double SSDBuilder::fsiliconShortWidth_1  = 4.60*cm;
const G4double SSDBuilder::fsiliconLength_0      = 6.37*cm;
const G4double SSDBuilder::fsiliconLength_1      = 2.18*cm;
const G4double SSDBuilder::fbottomSiliconOffset  = 0.659*cm;

const G4double SSDBuilder::fsiliconOffset_X = 0.00*cm;
const G4double SSDBuilder::fsiliconOffset_Y = fthickness/2. + fsiliconThickness/2.;
const G4double SSDBuilder::fsiliconOffset_Z = 0.5*(fsiliconLength_0-flength)+fbottomSiliconOffset;

const G4ThreeVector SSDBuilder::strip_arc_center(0,
                                                 0,
                                                 -0.5*fsiliconLength_0-fbottomSiliconOffset-3.51*cm+fsiliconOffset_Z);//-9.367*cm);//-7.137*cm);
//const G4ThreeVector SSDBuilder::strip_arc_center(0,0,-6.324*cm);//-9.367*cm);//-7.137*cm);
const G4double SSDBuilder::radius_first_strip_edge = 5.387*cm;
const G4double SSDBuilder::strip_width             = 0.502*cm;//0.477*cm;

SSDBuilder::SSDBuilder() 
    :
      fsiOffset(fsiliconOffset_X, fsiliconOffset_Y, fsiliconOffset_Z),
      fDielectricOffset(fdielectricBoardOffset_X,
          fdielectricBoardOffset_Y,
          fdielectricBoardOffset_Z),
      fmaterials(),
      fsensitivity(NULL),
      fsilicon_log(NULL),
      fdielectricBoard_log(NULL),
      ssd_assembly(NULL),
      fsilicon_solid(NULL),
      fdielectricBoard_solid(NULL),
      fsilicon_phys(NULL),
      fdielectricBoard_phys(NULL)
{
}

SSDBuilder::SSDBuilder(SSDBuilder const& obj)
    :
      fsiOffset(fsiliconOffset_X, fsiliconOffset_Y, fsiliconOffset_Z),
      fmaterials(obj.fmaterials),
      fsensitivity(obj.fsensitivity),
      fsilicon_log(obj.fsilicon_log),
      fdielectricBoard_log(obj.fdielectricBoard_log),
      ssd_assembly(obj.ssd_assembly),
      fsilicon_solid(obj.fsilicon_solid),
      fdielectricBoard_solid(obj.fdielectricBoard_solid),
      fsilicon_phys(obj.fsilicon_phys),
      fdielectricBoard_phys(obj.fdielectricBoard_phys)
{}

SSDBuilder::~SSDBuilder()
{}

SSDBuilder& 
SSDBuilder::operator=(SSDBuilder const& obj)
{
    if (this!=&obj)
    {
        fsiOffset              = obj.fsiOffset;
        fmaterials             = obj.fmaterials;
        fsensitivity           = obj.fsensitivity;
        fsilicon_log           = obj.fsilicon_log;
        fsilicon_solid         = obj.fsilicon_solid;
        fdielectricBoard_log   = obj.fdielectricBoard_log;
        fdielectricBoard_solid = obj.fdielectricBoard_solid;
        ssd_assembly           = obj.ssd_assembly;
        fsilicon_phys          = obj.fsilicon_phys;
        fdielectricBoard_phys  = obj.fdielectricBoard_phys;
    }
    return *this;
}

void 
SSDBuilder::SetSensitivity(G4VSensitiveDetector *sens)
{
    fsensitivity = sens;
    if (fsilicon_log!=NULL)
        fsilicon_log->SetSensitiveDetector(fsensitivity);
}

G4AssemblyVolume* 
SSDBuilder::GetADetector()
{
    if (ssd_assembly==NULL)
        Assemble();

    return ssd_assembly;
}

void 
SSDBuilder::Assemble ()
{
    DefineMaterials();
    BuildSilicon();
    BuildDielectricBoard();

    ssd_assembly = new G4AssemblyVolume;

    // G4ThreeVector dielectricBoard_offset(fdielectricBoardOffset_X,
    // 			         fdielectricBoardOffset_Y,
    // 			         fdielectricBoardOffset_Z);
    G4RotationMatrix rot;
    ssd_assembly->AddPlacedVolume(fdielectricBoard_log, 
                                    fDielectricOffset, 
                                    &rot);
    ssd_assembly->AddPlacedVolume(fsilicon_log, 
                                    fsiOffset, &rot);
}

void
SSDBuilder::DefineMaterials(void)
{

    G4String name;
    G4Material* mat;

    G4NistManager *man = G4NistManager::Instance();

    mat = man->FindOrBuildMaterial("G4_Si");
    fmaterials[name="Si"] = mat;

    mat = man->FindOrBuildMaterial("G4_PLEXIGLASS");
    fmaterials[name="Plexi"] = mat;

    mat = man->FindOrBuildMaterial("G4_Galactic");
    fmaterials[name="Vacuum"] = mat;

}

void
SSDBuilder::BuildSilicon (void)
{
    G4VisAttributes attr(true);

    G4Trap *solid_0 = new G4Trap("solid_0",
                                 fsiliconLength_0/2.,
                                 0, 0,
                                 fsiliconThickness/2.,
                                 fsiliconShortWidth_0/2.,
                                 fsiliconShortWidth_0/2.,
                                 0,
                                 fsiliconThickness/2.,
                                 fsiliconLongWidth/2.,
                                 fsiliconLongWidth/2.,
                                 0);

    G4Trap *solid_1 = new G4Trap("solid_1",
                                 fsiliconLength_1/2.,
                                 0, 0,
                                 fsiliconThickness/2.,
                                 fsiliconLongWidth/2.,
                                 fsiliconLongWidth/2.,
                                 0,
                                 fsiliconThickness/2.,
                                 fsiliconShortWidth_1/2.,
                                 fsiliconShortWidth_1/2.,
                                 0);

    G4RotationMatrix rot;
    G4ThreeVector v(0.,0.,fsiliconLength_0/2.+fsiliconLength_1/2.);
    G4UnionSolid *fsilicon_solid = new G4UnionSolid("silicon_solid",
                                                    solid_0,
                                                    solid_1,
                                                    &rot,
                                                    v);
    //  fsilicon_solid->DumpInfo();


    fsilicon_log   = new G4LogicalVolume(fsilicon_solid,
                                        fmaterials["Si"],
                                        "silicon_log");
    attr.SetColor(192.0/256.0, 192.0/256.0, 192.0/256.0);   // silver
    //  attr.SetForceSolid(true);

    fsilicon_log->SetVisAttributes(attr);
    fsilicon_log->SetSensitiveDetector(fsensitivity);
}

void 
SSDBuilder::BuildDielectricBoard (void)
{
    G4VisAttributes attr(true);
    attr.SetColor(0,1,0);   // green
    //  attr.SetForceSolid(true);

    G4Trap *wholeboard = new G4Trap("whole_board",
                                    flength/2.,
                                    0, 0,
                                    fthickness/2.,
                                    fshortWidth/2.,
                                    fshortWidth/2.,
                                    0,
                                    fthickness/2.,
                                    flongWidth/2.,
                                    flongWidth/2.,
                                    0);

    // G4Trap *cutout     = new G4Trap("cut_out",
    // 			    fcutoutLength/2.,
    // 			    0, 0,
    // 			    fthickness/2.,
    // 			    fcutoutShortWidth/2.,
    // 			    fcutoutShortWidth/2.,
    // 			    0,
    // 			    fthickness/2.,
    // 			    fcutoutLongWidth/2.,
    // 			    fcutoutLongWidth/2.,
    // 			    0);


    // G4ThreeVector transl(fcutoutOffset_Y,0,fcutoutOffset_X);


    // fdielectricBoard_solid = new G4SubtractionSolid("dielectricBoard_solid",
    // 					wholeboard,
    // 					cutout,
    // 					0,
    // 					transl);

    //  fdielectricBoard_log = new G4LogicalVolume(fdielectricBoard_solid,
    fdielectricBoard_log = new G4LogicalVolume(wholeboard,
                                               fmaterials["Plexi"],
                                               "dielectricBoard_log");
    fdielectricBoard_log->SetVisAttributes(attr);
}

void
SSDBuilder::PrintGeometryInfo(void)
{
    using std::setw;
    using std::right;
    using std::setprecision;

    std::cout << right << setw(6) << "Strip"
              << right << setw(12) << "Low Bound"
              << right << setw(12) << "Up Bound"
              << std::endl;
    std::cout << setw(30) << std::setfill('-') << '-' << std::endl;
    std::cout << std::setfill(' ');
    for (G4int i=0; i<16; i++)
    {
        std::cout << right << setw(6) << i
                  << right << setw(12) << setprecision(3) 
                  << (radius_first_strip_edge+i*strip_width)/cm
                  << right << setw(12) << setprecision(3) 
                  << (radius_first_strip_edge+(i+1)*strip_width)/cm
                  << std::endl;
    }

}
