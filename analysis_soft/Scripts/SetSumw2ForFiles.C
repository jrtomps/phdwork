

vector<TH1*>
GetAllTH1(TFile& rootfile)
{
  TH1 *h=0;
  TKey *obj=0;
  TListIter li(rootfile.GetListOfKeys());
  vector<TH1*> histvec;

  while (obj = static_cast<TKey*>(li.Next()))
    {
      h = dynamic_cast<TH1*>(obj->ReadObj());
      if (h!=NULL && h->InheritsFrom("TH1")) 
	histvec.push_back(h);
    }
  cout << histvec.size() << endl;
  return histvec;
}
