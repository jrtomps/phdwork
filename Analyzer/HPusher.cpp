
#include "HPusher.h"
#include <TH1.h>
#include <random>


ClassImp(HPusher)

HPusher::HPusher(double offset) : m_offset(offset) {}


TH1* HPusher::operator()(TH1* pHist) 
{
    TH1* pClone = dynamic_cast<TH1*>(pHist->Clone());
    pClone->Reset("ICE");

    TAxis* pAxis = pHist->GetXaxis();

    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_real_distribution<> dist(0,1);

    for (int bin=1; bin<=pAxis->GetNbins(); ++bin) {
      double content = pHist->GetBinContent(bin);

      for (int iter=0; iter<content; ++iter) {

        double val = dist(gen)*pAxis->GetBinWidth(bin)+pAxis->GetBinLowEdge(bin);

        pClone->Fill(m_offset+val);
      }
    }

    for (int bin=0; bin<=pAxis->GetNbins(); ++bin) {
      pHist->SetBinContent(bin, pClone->GetBinContent(bin));
    }

    delete pClone;

    return pHist;
}


void HPusher::setOffset(double offset) 
{
  m_offset = offset;
}

double HPusher::getOffset() const 
{
  return m_offset;
}
