#ifndef Parameters_H
#define Parameters_H

#include "Rtypes.h"

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<Parameter>;
}
#endif

#include <string>
#include <utility>
#include <fstream>
#include <map>
#include "Parameter.h"


//! Parameters
/*!
 * A group of Parameter objects that are organized as an stl map.
  */
class Parameters : public std::map<UInt_t, std::shared_ptr<Parameter> >
{
public:
    //! Default constructor
    Parameters();
    /*! Constructor that will read in data from a properly formatted file
* @param parfile is an ifstream that stores the parameters
* this can differentiate between a FitParameter and Parameter when it reads the data in.
* Finally, the file format should be as follows:
*   index   name    value   err lower_bound    upper_bound  stepsize
*
* However, the last 3 values are optional and only necessary for FitParameter objects.
*/
    Parameters(std::ifstream& parfile);

    /*! Constructor that takes an initial Parameter object
      */
    Parameters(const Parameter& par);
    Parameters(const Parameters& obj);
    Parameters& operator=(Parameters obj);
    void swap(Parameters& params) throw();
    friend void swap(Parameters& lhs, Parameters& rhs);

    Parameters& operator*=(const Double_t val);

    ~Parameters();

    /*! Add a parameter to the list. If a parameter exists with the specified
      * index, the operation will fail without throwing an exception but without
      * successfully adding the Parameter object
      */
    void AddParameter(UInt_t index, const Parameter& par);
    /*! Add a parameter without specifying the index. The internally stored index
      * of the referenced Parameter will be used as the key
      */
    void AddParameter(const Parameter& par);

    /*! Check to see that the n keys stored in the map are the following
      * values: 0, 1, 2, ..., n-1
      */
    Bool_t KeysAreSensible() const;
};

inline void swap(Parameters &lhs, Parameters &rhs)
{
    lhs.swap(rhs);
}

#include <ostream>
#include <iomanip>
inline std::ostream& operator<<(std::ostream& stream, Parameters& params)
{
    Parameters::iterator it = params.begin();
    for (; it!=params.end(); it++)
    {
        FitParameter* fp = dynamic_cast<FitParameter*>(it->second.get());

        if (fp!=0)  stream << *fp << std::endl;
        else        stream << *(it->second) << std::endl;
    }
    return stream;

}

inline std::ostream& operator<<(std::ostream& stream, std::pair<std::streamsize,Parameters> params)
{
    Parameters::iterator it = params.second.begin();
    for (; it!=params.second.end(); it++)
    {
        stream << std::setw(params.first) << ' ';

        FitParameter* fp = dynamic_cast<FitParameter*>(it->second.get());
        if (fp!=0)  stream << *fp << std::endl;
        else        stream << *(it->second) << std::endl;
    }
    return stream;

}

#endif // Parameters_H
