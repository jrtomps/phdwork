
#include <TObject.h>

class TH1;

class HPusher : public TObject 
{

  private:
    double m_offset;

  public:
    HPusher(double offset);


    TH1* operator()(TH1* pHist);

    void setOffset(double offset);
    double getOffset() const;

    ClassDef(HPusher,1);
};
