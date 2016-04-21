#ifndef VECTORWRITER_H
#define VECTORWRITER_H

#include <ostream>
#include <string>
#include <map>

class VectorWriter
{
public:
    VectorWriter();
    VectorWriter(const VectorWriter& that);
    VectorWriter& operator=(const VectorWriter& that);

    ~VectorWriter();

    void Clear();

    UInt_t RegisterVector(const std::vector<Double_t>& vec);
    UInt_t RegisterVector(const std::string name,
                          const std::vector<Double_t>& vec);

    void Write(std::ostream& stream);

private:
    std::string TruncatedString(std::string str, size_t n);
    std::map<UInt_t, std::pair<std::string, std::vector<Double_t> > >* fData;
};

#endif // VECTORWRITER_H
