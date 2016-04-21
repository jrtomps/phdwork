// NuclearChartBuilder.h
//
// Jeromy Tompkins
// 3/16/2011
//
// Creates a nuclear chart from an input file

#include <string>
#include "NuclearChart.h"
#include "TObject.h"

#ifndef NUCLEARCHARTBUILDER_H
#define NUCLEARCHARTBUILDER_H

class NuclearChartBuilder 
{
 public:
  NuclearChartBuilder(const std::string& setupfile);
  NuclearChartBuilder(NuclearChartBuilder const& obj);
  virtual ~NuclearChartBuilder(void);
  NuclearChartBuilder& operator=(NuclearChartBuilder const& obj);
  
  NuclearChart GetChart(void) const;

 protected:
  virtual void Load(const std::string& setupfile);
  NuclearChart chart;

  ClassDef(NuclearChartBuilder,0);

};

#endif
