

#include <fstream>
#include "TPolyLine3D.h"

TTimer timer(25);
//TGLViewer::ECameraType camera;

void
empty()
{

};

void
Draw()
{

  gStyle->SetCanvasPreferGL(true);

  TPolyLine3D *unrot = new TPolyLine3D(4);
  unrot->SetLineColor(1);
  TPolyLine3D *rot = new TPolyLine3D(4);
  rot->SetLineColor(2);
  TPolyLine3D *rot_trans = new TPolyLine3D(4);
  rot_trans->SetLineColor(3);

  Double_t x, y, z;
  ifstream ifile("unrotated",ifstream::in);
  for (Int_t i=0; i<4; i++)
    {
      ifile >> x >> y >> z;
      unrot->SetPoint(i,x,y,z);
    }
  ifile.close();

  ifile.open("rotated",ifstream::in);
  for (Int_t i=0; i<4; i++)
    {
      ifile >> x >> y >> z;
      rot->SetPoint(i,x,y,z);
    }
  ifile.close();

  ifile.open("rotatedandtranslated",ifstream::in);
  for (Int_t i=0; i<4; i++)
    {
      ifile >> x >> y >> z;
      rot_trans->SetPoint(i,x,y,z);
    }
  ifile.close();

  unrot->Draw("AL ogl");
  rot->Draw("L ogl");
  rot_trans->Draw("L ogl");
   

  // TGLViewer *v = (TGLViewer*) gPad->GetViewer3D();
  // camera = (TGLViewer::ECameraType) 1;
  // v->SetCurrentCamera(camera);
  // v->CurrentCamera().SetExternalCenter(true);

  
  // TGLSAViewer *sav = dynamic_cast<TGLSAViewer*>(v);
  // if (sav)
  //   {
  //     sav->GetFrame()->Connect("CloseWindow()","TTimer",&timer,"TurnOff()");
  //   }

  // timer.SetCommand("empty()");
  // timer.TurnOn();
  


}
