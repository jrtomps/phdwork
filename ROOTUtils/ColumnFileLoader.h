
#ifndef COLUMNFILELOADER_H
#define COLUMNFILELOADER_H

#include "Rtypes.h"
#include <vector>
#include <fstream>
#include <string>

typedef std::vector<Double_t> RowData;
typedef std::vector<Double_t> ColumnData;
typedef std::vector<RowData> DataBlock;
typedef std::vector<DataBlock> DataBlockVector;

class ColumnFileLoader
{

public:
    ColumnFileLoader();

    ColumnFileLoader(std::string fname);

    ColumnFileLoader(const ColumnFileLoader& that);

    ColumnFileLoader& operator=(ColumnFileLoader that );

    ~ColumnFileLoader() throw() {};

    DataBlock GetDataForDet(UInt_t i);

    DataBlock GetDataForDets(std::string phi_orientation);

    ColumnData GetColumnForDet(UInt_t det_index, UInt_t col_index);

    std::vector<ColumnData> GetColumnsForDet(UInt_t det_index, std::string col_indices="0 1");

    ColumnData GetColumnForDets(std::string phi_orientation, UInt_t col_index);

//    std::vector<ColumnData> GetColumnsForDets(std::string phi_orientation, std::string column_indices);

    UInt_t GetNPerDet(void) const
    {
        return fNPerDet;
    }

    void PrintValueTable(UInt_t col_index);

    void PrintValueTable(std::string col_indices="0 1");
    
    void LoadData(std::string fname);

    void SetNPerDet(UInt_t i)
    { fNPerDet = i; };

private:
    UInt_t fNPerDet;
    DataBlockVector fDataBlockVector;
    Bool_t FileIsNotOkay(std::ifstream& f);
    Bool_t LineShouldBeSkipped(std::string &line);

};

#endif // COLUMNFILELOADER_H
