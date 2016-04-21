// ExtensibleDb.h
//
// Jeromy Tompkins
// 12/13/2011
//
// Database to store the various optional parameters.

#include <iostream>
#include <iomanip>
#include <string>
#include <iterator>
#include <sstream>
#include <map>
#include <utility>
#include <algorithm>
//#include <memory>

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#endif

#include "DataElement.h"
#include "ExtensibleDb.h"
#include "TObject.h"

ClassImp(ExtensibleDb);

ExtensibleDb::ExtensibleDb(void)
    : ExDb()
{
    InteractivelyBuildDb();
}

ExtensibleDb::ExtensibleDb(DataElement const& obj)
    : ExDb()
{
    AddDataElement(obj);
}

ExtensibleDb::ExtensibleDb(std::istream& stream)
{
#ifdef DEBUG
    std::cout << "Constructing ExtensibleDb" << std::endl;
#endif 
    LoadDatabase(stream);
}

ExtensibleDb::ExtensibleDb(ExtensibleDb const& obj)
    : ExDb(obj)
{}

ExtensibleDb& ExtensibleDb::operator=(ExtensibleDb const& obj)
{
    if (this != &obj)
    {
        ExDb::operator=(obj);
    }
//    std::swap(obj, *this);
    return *this;
}

void
SafeDeleteSecond(std::pair<std::string, DataElement*> pair)
{
 if (pair.second!=0) {
    delete pair.second;
    pair.second = 0;
  }
}


ExtensibleDb::~ExtensibleDb(void)
{
  for_each(begin(), end(), SafeDeleteSecond);
}

void ExtensibleDb::GetValue(std::string const& headername, void* var_to_set)
{
    ExDb::iterator it;
    it = find(headername);
    if (it==end())
        std::cout << headername << " doesn't exist." << std::endl;
    else
    {
        std::string type = it->second->GetType();
        if (type.compare("Int_t")==0 || type.compare("int")==0)
        {
            std::vector<Int_t> vec;
            vec.resize(it->second->GetData().size());
            for (UInt_t j=0; j<vec.size(); j++)
	    {
                vec[j] = ((it->second->GetData()[j]).i);
	    }	       
            (*(reinterpret_cast<std::vector<Int_t>* >(var_to_set))) = vec;

	}
        else if (type.compare("Float_t")==0 || type.compare("float")==0)
	{
            std::vector<Float_t> vec;
            vec.resize(it->second->GetData().size());
            for (UInt_t j=0; j<vec.size(); j++)
	    {
                vec[j] = (it->second->GetData()[j]).f;
	    }
            *(reinterpret_cast<std::vector<Float_t>* >(var_to_set)) = vec;

	}
        else if (type.compare("Double_t")==0 || type.compare("double")==0)
	{
            std::vector<Double_t> vec;
            vec.resize(it->second->GetData().size());
            for (UInt_t j=0; j<vec.size(); j++)
	    {
                vec[j] = (it->second->GetData()[j]).d;
	    }
            (*(reinterpret_cast<std::vector<Double_t>* >(var_to_set))) = vec;
        }
        else if (type.compare("Char_t*")==0 || type.compare("string")==0 || type.compare("char*")==0 )
        {
            std::vector<std::string> vec;
            vec.resize(it->second->GetData().size());
            for (UInt_t j=0; j<vec.size(); j++)
	    {
                vec[j] = (it->second->GetData()[j]).cstr;
	    }
            (*(reinterpret_cast<std::vector<std::string>* >(var_to_set))) = vec;
        }
        else if (type.compare("Char_t")==0 || type.compare("char")==0 )
        {
            std::vector<Char_t> vec;
            vec.resize(it->second->GetData().size());
            for (UInt_t j=0; j<vec.size(); j++)
	    {
                vec[j] = (it->second->GetData()[j]).c;
	    }
            (*(reinterpret_cast<std::vector<Char_t>* >(var_to_set))) = vec;
	}
    }
}

void ExtensibleDb::Write(std::ostream& stream, Char_t spacer)
{
    ExDb::iterator it;
    for (it = begin(); it!=end(); it++)
    {
        it->second->Write(stream, spacer);
    }
}

void ExtensibleDb::WriteEntry(std::string key, std::ostream &stream, Char_t spacer)
{
    ExDb::iterator it = find(key);
    if (it!=end())
    {
        stream << *(it->second);
    }

}

