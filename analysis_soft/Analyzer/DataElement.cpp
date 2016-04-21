// DataElement.h
//
// Jeromy Tompkins
// 12/13/2011
//
//

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "DataElement.h"
#include "TObject.h"

ClassImp(DataElement);

DataElement::DataElement(void)
    : fRawHeader(),
    fHeader(),
    fType(),
    fSize(0),
    fData()
{
}

DataElement* DataElement::Construct(void)
{
    DataElement* de = new DataElement;
    de->InteractivelyBuildRawHeader();
    de->fData.resize(de->fSize);
    de->InteractivelyLoadData();
    return de;
}

DataElement::DataElement(std::string& raw_header)
    : fRawHeader(),
    fHeader(),
    fType(),
    fSize(0),
    fData()  
{
    ParseRawHeader(raw_header);
    fData.resize(fSize);
    InteractivelyLoadData();
}

DataElement::DataElement(std::string& raw_header, std::istream& stream)
    : fRawHeader(),
    fHeader(),
    fType(),
    fSize(0),
    fData()  
{
#ifdef DEBUG
    std::cout << "Constructing DataElement: " << raw_header << std::endl;
#endif 
    ParseRawHeader(raw_header);
    fData.resize(fSize);
    LoadData(stream);
}

DataElement::DataElement(DataElement const& obj)
    : fRawHeader(),
    fHeader(),
    fType(),
    fSize(0),
    fData()  
{
    if (this != &obj)
    {
        fRawHeader = obj.fRawHeader;
        fHeader    = obj.fHeader;
        fType      = obj.fType;
        fSize      = obj.fSize;
        fData      = obj.fData;
    }
}

DataElement& DataElement::operator=(DataElement const& obj)
{
    if (this != &obj)
    {
        fRawHeader = obj.fRawHeader;
        fHeader    = obj.fHeader;
        fType      = obj.fType;
        fSize      = obj.fSize;
        fData      = obj.fData;
    }
    return *this;
}

DataElement::~DataElement()
{
    if (fType.compare("Char_t*")==0 || fType.compare("string")==0
        || fType.compare("char*")==0)
    {
        for (UInt_t i=0; i<fData.size(); i++)
            if (fData[i].cstr!=0) delete [] fData[i].cstr;
    }
}

std::string DataElement::GetType(void) const
{ return fType;}

std::string DataElement::GetHeader(void) const
{ return fHeader;}

std::string DataElement::GetRawHeader(void) const
{ return fRawHeader;}

std::vector<Data_t> DataElement::GetData(void)
{  return fData;}

void DataElement::ParseRawHeader(std::string& raw_header)
{
    fRawHeader = raw_header;
    std::string str = fRawHeader;
    size_t in0 = str.rfind('[');
    size_t in1 = str.rfind(',');
    size_t in2 = str.rfind(']');
    fType = str.substr(in0+1,in1-in0-1);
    fSize = atoi(str.substr(in1+1,in2-in1-1).data());

    fHeader = str.substr(0,in0);
}

void DataElement::LoadData(std::istream& stream)
{
    std::string str, tmp;
    for (Int_t i=0; i<fSize; i++)
    {
        if (fType.compare("Int_t")==0 || fType.compare("int")==0)
            stream >> fData[i].i;
        else if (fType.compare("Float_t")==0 || fType.compare("float")==0)
            stream >> fData[i].f;
        else if (fType.compare("Double_t")==0 || fType.compare("double")==0)
            stream >> fData[i].d;
        else if (fType.compare("Char_t*")==0 || fType.compare("string")==0
                 || fType.compare("char*")==0)
        {
            getline(stream, str, '\"');
            getline(stream, str, '\"');
            // str.insert(0,1,'\"');
            // str.append(1,'\"');
            fData[i].cstr = new Char_t[str.length()+1];
            std::copy(str.begin(), str.end(), fData[i].cstr);
        }
        else if (fType.compare("Char_t")==0 || fType.compare("char")==0)
            stream >> fData[i].c;
    }

}

void DataElement::Write(std::ostream& stream, Char_t spacer)
{
    stream << ConstructRawHeader() << spacer;
    for (UInt_t i=0; i<fData.size(); i++)
    {
        if (fType.compare("Int_t")==0 || fType.compare("int")==0)
            stream << fData[i].i;
        else if (fType.compare("Float_t")==0 || fType.compare("float")==0)
            stream << fData[i].f;
        else if (fType.compare("Double_t")==0 || fType.compare("double")==0)
            stream << fData[i].d;
        else if (fType.compare("Char_t*")==0 || fType.compare("string")==0
                 || fType.compare("char*")==0)
        {
            stream << '\"' << fData[i].cstr << '\"';
        }
        else if (fType.compare("Char_t")==0 || fType.compare("char")==0)
            stream << fData[i].c;

        stream << spacer;
    }

}


