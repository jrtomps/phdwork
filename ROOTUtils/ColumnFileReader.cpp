
#include "ColumnFileReader.h"
#include  <string>
#include  <sstream>
#include  <stdexcept>

using namespace std;

namespace PhD {
  namespace ColumnFileReader {

    std::vector<std::vector<double>> 
      parseFile(std::istream& stream, size_t nLinesToIgnore) 
      {
        std::vector<std::vector<double> > data;
        stringstream lineStream;
        string line;

        for (size_t i=0; i<nLinesToIgnore; ++i) { getline(stream, line); }

        while (1) {
          lineStream.clear();

          std::getline(stream, line);
          if (! stream.good() ) break;

          lineStream.str(line);
          
          auto lineData = parseLine(lineStream);
          if (lineData.size() > 0) {
            data.push_back(lineData);
          }
        }

        return data;
      }

    std::vector<double>   
      parseLine(std::istream& stream) 
      {
        vector<double> data;

        double value;
        while (1) {
          stream >> value;

          if (! stream.good() ) break;

          data.push_back(value);
        }

        return data;
      }


    std::vector<double>
      column(const std::vector<std::vector<double> > & matrix,
              size_t columnIndex) 
      {
        vector<double> columnData;

        for (auto& rowVector : matrix) {
          if (rowVector.size() <= columnIndex) {
            string msg("Data matrix contains only ");
            msg += to_string(rowVector.size());
            msg += " columns. User requested ";
            msg += to_string(columnIndex);
            throw std::runtime_error(msg);
          }

          columnData.push_back(rowVector.at(columnIndex));
        }

        return columnData;
      }
  }
}
