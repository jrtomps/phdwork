// ExtensibleDb.h
//
// Jeromy Tompkins
// 12/13/2011
//
// Database to store the various optional parameters.


#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <typeinfo>
#include <vector>
#include <algorithm>
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#endif
#include "DataElement.h"
#include "TObject.h"

#ifndef EXTENSIBLEDB_H
#define EXTENSIBLEDB_H 1

#ifndef __CINT__
typedef std::map<std::string, DataElement* > ExDb;
typedef std::pair<std::string, DataElement* > ExDbEntry;
#else
class ExDb;
class ExDbEntry;
#endif

class ExtensibleDb : public ExDb
{
 public:
  ExtensibleDb(void); 
  ExtensibleDb(std::istream& stream);
  ExtensibleDb(DataElement const& de);
  ExtensibleDb(ExtensibleDb const& obj);
  ExtensibleDb& operator=(ExtensibleDb const& obj);
  friend void swap(ExtensibleDb& lhs, ExtensibleDb& rhs);
  ~ExtensibleDb(void); 

  void GetValue(std::string const& headername, void* var_to_set);
  
  template<class T>
    void GetValues(std::string const& headername, std::vector<T>& data);
  
  void Write(std::ostream& stream, Char_t spacer=' ');
  void WriteEntry(std::string key, std::ostream& stream=std::cout, Char_t spacer=' ');

  void PrintKeys(std::ostream& stream=std::cout) const;
  void PrintHeaders(std::ostream& stream=std::cout, Bool_t printRaw=true) const;

  void Print(std::ostream& stream=std::cout, Char_t spacer=' ');

  void InteractivelyBuildDb(void);

  void InteractivelyAddDataElement(void);

  void AddDataElement(DataElement const& d);
  void AddDataElement(DataElement* de);

  Bool_t RemoveElement(std::string headername);

 private:
  void LoadDatabase(std::istream& stream);
  
  ClassDef(ExtensibleDb,0);

};

inline void swap(ExtensibleDb &lhs, ExtensibleDb &rhs)
{
    std::swap(lhs,rhs);
}


// Template class and function definitions!

template<class T>
class Data_tVectorToVectorConverter {
 public:
 Data_tVectorToVectorConverter(DataElement const& de) : fData(de) {} ;
  std::vector<T> operator()(void);
 private:
  DataElement fData;
};


template<class T>
void 
ExtensibleDb::GetValues(std::string const& headername, 
			std::vector<T>& data)
{
    ExDb::iterator it;
  it = find(headername);
  if (it == end())
    {
      std::cout << headername << " doesn't exist." << std::endl;
      return;
    }
  else
    {
      Data_tVectorToVectorConverter<T> converter(*it->second);
      std::vector<T> vec = converter();
      data = vec;
    } //end else statement
}

template<class T>
std::vector<T> 
Data_tVectorToVectorConverter<T>::operator()(void)
{
  std::vector<T> vec;
  //  std::cout << typeid(T).name() << std::endl;
  
  std::string type = fData.GetType();
  if (typeid(T) == typeid(int) && ( type.compare("Int_t")==0 || type.compare("int")==0))
    {
      vec.resize(fData.GetData().size());
      for (UInt_t j=0; j<vec.size(); j++)
	{
	  vec[j] = ((fData.GetData()[j]).i);
	}	       
    }
  else if (typeid(T) == typeid(float) && (type.compare("Float_t")==0 || type.compare("float")==0))
    {
      vec.resize(fData.GetData().size());
      for (UInt_t j=0; j<vec.size(); j++)
	{
	  vec[j] = (fData.GetData()[j]).f;
	}
    }
  else if (typeid(T) == typeid(double) && (type.compare("Double_t")==0 || type.compare("double")==0))
    {
      vec.resize(fData.GetData().size());
      for (UInt_t j=0; j<vec.size(); j++)
	{
	  vec[j] = (fData.GetData()[j]).d;
	}
    }
  else if (typeid(T) == typeid(char) && (type.compare("Char_t")==0 || type.compare("char")==0 ))
    {
      vec.resize(fData.GetData().size());
      for (UInt_t j=0; j<vec.size(); j++)
	{
	  vec[j] = (fData.GetData()[j]).c;
	}
    }
  /* else if (typeid(T) == typeid(std::string) && (type.compare("Char_t*")==0 || type.compare("string")==0 || type.compare("char*")==0 ) ) */
  /* 	{ */
  /* 	  vec.resize(fData.GetData().size()); */
  /* 	  for (UInt_t j=0; j<vec.size(); j++) */
  /* 	    { */
  /* 	      std::stringstream ss; */
  /* 	      ss << ((fData.GetData().at(j)).cstr); */
  /* 	      vec[j] = ss.str(); */
  /* 	    } */
  /* 	} */
  
  return vec;
}

#endif
