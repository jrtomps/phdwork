
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>
#include "/home/jrtomps/g4work/FragSim232Th/include/SSDBuilder.hh"
#include <CLHEP/Vector/ThreeVector.h>
#include <CLHEP/Vector/Rotation.h>
#include "/home/jrtomps/g4work/FragSim232Th/include/Target.hh"

using namespace std;
using namespace CLHEP;

const double in = 25.4; 
// const double cm = 1.00;

vector<Hep3Vector> vecs;
vector<Hep3Vector> vecs_c;
vector<Hep3Vector> vecs_g4;
Hep3Vector Rc;
Hep3Vector center_det, center_det_c;
Hep3Vector center_det_g4, center_det_c_g4;
Hep3Vector center_tar, center_tar_c;
Hep3Vector center_tar_g4, center_tar_c_g4;

HepRotation rot, invrot;
bool vecsAlreadySetUp = false;
bool rotAlreadySetUp  = false;

void
PrintVectorInInches(Hep3Vector vec)
{
  cout << setiosflags(ios::fixed | ios::right) << setprecision(4);
  cout << "(" << setw(8) << vec.x()/in
       << "," << setw(8) << vec.y()/in
       << "," << setw(8) << vec.z()/in << ") inches";
  cout << resetiosflags(ios::fixed | ios::right);
}

void
PrintVectorInCm(Hep3Vector vec)
{
  cout << setiosflags(ios::fixed | ios::right) << setprecision(4);
  cout << "(" << setw(8) << vec.x()/cm
       << "," << setw(8) << vec.y()/cm
       << "," << setw(8) << vec.z()/cm << ") cm";
  cout << resetiosflags(ios::fixed | ios::right);
}

void
OutputVectorInInches(std::ostream& stream, Hep3Vector& vec)
{
  stream << setiosflags(ios::fixed) << setprecision(4);
  stream << setw(10) << vec.x()/in
         << setw(10) << vec.y()/in
         << setw(10) << vec.z()/in;
}

void
PrintHeader(const string str)
{
  cout << setfill('=') << setw(30) << "=" << endl;
  cout << setfill(' ');
  cout << setiosflags(ios::internal) << setw(30) << str << endl;
  cout << resetiosflags(ios::internal);
  cout << endl;
}

void
SetUpVectors(void)
{
  // Data taken from 
  vecs.push_back(Hep3Vector(-5.224*in, -0.684*in, 6.756*in)); // 0
  vecs.push_back(Hep3Vector(-5.878*in, -0.684*in, 7.410*in)); // 1
  vecs.push_back(Hep3Vector(-6.535*in,  0.246*in, 6.753*in)); // 2
  vecs.push_back(Hep3Vector(-5.881*in,  0.246*in, 6.099*in)); // 3
  vecs.push_back(Hep3Vector(-5.616*in, -0.612*in, 4.262*in)); // 4
  vecs.push_back(Hep3Vector(-7.398*in, -0.612*in, 4.262*in)); // 5
  vecs.push_back(Hep3Vector(-8.789*in, -2.008*in, 7.634*in)); // 6
  vecs.push_back(Hep3Vector(-4.226*in, -2.008*in, 7.634*in)); // 7
  vecs.push_back(Hep3Vector(-6.892*in, -0.698*in, 4.497*in)); // 8
  vecs.push_back(Hep3Vector(-5.568*in, -0.698*in, 4.497*in)); // 9

  vecs_c = vecs;
  
  Rc = Hep3Vector(-6.210*in, 0.000*in, 0.000*in);
  for (unsigned int i=0; i<vecs_c.size(); i++)
    {
      vecs_c[i] = vecs[i]-Rc;

      cout << setw(4) << left << i;
      PrintVectorInInches(vecs_c[i]);
      cout << endl;

    }
  vecsAlreadySetUp = true;

  // compute detector center
  // Compute detector center and print to screen
  // Hep3Vector center_det, center_det_c;
  center_det.setX(0.5*(vecs[7].x() + vecs[6].x()));
  center_det.setY(0.5*(vecs[4].y() + vecs[7].y()));
  center_det.setZ(0.5*(vecs[4].z() + vecs[7].z()));
  center_det_c = center_det - Rc;

  center_det_g4   = center_det;
  center_det_c_g4 = center_det_c;

  SSDBuilder anSSDB;

  //  Hep3Vector siOffset = anSSDB.GetSiOffset();
  Hep3Vector dielOffset = anSSDB.GetDielectricOffset();

  // cout << "Si Offset " << endl;
  // PrintVectorInInches(siOffset);
  // cout << endl;
  cout << "Dielectric Offset " << endl;
  PrintVectorInInches(dielOffset);
  cout << endl;

 // Compute target center and print to screen  
  center_tar   = 0.5*(vecs[3]+vecs[1]);
  center_tar_c = center_tar - Rc;

  vecs_g4 = vecs;
  
  cout << "Geant4 converted" << endl;
  for (unsigned int i=0; i<vecs_c.size(); i++)
    {
      vecs_g4[i] = vecs_c[i]-center_det_c;
      
      //      vecs_g4[i].setX(-1.0*vecs_g4[i].x());
      vecs_g4[i].setY(-1.0*vecs_g4[i].y());
      vecs_g4[i].setZ(-1.0*vecs_g4[i].z());
      vecs_g4[i] += center_det_c;

      center_det_g4.setX(0.5*(vecs_g4[7].x() + vecs_g4[6].x()));
      center_det_g4.setY(0.5*(vecs_g4[4].y() + vecs_g4[7].y()));
      center_det_g4.setZ(0.5*(vecs_g4[4].z() + vecs_g4[7].z()));
      //      center_det_c_g4 = center_det_g4 - Rc;
      
      //      center_det_g4   -= siOffset;
      center_det_g4   -= dielOffset;   // note that this is already in G4 coords
      
      //      center_det_c_g4 -= siOffset;
      //      center_det_c_g4 -= dielOffset;

      cout << setw(4) << left << i;
      PrintVectorInInches(vecs_g4[i]);
      cout << endl;

    }

  center_tar_g4   = 0.5*(vecs_g4[3]+vecs_g4[1]);
  //  center_tar_c_g4 = center_tar_g4 - Rc;

  vecsAlreadySetUp = true;
}

