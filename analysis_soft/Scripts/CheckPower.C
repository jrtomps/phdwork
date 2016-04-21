

#include "NIMCrate.h"
#include "NIMModule.h"

NIMCrate crateL0(10, 10, 3, 3, 1.5, 1.5);
NIMCrate crateL1(10, 10, 3, 3, 1.5, 1.5);
NIMCrate crateR0(10, 10, 3, 3, 1.5, 1.5);
NIMCrate crateR1(10, 10, 3, 3, 1.5, 1.5);
NIMCrate crateR2(12, 12, 4, 4, 2.0, 2.0);


void
CheckPowerLeft0()
{
  NIMModule *ps794 = new NIMModule(0.42,0.40,0.10,0.160,0.040,0.065);
  ps794->SetName("ps794");
  NIMModule *ps755_0 = new NIMModule(0.25,0.40,0.00,0.165,0.035,0.060);
  ps755_0->SetName("ps755");

  NIMModule *ps744 = new NIMModule(0.40,0.45,0.05,0.165,0.000,0.080);
  ps744->SetName("ps744");

  NIMModule *n568b = new NIMModule(0.750,0,2.3,1.05,0,0);
  n568b->SetName("n568b");

  NIMModule *ps706 = new NIMModule(0.40,0.45,0.05,0.165,0,0.08);
  ps706->SetName("ps706");

  NIMModule *ps726 = new NIMModule(0.35,0.40,0.10,0.170,0.060,0.080);
  ps726->SetName("ps726");

  NIMModule *ps755_1 = new NIMModule(0.25,0.40,0.00,0.165,0.035,0.060);
  ps755_1->SetName("ps755");

  crateL0.AddModule(ps794,0);
  crateL0.AddModule(ps755_0,1);
  crateL0.AddModule(ps744,2);
  crateL0.AddModule(n568b,3);
  crateL0.AddModule(ps706,4);
  crateL0.AddModule(ps726,5);
  crateL0.AddModule(ps755_1,6);

  crateL0.PrintPowerUsage();
  crateL0.PrintAllModules();
  
}

void
CheckPowerLeft1(void)
{

  NIMModule *n568b = new NIMModule(0.750,0,2.3,1.05,0,0);
  n568b->SetName("n568b");

  NIMModule *clock = new NIMModule(0,0,0,0,0,0);
  clock->SetName("clock");

  NIMModule *o710  = new NIMModule(0,0,0.39,0.36,0.09,0.09);
  o710->SetName("O710 Quad Bias");
  
  NIMModule *l729A = new NIMModule(0.30,0.42,0.035,0.07,0,0);
  l729A->SetName("l729A");

  NIMModule *ps726 = new NIMModule(0.35,0.40,0.10,0.170,0.060,0.080);
  ps726->SetName("ps726");

  crateL1.AddModule(n568b,0);
  crateL1.AddModule(clock,1);
  crateL1.AddModule(o710,2);
  crateL1.AddModule(l729A,3);
  crateL1.AddModule(ps726,4);

  crateL1.PrintPowerUsage();
  crateL1.PrintAllModules();
  

};

void
CheckPowerRight2(void)
{

  NIMModule *n568b = new NIMModule(0.750,0,2.3,1.05,0,0);
  n568b->SetName("n568b");

  NIMModule *o710  = new NIMModule(0,0,0.39,0.36,0.09,0.09);
  o710->SetName("O710 Quad Bias");
 
  NIMModule *l428F = new NIMModule(0,0,0.16,0.16,0.08,0.08);
  l428F->SetName("l428F FanInOut");

  NIMModule *l427FL_0 = new NIMModule(0,0,0.165,0.155,0.025,0.035);
  l427FL_0->SetName("l427FL FanIn");

  NIMModule *l427FL_1 = new NIMModule(0,0,0.165,0.155,0.025,0.035);
  l427FL_1->SetName("l427FL FanIn");

  NIMModule *ps706 = new NIMModule(0.40,0.45,0.05,0.165,0,0.08);
  ps706->SetName("ps706");

  NIMModule *n843 = new NIMModule(1.1,3.4,0.06,0.03,0,0);
  n843->SetName("n843 16ch CFD");
  
  crateR2.AddModule(n568b,0);
  crateR2.AddModule(o710,1);
  crateR2.AddModule(l428F,2);
  crateR2.AddModule(ps706,3);
  crateR2.AddModule(n843,4);  
  crateR2.AddModule(l427FL_0,5);
  crateR2.AddModule(l427FL_1,6);

  crateR2.PrintPowerUsage();
  crateR2.PrintAllModules();
  

};