void ExtensibleDb::PrintKeys(std::ostream& stream) const
{
    stream << std::left
            << std::setw(6) << "Index"
            << std::setw(30) << "Key Name"
            << "\n"
            << std::setfill('-') << std::setw(5) << "-"
            << " "
            << std::setw(30) << "-"
            << std::setfill(' ');

    ExDb::const_iterator it;
    for (it = begin(); it!=end(); it++)
    {
        stream << "\n"
                << std::setw(6) << std::distance(begin(), it)
                << std::setw(30) << it->first;

    }
    stream << std::right << std::endl;

}

void ExtensibleDb::PrintHeaders(std::ostream& stream, Bool_t printRaw) const
{
    stream << std::left
            << std::setw(6) << "Index"
            << std::setw(30) << "Header"
            << "\n"
            << std::setfill('-') << std::setw(5) << "-"
            << " "
            << std::setw(30) << "-"
            << std::setfill(' ');

    ExDb::const_iterator it;
    for (it = begin(); it!=end(); it++)
    {
        stream << "\n"
                << std::setw(6) << std::distance(begin(), it);
        if (printRaw)
        {
            stream << std::setw(30) << it->second->GetRawHeader();
        }
        else
        {
            stream << std::setw(30) << it->first;
        }
    }
    stream << std::right << std::endl;

}

void ExtensibleDb::Print(std::ostream& stream, Char_t spacer)
{
    ExDb::iterator it;
    for (it = begin(); it!=end(); it++)
    {
        it->second->Write(stream, spacer);
        stream << std::endl;
    }
}

void ExtensibleDb::LoadDatabase(std::istream& stream)
{
    std::string str;
    Char_t peeker;
    // Specifically skip over spaces but not all white space.
    stream >> std::noskipws;
    peeker = stream.peek();
    while( peeker == ' ') stream >> peeker;
    stream.unget();

    stream >> std::skipws;
    while (stream.peek()<'0' || stream.peek()>'9')
    {
        stream >> str;
        if (stream.rdstate()!=0) break;

        DataElement* de(new DataElement(str, stream));
        //DataElement *de = new DataElement(str, stream));
        insert(ExDbEntry(de->GetHeader(),de));
        stream >> std::ws;
    }
    stream >> std::ws;
}


void ExtensibleDb::InteractivelyBuildDb(void)
{
    Char_t answer='y';
    while (answer=='y' || answer=='Y')
    {
        InteractivelyAddDataElement();
        std::cout << std::endl;
        std::cout << "Add another DataElement? (y/n)";
        std::cin  >> answer;
    }
}

void ExtensibleDb::InteractivelyAddDataElement(void)
{
    DataElement* de(DataElement::Construct());

    UInt_t original_size = size();

    insert(ExDbEntry(de->GetHeader(),de));
    if (original_size == size() )
    {
        std::cout << "DataElement already exists with this header." << std::endl;
        std::cout << "Do you want to overwrite? (y/n) : ";
        Char_t ans;
        std::cin  >> ans;
        if (ans == 'y' || ans=='Y')
        {
            erase(de->GetHeader());
            insert(ExDbEntry(de->GetHeader(),de));
        }
    }
}

void ExtensibleDb::AddDataElement(DataElement const& d)
{
    DataElement* de(new DataElement(d));

    UInt_t original_size = size();

    insert(ExDbEntry(de->GetHeader(),de));
    if (original_size == size() )
    {
        std::cout << "DataElement already exists with this header." << std::endl;
        std::cout << "Do you want to overwrite? (y/n) : ";
        Char_t ans;
        std::cin  >> ans;
        if (ans == 'y' || ans=='Y')
        {
            erase(de->GetHeader());
            insert(ExDbEntry(de->GetHeader(),de));
        }
    }
}


void ExtensibleDb::AddDataElement(DataElement* de)
{
    DataElement* dd(new DataElement(*de));
    UInt_t original_size = size();

    insert(ExDbEntry(dd->GetHeader(),dd));
    if (original_size == size() )
    {
        std::cout << "DataElement already exists with this header." << std::endl;
        std::cout << "Do you want to overwrite? (y/n) : ";
        Char_t ans;
        std::cin  >> ans;
        if (ans == 'y' || ans=='Y')
        {
            erase(dd->GetHeader());
            insert(ExDbEntry(dd->GetHeader(),dd));
        }
    }
}

Bool_t ExtensibleDb::RemoveElement(std::string headername)
{
   UInt_t original_size = size();
   erase(headername);

   return (size()!=original_size);
}
