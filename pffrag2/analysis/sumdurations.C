{
    ConfigEntry* ce;
    Double_t sum;
    for (UInt_t i=490; i<560; ++i)
    {   
        ce = cm1.GetRun(i);
        if (ce!=0)
        {
            sum += ce->GetDuration();
        }

    }


    std::cout << "total_durations = " << sum->Get



}
