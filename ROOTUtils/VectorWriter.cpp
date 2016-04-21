#include <vector>
#include <iomanip>
#include <string>
#include <ostream>
#include <sstream>
#include <map>
#include <utility>
#include "Rtypes.h"
#include "VectorWriter.h"

VectorWriter::VectorWriter()
    : fData(NULL)
{
    fData = new std::map<UInt_t, std::pair<std::string, std::vector<Double_t> > >;
 }

VectorWriter::~VectorWriter()
{
    delete fData;
}

VectorWriter::VectorWriter(const VectorWriter& that)
    : fData(NULL)
{
    if (this!=&that)
    {
        std::map<UInt_t, std::pair<std::string, std::vector<Double_t> > >* tmp1;
        tmp1 = new std::map<UInt_t, std::pair<std::string, std::vector<Double_t> > >(*that.fData);

        delete fData;

        fData = tmp1;
    }
}

VectorWriter& VectorWriter::operator=(const VectorWriter& that)
                                     {
    if (this!=&that)
    {
        std::map<UInt_t, std::pair<std::string, std::vector<Double_t> > > * tmp1;
        tmp1 = new std::map<UInt_t, std::pair<std::string, std::vector<Double_t> > >(*that.fData);

        delete fData;

        fData = tmp1;
    }
    return *this;
}

void VectorWriter::Clear()
{
    fData->clear();
}

UInt_t VectorWriter::RegisterVector(const std::vector<Double_t> &vec)
{
    std::stringstream ss;
    ss << fData->size();
    fData->insert(std::make_pair(fData->size(),std::make_pair(ss.str(), vec)));
    return std::distance(fData->begin(),fData->end())-1;
}

UInt_t VectorWriter::RegisterVector(const std::string name,
                                    const std::vector<Double_t> &vec)
{
    fData->insert(std::make_pair(fData->size(),std::make_pair(name,vec)));
    return std::distance(fData->begin(),fData->end())-1;
}

void VectorWriter::Write(std::ostream& stream)
{
    stream << std::setiosflags(std::ios::scientific)
            << std::right
            << std::setprecision(4);
    std::map<UInt_t, std::pair<std::string, std::vector<double> > >::iterator it;
    stream << std::setw(3) << " ";

    for (it=fData->begin(); it!=fData->end(); it++)
    {
        if ((*it).second.first.length()<11)
            stream << std::setw(12) << (*it).second.first;
        else
            stream << std::setw(12) << TruncatedString((*it).second.first,11);
    }

    for (unsigned int i=0; i<fData->begin()->second.second.size(); i++)
    {
        stream << "\n" << std::setw(3) << i;

        for (it=fData->begin(); it!=fData->end(); it++)
        {
            stream << std::setw(12) << (*it).second.second.at(i);
        }
    }
    stream << std::resetiosflags(std::ios::scientific)
            << std::setprecision(6)
            << std::endl;
}

std::string VectorWriter::TruncatedString(std::string str, size_t n)
{
    std::string cpy = str;
    if (str.length()>=n)
    {
        // n=8
        // str="asdfljasdfljasflj"
        //      01234567890123456
        // ret "asd...lj"
        // rep_str = "..."
        // replace n-1-2-rep_str.length() = n-6
        // ntorep str.length()-2-(n+1-6) = str.length()-n+3 = 12

        cpy.replace(n-6,str.length()-n+3,"...");

    }
    return cpy;
}
