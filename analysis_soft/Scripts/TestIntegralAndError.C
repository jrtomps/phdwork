



void
TestIntegralAndError()
{
  TH1F * h = new TH1F ("h","",10,0,10);
  h->Sumw2();

  for (Int_t i=0; i<=11; i++)
    {
      h->Fill(i);
    }

  Double_t err, val;
  val = h->IntegralAndError(h->GetXaxis()->GetFirst(),h->GetXaxis()->GetLast(),err);
  cout << val << "\t" << err << endl;
  h->Rebin(5);
  val = h->IntegralAndError(h->GetXaxis()->GetFirst(),h->GetXaxis()->GetLast(),err);
  cout << val << "\t" << err << endl;
  

}
