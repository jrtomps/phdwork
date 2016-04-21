#ifndef REGIONINTEGRATORMULTIDIM_H
#define REGIONINTEGRATORMULTIDIM_H

#include "Rtypes.h"
#include <vector>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <Math/IntegratorMultiDim.h>
#include <Math/IntegrationTypes.h>
#include <Math/IFunction.h>
#include "TCutG.h"


//! Limits to a rectangular region
/*!
  * @code
  *  grid_yhigh -> ------------------------------------
  *                |      |                  |    o   |  <- cntr_yhigh
  *  cntr_ylow  -> |    o |                  |        |
  *  grid_ylow  -> ------------------------------------
  *                ^    ^                         ^   ^
  *                |    cntr_xlow                 |   grid_xhigh
  *                |                              |
  *                grid_xlow                      cntr_xhig
  * @endcode
  *
  *
  */
struct Region
{
    Double_t grid_xlow; //!< lower bound x-component
    Double_t cntr_xlow; //!<
    Double_t grid_xhigh;
    Double_t cntr_xhigh;

    Double_t grid_ylow;
    Double_t cntr_ylow;
    Double_t grid_yhigh;
    Double_t cntr_yhigh;

};


//! Insertion operator
/*!
  * Writes the data encapsulated by the Region to a stream with proper
  * formatting.
  */
inline std::ostream& operator<<(std::ostream& stream, const Region& reg)
{
    using std::setw;
    stream << std::setiosflags(std::ios::scientific | std::ios::right )
            << std::setprecision(2);

    stream << setw(10) << reg.grid_xlow
            << setw(10) << reg.cntr_xlow
            << setw(10) << reg.grid_xhigh
            << setw(10) << reg.cntr_xhigh
            << setw(10) << reg.grid_ylow
            << setw(10) << reg.cntr_ylow
            << setw(10) << reg.grid_yhigh
            << setw(10) << reg.cntr_yhigh;

    stream << std::resetiosflags(std::ios::scientific | std::ios::right )
            << std::setprecision(6);
    return stream;
}

//! Multiplicative Assignment
/*!
  * Because the a region is a easily thought of as a value, and likely contains
  * data stored with a unit, it makes sense to easily apply a scaling to all of
  * the region as a whole.
  */
inline Region& operator*=(Region& reg, const Double_t v)
{
    reg.grid_xlow *= v;
    reg.grid_ylow *= v;
    reg.grid_xhigh *= v;
    reg.grid_yhigh *= v;

    reg.cntr_xlow *= v;
    reg.cntr_ylow *= v;
    reg.cntr_xhigh *= v;
    reg.cntr_yhigh *= v;

    return reg;
}


//! A macroscopic region for integration purposes
/*! A set of rectangles when combined form a region.
  * This combines a series of Region structs to do just this.
  * The result is useful because integration over the region can
  * be executed by merely summing the integrals of the
  * subregions that compose the region.
  */
typedef std::vector<Region> Regions;

template<class T> T regions_cast(Regions& obj) {}

template<> inline TCutG regions_cast<TCutG>(Regions& obj)
{
    std::vector<Double_t> x(2*obj.size()+1,0);
    std::vector<Double_t> y(2*obj.size()+1,0);
    UInt_t n = obj.size();
    for (UInt_t i=0; i<n; i++)
    {
        x[i] = obj[i].cntr_xlow;
        y[i] = obj[i].cntr_ylow;
    }

    for (UInt_t i=0; i<n; i++)
    {
        x[i+n] = obj[n-i-1].cntr_xhigh;
        y[i+n] = obj[n-i-1].cntr_yhigh;
    }

    x.back() = obj[0].cntr_xlow;
    y.back() = obj[0].cntr_ylow;

    return TCutG("regions", x.size(), x.data(), y.data());
}

//! Insertion operator overload
/*!
  * Sequentially writes the subregions that make up the Regions object
  * to the stream. The output of this command is formatting such that
  * if stored in a file, the RegionFileLoader object can reconstruct a
  * Regions object from it.
  */