void
ComputeOffset(void)
{  
  PrintHeader("\n\nBegin Offset Calc");
  
  if (! vecsAlreadySetUp)
    SetUpVectors();

 
  cout << "\nCenter of target position" << endl;
  PrintVectorInInches(center_tar_g4); cout << endl;
  // cout << "\nCenter of target position wrt beam" << endl;
  // PrintVectorInInches(center_tar_c_g4); cout << endl;

  cout << "\nCenter of detector position" << endl;
  PrintVectorInInches(center_det_g4); cout << endl;
  // cout << "\nCenter of detector position wrt beam" << endl;
  // PrintVectorInInches(center_det_c_g4); cout << endl;

  // Compute target offset from detector
  Hep3Vector tar_minus_det, tar_minus_det_c;
  tar_minus_det   = center_tar_g4   - center_det_g4;
  //  tar_minus_det_c = center_tar_c_g4 - center_det_c_g4;

  cout << "\nTarget Offset (target center - detector center)" << endl;
  PrintVectorInInches(tar_minus_det); cout << endl;
  // cout << "\nTarget Offset (target center (wrt beam) - detector center (wrt beam)" << endl;
  // PrintVectorInInches(tar_minus_det_c);
  cout << "or" << endl;
  PrintVectorInCm(tar_minus_det);
  cout << endl;

}

void
SetUpRotation(void)
{
  Hep3Vector a1(1,0,0);
  Hep3Vector a2(0,1,0);
  Hep3Vector a3(-1,0,1);
  HepRotation rM1; rM1.set(a1, 90.0*deg);
  HepRotation rM2; rM2.set(a2, 45.0*deg);
  HepRotation rM3; rM3.set(a3,-45.0*deg);
  rot = rM1*rM2*rM3;

  //  rot.print(cout);

  invrot = rot.inverse();

  rotAlreadySetUp = true;

  
}

