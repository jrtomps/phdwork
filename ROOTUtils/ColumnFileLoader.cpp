#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include "TString.h"
#include "ColumnFileLoader.h"

ColumnFileLoader::ColumnFileLoader()
    : fNPerDet(0),
    fDataBlockVector()
{
}

ColumnFileLoader::ColumnFileLoader(std::string fname)
    : fNPerDet(16),
    fDataBlockVector()
{
    LoadData(fname);
}
ColumnFileLoader::ColumnFileLoader(const ColumnFileLoader& that)
    : fNPerDet(that.fNPerDet), fDataBlockVector()
{
    if (this != &that)
    {
        fDataBlockVector = that.fDataBlockVector;
    }
}

ColumnFileLoader& ColumnFileLoader::operator=(ColumnFileLoader that)
                           {
    if (this != &that)
    {
        std::swap(fNPerDet, that.fNPerDet);
        std::swap(fDataBlockVector, that.fDataBlockVector);
    }
    return *this;
}

DataBlock ColumnFileLoader::GetDataForDet(UInt_t i)
{
    if (i<fDataBlockVector.size())
        return fDataBlockVector.at(i);
    else
        return DataBlock();
}

DataBlock ColumnFileLoader::GetDataForDets(std::string phi_orientation)
{
    DataBlock vec;
    if (phi_orientation.find("perp") != std::string::npos
        || phi_orientation.find("90") != std::string::npos)
    {
        vec = fDataBlockVector.at(3);
        vec.insert(vec.end(),fDataBlockVector.at(0).begin(),fDataBlockVector.at(0).end());
    }
    else
    {
        vec = fDataBlockVector.at(1);
        vec.insert(vec.end(),fDataBlockVector.at(2).begin(),fDataBlockVector.at(2).end());
    }
    return vec;
}

Bool_t ColumnFileLoader::FileIsNotOkay(std::ifstream& f)
{
    Bool_t notokay = false;
    std::ios::iostate state = f.rdstate();

    if (f.eof())
    {
        notokay = true;
    }
    else if (f.fail())
    {
        std::cerr << std::hex << std::showbase
                << "fstream is corrupted (badbit=" << (state&std::ios_base::badbit)
                << ",failbit=" << (state&std::ios_base::failbit) << ")"
                << std::dec << std::noshowbase
                << std::endl;
        notokay = true;
    }

    return notokay;
}

Bool_t ColumnFileLoader::LineShouldBeSkipped(std::string& line)
{
    if ( (line.length()==0) || (line.find_first_not_of(" \t") == std::string::npos) )
        return true;
    else
        return false;
}

Bool_t checkstate(std::stringstream& ss)
{
    std::ios::iostate state = ss.rdstate();
    std::ios::iostate bbit = std::ios::badbit;
    std::ios::iostate fbit = std::ios::failbit;
    std::ios::iostate ebit = std::ios::eofbit;

    std::cout << "| b:" << ((state&bbit)!=0) << " |"
            << "| f:" << ((state&fbit)!=0) << " |"
            << "| eof:" << ((state&ebit)!=0) << " |"
            << std::endl;

    return !ss.fail();//(state==std::ios::goodbit);
}

void ColumnFileLoader::LoadData(std::string fname)
{
    std::ifstream file(fname.data(), std::ifstream::in);
    if (file.fail())
    {
        std::cerr << "Failed to open " << fname << std::endl;
        return;
    }

    DataBlock data;

    Double_t sa;
    std::string aline;
    std::stringstream ss;
    while(1)
    {
        getline(file,aline);
//        std::cout << aline;
        if ( FileIsNotOkay(file) )
            break;
        else if ( LineShouldBeSkipped(aline) )
            continue;
        else
        {
            RowData vec;
            UInt_t c=0;
            ss.clear(); ss.str("");

            ss << aline;
            ss >> sa;
            while (!ss.fail()) //(checkstate(ss))
            {
                vec.push_back(sa);
                ss >> sa;
                c++;
            }
//            std::cout << " : found " << std::setw(3) << c << " numbers";

            if (c>0)
            {
                if (fDataBlockVector.size()==0)
                    fDataBlockVector.push_back(data);
                else if (fNPerDet!=0)
                {
                    if (fDataBlockVector.back().size()%fNPerDet==0)
                        fDataBlockVector.push_back(data);
                }
                fDataBlockVector.back().push_back(vec);
            }
        }
//        std:cout << std::endl;
    }

    file.close();
}