inline std::ostream& operator<<(std::ostream& stream, const Regions& reg)
{
    for (UInt_t i=0; i<reg.size(); i++)
    {
        stream << reg[i] << "\n";
    }
    return stream;
}

//! Multiplicative assignment operator
/*!
  * Scale the entirety of the data stored in the Regions object
  * by a factor
  */
inline Regions& operator*=(Regions& reg, const Double_t v)
{
    for (UInt_t i=0; i<reg.size(); i++)
    {
        reg[i] *= v;
    }
    return reg;
}

inline std::ostream& operator<<(std::ostream& stream, const std::vector<Regions>& reg)
{
    for (UInt_t i=0; i<reg.size(); i++)
    {
        stream << "det" << i
                << "\n" << reg[i];
    }

    return stream;
}

//! Load the data stored as a region from a file
/*!
  * Retrieves data stored in a file. The retrieved data
  * is stored as a vector of Regions
  *
  *
  * Data is stored in the following form:
  *
  * grid_xlow  cntr_xlow  grid_xhigh  cntr_xhigh  grid_ylow  cntr_ylow  grid_yhigh  cntr_yhigh
  *
  */
class RegionFileLoader
{
private:
    std::vector<Regions> fTheRegions;

public:
    //! Default constructor
    /*!
      * Initializes the stored buffer of Regions but does no more.
      * A subsequent call to the LoadDataFromFile function can be
      * used to fill the buffer.
      */
    RegionFileLoader();

    //! Constructor from an input file
    /*!
      * Construct the data from an already opened input file stream.
      * If the file stream is not able to perform input operations,
      * nothing is read from it. Multiple regions can be stored in a
      * given file by separating them by a line containing "det<#>".
      *
      * @param ifile is an input file stream storing data to construct Regions object
      */
    RegionFileLoader(std::ifstream& ifile);

    /*! Access the stored vector of Regions objects.
      *
      * @return const reference to the data owned by this object
      */
    const std::vector<Regions>& GetRegions() const;

    //! Read in data specifying Regions from an input file stream
    /*!
      * If the file stream is not able to perform input operations,
      * nothing is read from it. Multiple regions can be stored in a
      * given file by separating them by a line containing "det<#>".
      *
      * @param ifile is an input file stream storing data to construct Regions object
      *
      */
    void LoadDataFromFile(std::ifstream& ifile);

private:
    Bool_t FileIsNotOkay(std::ifstream& f);
    Bool_t LineShouldBeSkipped(std::string& line);
    Bool_t BeginningOfRegions(std::string& line);
    Regions ReadRegions(std::ifstream& f);
    Bool_t IsNumber(Char_t c);
};


//! A numerical integrator capable of integrating a multi-dim function over a specified region
/*!
  * Extends the ROOT::Math::IntegratorMultiDim by adding the possibility to
  * integrate over a non rectangular shape specified by a Regions object
  */
class RegionIntegratorMultiDim : public ROOT::Math::IntegratorMultiDim
{
private:
    Bool_t fUseCenters;

public:
    //! Constructor
    /*!
      * The final parameter in the construct specifies the option to use the external edges of
      * the Regions argument or to use the computed center.
      */
    RegionIntegratorMultiDim(ROOT::Math::IntegrationMultiDim::Type type=ROOT::Math::IntegrationMultiDim::kDEFAULT, double absTol=0, double relTol=0, unsigned int ncall=0);

    //! Integrate a region
    /*!
      * A function is assumed to have been set prior to this operation.
      */
    Double_t RegionIntegral(const Regions& reg);

    //! Integrate a function over a region
    Double_t RegionIntegral(const ROOT::Math::IMultiGenFunction &f, const Regions &reg);

    Bool_t IsUsingCenter() const {return fUseCenters;}
    void SetUsingCenter(Bool_t flag) { fUseCenters = flag; }

};

#endif // REGIONINTEGRATORMULTIDIM_H