void
ComputeTranslation()
{

  PrintHeader("\n\nBegin Translation Calc");

  if (!rotAlreadySetUp)
    SetUpRotation();
  
  // Hep3Vector a0prime = vecs_c[2] - vecs_c[1]; // long side x-direction
  // Hep3Vector a1prime = vecs_c[0] - vecs_c[1]; // short side y-direction
  // cout << "Angle between sides = " << a0prime.angle(a1prime)/deg << endl;
  Hep3Vector a0prime = vecs_g4[2] - vecs_g4[1]; // long side x-direction
  Hep3Vector a1prime = vecs_g4[0] - vecs_g4[1]; // short side y-direction
  cout << "Angle between sides = " << a0prime.angle(a1prime)/deg << endl;

  Hep3Vector norm0   = a1prime.cross(a0prime);
  norm0.setMag(1.0);

  cout << "Magnitude of a0prime = " << a0prime.r()/in << " in" << endl;
  cout << "Magnitude of a1prime = " << a1prime.r()/in << " in" << endl;

  Hep3Vector a0( a0prime.r()/2.,  a1prime.r()/2., 0.0*cm);   
  Hep3Vector a1( a0prime.r()/2., -a1prime.r()/2., 0.0*cm);   
  Hep3Vector a2(-a0prime.r()/2., -a1prime.r()/2., 0.0*cm);   
  Hep3Vector a3(-a0prime.r()/2.,  a1prime.r()/2., 0.0*cm);   

  Hep3Vector a0p = rot*a0;
  Hep3Vector a1p = rot*a1;
  Hep3Vector a2p = rot*a2;
  Hep3Vector a3p = rot*a3;

  Hep3Vector tr0 = vecs_g4[0] - a0p;
  Hep3Vector tr1 = vecs_g4[1] - a1p;
  Hep3Vector tr2 = vecs_g4[2] - a2p;
  Hep3Vector tr3 = vecs_g4[3] - a3p;

  Hep3Vector norm1  = (a1p).cross(a0p);
  norm1.setMag(1.0);

  // Hep3Vector norm2;//  = vecs_c[2].cross(vecs_c[3]);
  //  norm2.setMag(1.0);

  cout << setiosflags(ios::right);
  cout << setw(10) << " " ;              PrintVectorInInches(vecs_g4[2]); cout << endl;
  cout << setiosflags(ios::right);
  cout << setw(10) << "- ";              PrintVectorInInches(vecs_g4[1]); cout << endl;
  cout << setw(7) << "a0prime" << " = "; PrintVectorInInches(a0prime);  cout << endl;
  cout << setw(7) << "a0"      << " = "; PrintVectorInInches(a0);       cout << endl;
  cout << setw(7) << "R*a0"    << " = "; PrintVectorInInches(a0p);   cout << endl;
  cout << "tr0 = r2 - R*a0 = "; PrintVectorInInches(tr0); cout << endl;
  cout << setw(7) << "Mag" << " = " << tr0.r() << endl;

  cout << endl;

  cout << setiosflags(ios::right);
  cout << setw(10) << " " ;              PrintVectorInInches(vecs_g4[0]); cout << endl;
  cout << setiosflags(ios::right);
  cout << setw(10) << "- ";              PrintVectorInInches(vecs_g4[1]); cout << endl;
  cout << setw(7) << "a1prime" << " = "; PrintVectorInInches(a1prime);  cout << endl;
  cout << setw(7) << "a1"      << " = "; PrintVectorInInches(a1);       cout << endl;
  cout << setw(7) << "R*a1"    << " = "; PrintVectorInInches(a1p);   cout << endl;
  cout << "tr1 = r1 - R*a1 = "; PrintVectorInInches(tr1); cout << endl;
  cout << setw(7) << "Mag" << " = " << tr1.r() << endl;

  cout << endl;

  cout << setiosflags(ios::right);
  cout << setw(7) << "a2"      << " = "; PrintVectorInInches(a2);       cout << endl;
  cout << setw(7) << "R*a2"    << " = "; PrintVectorInInches(a2p);   cout << endl;
  cout << "tr2 = r0 - R*a2 = "; PrintVectorInInches(tr2); cout << endl;
  cout << setw(7) << "Mag" << " = " << tr2.r() << endl;

  cout << endl;

  cout << setiosflags(ios::right);
  cout << setw(7) << "a3"      << " = "; PrintVectorInInches(a3);   cout << endl;
  cout << setw(7) << "R*a3"    << " = "; PrintVectorInInches(a3p);  cout << endl;
  cout << "tr3 = r3 - R*a3 = ";          PrintVectorInInches(tr3);  cout << endl;
  cout << setw(7) << "Mag" << " = " << tr3.r() << endl;
  

  // cout << endl;
  // cout << "Centers " << endl;
  // cout << setw(7) << "tr2-tr0" << " = "; PrintVectorInInches(0.5*tr2+0.5*tr0); cout << endl;
  // cout << setw(7) << "tr3-tr1" << " = "; PrintVectorInInches(0.5*tr3+0.5*tr1); cout << endl;

  cout << endl;
  cout << setiosflags(ios::left);
  cout << setw(10) << "Plane 0"<< resetiosflags(ios::left); 
  PrintVectorInInches(norm0); cout << endl;
  cout << setiosflags(ios::left);
  cout << setw(10) << "Plane 1"<< resetiosflags(ios::left); 
  PrintVectorInInches(norm1); cout << endl;
  // cout << setw(10) << "Plane 2"<< resetiosflags(ios::left); 
  // PrintVectorInInches(norm2); cout << endl;


  ofstream of("rotatedandtranslated", ofstream::out);
  OutputVectorInInches(of,vecs_g4[2]); of << endl;
  OutputVectorInInches(of,vecs_g4[1]); of << endl;
  OutputVectorInInches(of,vecs_g4[0]); of << endl;
  OutputVectorInInches(of,vecs_g4[3]); of << endl;
  of.close();

  of.open("rotated", ofstream::out);
  OutputVectorInInches(of,a2p); of << endl;
  OutputVectorInInches(of,a1p); of << endl;
  OutputVectorInInches(of,a0p); of << endl;
  OutputVectorInInches(of,a3p); of << endl;
  of.close();

  of.open("unrotated", ofstream::out);
  OutputVectorInInches(of,a2); of << endl;
  OutputVectorInInches(of,a1); of << endl;
  OutputVectorInInches(of,a0); of << endl;
  OutputVectorInInches(of,a3); of << endl;
  of.close();

}


