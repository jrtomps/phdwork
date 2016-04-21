
#include <iostream>
#include <fstream>
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"

#include "G4UIterminal.hh"
#ifdef G4UI_USE_TCSH
#include "G4UItcsh.hh"
#endif

#include "G4VisExecutive.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
//#include "FragSimSource.hh"
#include "FragSimSource2.hh"
//#include "AllVolumeSource.hh"
//#include "AllVolumeSource2.hh"
#include "PrimarySupervisor.hh"
#include "AngDist.h"
#include "Parameters.h"
#include <dirent.h>

#include "FragSimDetConstruction.hh"
#include "FragSimPrimaryGeneratorAction.hh"
#include "FragSimEventAction.hh"
#include "FragSimRunAction.hh"
#include "RandomSeedManager.hh"
#include "PhysicsList.hh"

G4String AskForRunName(void);

AngDist GetAngDist();

int 
main(int argc, char* argv[])
{
    G4String run_name = AskForRunName();

    RandomSeedManager rsm(run_name);

    G4RunManager* runManager = new G4RunManager;

    try
    {
        // set mandatory initialization classes
        FragSimDetConstruction *fsdc = new FragSimDetConstruction;
        runManager->SetUserInitialization(fsdc);
        runManager->SetUserInitialization(new PhysicsList);

//        FragSimSource2 fss(0.9525*cm,
        FragSimSource2 fss(1.02*cm,
                           "av_1_impr_7_target_log_pv_0",
                           FragSimSource2::kSurfaceLayer,
                           10.0*um);

//        AllVolumeSource2 fss("expHall","av_1_impr_7_target_log_pv_0", 33.0*um);
        PrimarySupervisor ps("expHall","av_1_impr_7_target_log_pv_0");

        FragSimPrimaryGeneratorAction *pga = new FragSimPrimaryGeneratorAction;
        pga->SetSource(fss);
        pga->SetPrimarySupervisor(&ps);

        runManager->SetUserAction(pga);

        runManager->SetUserAction(new FragSimRunAction);

        runManager->SetUserAction(new FragSimEventAction);

#ifdef G4VIS_USE
        std::cout << "G4VIS_USE conditional code executed" << std::endl;
        G4VisManager* visManager = new G4VisExecutive;
        visManager->Initialize();
#endif    

        // get the pointer to the User Interface manager
        G4UImanager* UI = G4UImanager::GetUIpointer();

        if (argc!=1)   // batch mode
        {
            G4String command = "/control/execute ";
            G4String fileName = argv[1];
            UI->ApplyCommand(command+fileName);
        }

        else           //define visualization and UI terminal for interactive mode
        { 
#ifdef G4UI_USE
            G4UIExecutive *ui = new G4UIExecutive(argc, argv);
            ui->SessionStart();
            delete ui;
#endif
            G4UIsession * session = 0;
#ifdef G4UI_USE_TCSH
            session = new G4UIterminal(new G4UItcsh);
#else
            //	session = new G4UIterminal();
#endif     
            //	session->SessionStart();
            //	delete session;

#ifdef G4VIS_USE
            delete visManager;
#endif     
        }      

    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    // Free the heap: user actions, physics_list and detector_description are
    //                 owned and deleted by the run manager, so they should not
    //                 be deleted in the main() program !
    delete runManager;

    return 0;
}

void DisplayAllFilesInDirOfType(G4String dr, G4String suffix)
{
    ::DIR* dp = ::opendir(dr.data());
    ::dirent* dir;

    G4String name;

    G4cout << std::left
            << "\nThe following .dats file are in the current folder\n";
    while( (dir = ::readdir(dp) ) )
    {
        name = dir->d_name;
        if (name.rfind(suffix)!= G4String::npos)
        {
            G4cout << "\n\t" << name;
        }
    }
    ::closedir(dp);

    G4cout << std::right << std::endl;
}

AngDist GetAngDist()
{
    G4String par_file_dir("params/.");
    G4String par_file_name;
    std::cout << "\nFound the following param files";
    DisplayAllFilesInDirOfType(par_file_dir,".params");
    std::cout << "\nEnter param file : ";
    if (std::cin.peek()=='\n') std::cin.get();
    std::getline(std::cin, par_file_name);
    std::ifstream ifile((par_file_dir+"/"+par_file_name).data(),std::ios::in);
    Parameters params(ifile);
    ifile.close();

    std::cout << "\nFound the following parameters:"
            << "\n" << params
            << std::endl;

    AngDist w(params);
    w.SetConstrainedRange(false);
    return w;
}

G4String
AskForRunName(void)
{
    G4String run_name;
    std::cout << "Enter run name : ";
    std::getline(std::cin, run_name);
    return run_name;
}