void DataElement::InteractivelyLoadData(void)
{
    Char_t ans;
    std::cout << "Enter values individually? (y/n) : ";
    std::cin  >> ans;

    Data_t val;
    std::string str;
    if (ans=='n' || ans=='N')
    {

        std::cout << "Value = ";
        if (fType.compare("Int_t")==0 || fType.compare("int")==0)
            std::cin  >> val.i;
        else if (fType.compare("Float_t")==0 || fType.compare("float")==0)
            std::cin >> val.f;
        else if (fType.compare("Double_t")==0 || fType.compare("double")==0)
            std::cin >> val.d;
        else if (fType.compare("Char_t*")==0 || fType.compare("string")==0 || fType.compare("char*")==0)
        {
            getline(std::cin, str, '\n');
        }
        else if (fType.compare("Char_t")==0 || fType.compare("char")==0)
            std::cin >> val.c;

        for (Int_t i=0; i<fSize; i++)
        { 
            if (fType.compare("Int_t")==0 || fType.compare("int")==0)
                fData[i].i = val.i;

            else if (fType.compare("Float_t")==0 || fType.compare("float")==0)
                fData[i].f = val.f;

            else if (fType.compare("Double_t")==0 || fType.compare("double")==0)
                fData[i].d = val.d;

            else if (fType.compare("Char_t*")==0 || fType.compare("string")==0 || fType.compare("char*")==0)
            {
                fData[i].cstr = new Char_t[str.length()+1];
                std::copy(str.begin(), str.end(), fData[i].cstr);
            }
            else if (fType.compare("Char_t")==0 || fType.compare("char")==0)
                fData[i].c = val.c;
        } // end for loop
    } // end if (ans=='n' || ans=='N')
    else
    {
        std::cout << "Value = ";
        for (Int_t i=0; i<fSize; i++)
        { 
            if (fType.compare("Int_t")==0 || fType.compare("int")==0)
            {
                std::cin  >> val.i;
                fData[i].i = val.i;
            }
            else if (fType.compare("Float_t")==0 || fType.compare("float")==0)
            {
                std::cin >> val.f;
                fData[i].f = val.f;
            }
            else if (fType.compare("Double_t")==0 || fType.compare("double")==0)
            {
                std::cin >> val.d;
                fData[i].d = val.d;
            }
            else if (fType.compare("Char_t*")==0 || fType.compare("string")==0
                     || fType.compare("char*")==0)
            {
                getline(std::cin, str, '\n');
                if (str[0]!='\"')
                    str.insert(0,1,'\"');
                if (str[str.length()-1]!='\"')
                    str.append(1,'\"');
                fData[i].cstr = new Char_t[str.length()+1];
                std::copy(str.begin(), str.end(), fData[i].cstr);
            }
            else if (fType.compare("Char_t")==0 || fType.compare("char")==0)
            {
                std::cin >> val.c;
                fData[i].c = val.c;
            }
        }
    }

}


void DataElement::InteractivelyBuildRawHeader()
{
    Int_t index, size;
    std::cout << std::left << std::setw(15) << "Enter header" << " : ";
    std::cin  >> fHeader;
    std::cout << std::left << std::setw(15) << "Choose type" << " : " << std::endl;
    std::cout << std::left << std::setw(15) << 1 << "Int_t" << std::endl;
    std::cout << std::left << std::setw(15) << 2 << "Float_t" << std::endl;
    std::cout << std::left << std::setw(15) << 3 << "Double_t" << std::endl;
    std::cout << std::left << std::setw(15) << 4 << "Char_t*" << std::endl;
    std::cout << std::left << std::setw(15) << 5 << "Char_t" << std::endl;
    std::cin  >> index;

    switch(index)
    {
    case 1:
        fType.append("Int_t");
        break;
    case 2:
        fType.append("Float_t");
        break;
    case 3:
        fType.append("Double_t");
        break;
    case 4:
        fType.append("Char_t*");
        break;
    case 5:
        fType.append("Char_t");
        break;
    default:
        std::cout << "Bad input. Exiting." << std::endl;
        return;
        break;
    }

    std::cout << std::setw(25) << "Number datapoints" << " : ";
    std::cin  >> fSize;

    fRawHeader = ConstructRawHeader();

};

std::string DataElement::ConstructRawHeader(void)
{
    std::stringstream ss;
    ss << fHeader << "[" << fType << "," << fSize << "]";

    return ss.str();
}