void
ComputeThickness(void)
{
  Target fTar;

  cout << "\n\nPoints BEFORE rotation" << endl;
  Hep3Vector p0( 0.5*fTar.GetLength(), -0.5*fTar.GetWidth(), -0.5*fTar.GetThickness());
  Hep3Vector p1(-0.5*fTar.GetLength(), -0.5*fTar.GetWidth(), -0.5*fTar.GetThickness());
  Hep3Vector p2(-0.5*fTar.GetLength(),  0.5*fTar.GetWidth(), -0.5*fTar.GetThickness());
  cout << setw(10) << "p0"<< resetiosflags(ios::left);  PrintVectorInInches(p0); cout << endl;
  cout << setw(10) << "p1"<< resetiosflags(ios::left);  PrintVectorInInches(p1); cout << endl;
  cout << setw(10) << "p2"<< resetiosflags(ios::left);  PrintVectorInInches(p2); cout << endl;

  p0 *= rot;
  p1 *= rot;
  p2 *= rot;
  
  Hep3Vector q0( 0.5*fTar.GetLength(), -0.5*fTar.GetWidth(), 0.5*fTar.GetThickness());
  Hep3Vector q1(-0.5*fTar.GetLength(), -0.5*fTar.GetWidth(), 0.5*fTar.GetThickness());
  Hep3Vector q2(-0.5*fTar.GetLength(),  0.5*fTar.GetWidth(), 0.5*fTar.GetThickness());
  cout << setw(10) << "q0"<< resetiosflags(ios::left);  PrintVectorInInches(q0); cout << endl;
  cout << setw(10) << "q1"<< resetiosflags(ios::left);  PrintVectorInInches(q1); cout << endl;
  cout << setw(10) << "q2"<< resetiosflags(ios::left);  PrintVectorInInches(q2); cout << endl;

  q0 *= rot;
  q1 *= rot;
  q2 *= rot;
  
  cout << endl;
  cout << "Points AFTER rotation" << endl;
  cout << setw(10) << "p0"<< resetiosflags(ios::left);  PrintVectorInInches(p0); cout << endl;
  cout << setw(10) << "p1"<< resetiosflags(ios::left);  PrintVectorInInches(p1); cout << endl;
  cout << setw(10) << "p2"<< resetiosflags(ios::left);  PrintVectorInInches(p2); cout << endl;
  cout << setw(10) << "q0"<< resetiosflags(ios::left);  PrintVectorInInches(q0); cout << endl;
  cout << setw(10) << "q1"<< resetiosflags(ios::left);  PrintVectorInInches(q1); cout << endl;
  cout << setw(10) << "q2"<< resetiosflags(ios::left);  PrintVectorInInches(q2); cout << endl;

  Hep3Vector pa = p1 - p0;
  Hep3Vector pb = p2 - p0;

  Hep3Vector qa = q1 - q0;
  Hep3Vector qb = q2 - q0;

  Hep3Vector np = pa.cross(pb);
  Hep3Vector nq = qa.cross(qb);
  np.setMag(1.0);
  nq.setMag(1.0);

  cout << "\nPlanes" << endl;
  cout << setw(10) << "np"<< resetiosflags(ios::left);  PrintVectorInInches(np); cout << endl;
  cout << setw(10) << "nq"<< resetiosflags(ios::left);  PrintVectorInInches(nq); cout << endl;
  

  G4double dp = -1.0*(np.x()*p0.x()+np.y()*p0.y()+np.z()*p0.z());
  G4double dq = -1.0*(nq.x()*q0.x()+nq.y()*q0.y()+nq.z()*q0.z());

  G4double zp = -1.0*dp/np.z();
  G4double zq = -1.0*dq/nq.z();
 
  std:: cout << zp << std::endl;
  std::cout << zq << std::endl;
  std::cout << "Rotated Thickness = " << fabs(zp-zq)/cm << std::endl;
  std::cout << "Scale factor = " << fabs(zp-zq)/fTar.GetThickness() << std::endl;
}

int
main()
{
  ComputeOffset();
  ComputeTranslation();
  ComputeThickness();
  
  return 0;
}
