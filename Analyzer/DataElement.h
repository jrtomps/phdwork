// DataElement.h
//
// Jeromy Tompkins
// 12/13/2011
//
//

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "TObject.h"
#include "Rtypes.h"

#ifndef DATAELEMENT_H
#define DATAELEMENT_H 1

union Data_t
{
  Int_t               i;
  Float_t             f;
  Double_t            d;
  Char_t              c;
  Char_t*          cstr;
};

class DataElement  : public TObject
{
 private:
  DataElement(void);
  
 public:
  static DataElement* Construct(void);

  template<typename T>
    static DataElement* Construct(std::vector<T>& vec);
  
  template<typename T>
    static DataElement* Construct(std::string& header, std::vector<T>& vec);
  DataElement(std::string& raw_header);
  DataElement(std::string& raw_header, std::istream& stream);
  DataElement(DataElement const& obj);
  DataElement& operator=(DataElement const& obj);
  ~DataElement();

  std::string GetType(void) const;
  std::string GetHeader(void) const;
  std::string GetRawHeader(void) const;
  std::vector<Data_t> GetData(void);
  void Write (std::ostream& stream, Char_t spacer=' ');
  friend std::ostream& operator<<(std::ostream& stream, DataElement& de);

 private:
  void InteractivelyBuildRawHeader(void);
  void InteractivelyLoadData(void);
  template<class T>
    void LoadData(std::vector<T>& vec);
  std::string ConstructRawHeader(void);
  void ParseRawHeader(std::string& raw_header);
  void LoadData(std::istream& stream);
  std::string fRawHeader;
  std::string fHeader;
  std::string fType;
  Int_t       fSize;
  std::vector<Data_t> fData;

  ClassDef(DataElement,0);
};

inline
std::ostream& operator<<(std::ostream& stream, DataElement& obj)
{
    stream << obj.GetRawHeader() << ' ';
    for (UInt_t i=0; i<obj.fData.size(); i++)
    {
        if (obj.fType.compare("Int_t")==0 || obj.fType.compare("int")==0)
            stream << obj.fData[i].i;
        else if (obj.fType.compare("Float_t")==0 || obj.fType.compare("float")==0)
            stream << obj.fData[i].f;
        else if (obj.fType.compare("Double_t")==0 || obj.fType.compare("double")==0)
            stream << obj.fData[i].d;
        else if (obj.fType.compare("Char_t*")==0 || obj.fType.compare("string")==0
                 || obj.fType.compare("char*")==0)
        {
            stream << '\"' << obj.fData[i].cstr << '\"';
        }
        else if (obj.fType.compare("Char_t")==0 || obj.fType.compare("char")==0)
            stream << obj.fData[i].c;

        stream << ' ';
    }
    return stream;
}

template<typename T>
DataElement* DataElement::Construct(std::vector<T>& vec)
{
  DataElement* de = new DataElement;
  std::cout << std::left << std::setw(15) << "Enter header" << " : ";
  std::cin  >> de->fHeader;
  de->LoadData<T>(vec);
  de->fRawHeader = de->ConstructRawHeader();
  return de;
}

template<typename T>
DataElement* DataElement::Construct(std::string& header, std::vector<T>& vec)
{
  DataElement* de = new DataElement;	       
  de->fHeader = header;
  de->LoadData<T>(vec);
  de->fRawHeader = de->ConstructRawHeader();
  return de;
}


template<class T>
void DataElement::LoadData(std::vector<T>& vec)
{

  fData.resize(vec.size());

  if (typeid(T) == typeid(int))
    {
      fType = "Int_t";
      for (UInt_t i=0; i<vec.size(); i++)
        fData[i].i = vec[i];
    }
  else if (typeid(T) == typeid(float))
    {
      fType = "Float_t";
      for (UInt_t i=0; i<vec.size(); i++)
        fData[i].f = vec[i];
    }
  else if (typeid(T) == typeid(double))
    {
      fType = "Double_t";
      for (UInt_t i=0; i<vec.size(); i++)
        fData[i].d = vec[i];
    }
  else if (typeid(T) == typeid(char))
    {
      fType = "Char_t";
      for (UInt_t i=0; i<vec.size(); i++)
        fData[i].c = vec[i];
    }

  fSize = fData.size();
  
}

#endif
