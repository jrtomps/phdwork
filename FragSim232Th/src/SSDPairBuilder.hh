// SSDPairBuilder.hh
//
// Jeromy Tompkins
// 9/15/2011
//
// Generates a pair of detectors


class G4AssemblyVolume;

#ifndef SSDPAIRBUILDER_H
#define SSDPAIRBUILDER_H 1

class SSDPairBuilder 
{
public:
  SSDPairBuilder();
  ~SSDPairBuilder();

  G4AssemblyVolume* GetPair() const;

private:
  void Assemble(void);
 
};
#endif