void ColumnFileLoader::PrintValueTable(UInt_t column_index)
{
    std::map<TString, ColumnData> tmap;
    std::map<TString, ColumnData>::iterator it;

    tmap["Para"] = GetColumnForDets("parallel",column_index);
    tmap["Perp"] = GetColumnForDets("perpendicular",column_index);


    //it = tmap.begin();
    std::cout << std::right
            << std::setprecision(2)
            << std::setw(3) << " ";
    for (it=tmap.begin(); it!=tmap.end(); it++)
    {
        std::cout << std::setw(10) << it->first;
    }
    std::cout << "\n"
            << std::setw(3) << " "
            << std::setfill('-')
            << std::setw(10*tmap.size()) << '-'
            << std::setfill(' ');

    std::cout << std::setiosflags(std::ios::scientific);

    for (UInt_t i=0; i<tmap.begin()->second.size(); i++)
    {
        std::cout << "\n"
                << std::setw(3) << i;
        for (it=tmap.begin(); it!=tmap.end(); it++)
        {
            std::cout << std::setw(10)
                    << it->second.at(i);
        }
    }

    std::cout << std::endl;
    std::cout << std::resetiosflags(std::ios::scientific)
            << std::setprecision(6);

}

void ColumnFileLoader::PrintValueTable(std::string col_indices)
{
    std::map<TString, std::vector<ColumnData> > tmap;
    std::map<TString, std::vector<ColumnData> >::iterator it;
    std::vector<ColumnData>::iterator cd_it;

    tmap["Perp[0]"] = GetColumnsForDet(0,col_indices);
    tmap["Para[1]"] = GetColumnsForDet(1,col_indices);
    tmap["Para[2]"] = GetColumnsForDet(2,col_indices);
    tmap["Perp[3]"] = GetColumnsForDet(3,col_indices);


    //it = tmap.begin();
    std::cout << std::right
            << std::setprecision(2)
            << std::setw(3) << " ";

    for (it=tmap.begin(); it!=tmap.end(); it++)
    {
        for(cd_it=it->second.begin(); cd_it!=it->second.end(); cd_it++)
        {
            std::cout << std::setw(10) << it->first
                    << "(" << std::distance(it->second.begin(),cd_it)
                    << ")";
        }
    }
    std::cout << "\n"
            << std::setw(3) << " "
            << std::setfill('-')
            << std::setw(13*tmap.size()*tmap.begin()->second.size()) << '-'
            << std::setfill(' ');

    std::cout << std::setiosflags(std::ios::scientific);

    for (UInt_t i=0; i<tmap.begin()->second.front().size(); i++)
    {
        std::cout << "\n"
                << std::setw(3) << i;
        for (it=tmap.begin(); it!=tmap.end(); it++)
        {
            for(cd_it=it->second.begin(); cd_it!=it->second.end(); cd_it++)
            {
                std::cout << std::setw(13) << cd_it->at(i);
            }
        }
    }

    std::cout << std::endl;
    std::cout << std::resetiosflags(std::ios::scientific)
            << std::setprecision(6);

}


ColumnData ColumnFileLoader::GetColumnForDet(UInt_t det_index, UInt_t col_index)
{
    ColumnData vec;
    if (det_index>=fDataBlockVector.size()) return vec;

    for (UInt_t i=0; i<fDataBlockVector.at(det_index).size(); i++)
    {
        if (fDataBlockVector.at(det_index).at(i).size()>col_index)
            vec.push_back(fDataBlockVector.at(det_index).at(i).at(col_index));
        else
            continue;
    }

    return vec;
}

ColumnData ColumnFileLoader::GetColumnForDets(std::string phi_orientation, UInt_t col_index)
{
    ColumnData vec;
    if (phi_orientation.find("perp") != std::string::npos
        || phi_orientation.find("90") != std::string::npos)
    {
        vec = GetColumnForDet(3,col_index);
        ColumnData tmpvec = GetColumnForDet(0,col_index);
        vec.insert(vec.end(),tmpvec.begin(),tmpvec.end());
    }
    else
    {
        vec = GetColumnForDet(1,col_index);
        ColumnData tmpvec = GetColumnForDet(2,col_index);
        vec.insert(vec.end(),tmpvec.begin(),tmpvec.end());
    }
    return vec;
}

//std::vector<ColumnData> ColumnFileLoader::GetColumnsForDets(std::string phi_orientation, std::string column_indices)
//{
//    std::vector<ColumnData> res;
//    ColumnData vec;
//    if (phi_orientation.find("perp") != std::string::npos
//        || phi_orientation.find("90") != std::string::npos)
//    {
//        vec = GetColumnForDet(3,col_index);
//        ColumnData tmpvec = GetColumnForDet(0,col_index);
//        vec.insert(vec.end(),tmpvec.begin(),tmpvec.end());
//    }
//    else
//    {
//        vec = GetColumnForDet(1,col_index);
//        ColumnData tmpvec = GetColumnForDet(2,col_index);
//        vec.insert(vec.end(),tmpvec.begin(),tmpvec.end());
//    }
//    return vec;
//}

std::vector<ColumnData> ColumnFileLoader::GetColumnsForDet(UInt_t det_index, std::string col_indices)
{
    std::vector<ColumnData> res;

    std::stringstream ss;
    ss << col_indices;
    UInt_t index;
    ss >> index;
    while (!ss.fail())
    {
        res.push_back(GetColumnForDet(det_index,index));
        ss >> index;
    }

    return res;
}
