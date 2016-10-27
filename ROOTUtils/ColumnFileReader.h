

#include <vector>
#include <iosfwd>


namespace PhD {
  namespace ColumnFileReader {

  std::vector<std::vector<double> > parseFile(std::istream& stream, size_t nLinesToIgnore);
  std::vector<double>               parseLine(std::istream& stream);

  std::vector<double>               column(const std::vector<std::vector<double> >& matrix,
                                           size_t columnIndex);
  

  } // end ColumnFileReader
} // end PhD namespace

