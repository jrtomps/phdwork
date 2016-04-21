
//0     HighCut
//1     HighCut2
//2     HighCut3
//3     HighTOFCut
//4     LowCut
//5     LowCut2
//6     LowCut3
//7     LowTOFCut
//8     LowerIntBound
//9     Pedestal
//10    Threshold
//11    UpperIntBound
//



{
    gROOT->ProcessLine(".L analysis_soft/Scripts/SetThresholdsForRuns.C+");
    gROOT->ProcessLine(".L analysis_soft/Scripts/SetGatesForRuns.C+");


    cout << "\n";
    cout << "Begin database setup       \n";
    cout << "===========================\n";
    cout << "\n\n";
    cout << "Setup ADC threshold        \n";
    cout << "---------------------------\n";
    cout << endl;

    SetThresholdsForRuns(false,"Threshold");
    cout << "\n";
    cout << "---------------------------\n";


    cout << "Setup TDC gates        \n";
    cout << "---------------------------\n";
    cout << endl;
    SetGatesForRuns(0,"LowCut","HighCut");
    cout << "\n";
    cout << "---------------------------\n";

    cout << "Setup Secondary TDC gates        \n";
    cout << "---------------------------\n";
    cout << endl;
    SetGatesForRuns(0,"LowCut2","HighCut2");
    cout << "\n";
    cout << "---------------------------\n";

    cout << "Setup Tertiary TDC gates        \n";
    cout << "---------------------------\n";
    cout << endl;
    SetGatesForRuns(0,"LowCut3","HighCut3");
    cout << "\n";
    cout << "---------------------------\n";

    cout << "Setup TOF gates       \n";
    cout << "---------------------------\n";
    cout << endl;
    SetGatesForRuns(0,"LowTOFCut","HighTOFCut");
    cout << "\n";
    cout << "---------------------------\n";
    cout << "Database setup complete    \n";
    cout << "===========================\n";

}
