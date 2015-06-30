/**
 * \file TestShowerDataMaker.h
 *
 * \ingroup 
 * 
 * \brief Class def header for a class TestShowerDataMaker
 *
 * @author elena
 */

/** \addtogroup App

    @{*/
#ifndef TESTSHOWERDATAMAKER_H
#define TESTSHOWERDATAMAKER_H

#include <iostream>
#include "ERTool/Base/Shower.h"
#include "ERTool/Base/UtilFunc.h"
namespace ertool {
  /**
     \class TestShowerDataMaker
     User defined class TestShowerDataMaker ... these comments are used to generate
     doxygen documentation!
  */
  class TestShowerDataMaker{
    
  public:
    
    /// Default constructor
    TestShowerDataMaker(){}
    
    /// Default destructor
    ~TestShowerDataMaker(){}



    /// Shower adder
    Shower MakeShower(double x, double y, double z,
		      double px, double py, double pz,
		      int pdg_code)
    {
      Shower s( geoalgo::Vector(x,y,z),
		geoalgo::Vector(px,py,pz),
		30, 15);
      s._energy = 100;
      s._dedx = 2;
      return s;
    }

    /// Vtx sigma
    void SetVertexSigma(const double s)
    { _vtx_sigma = s; }

   
    /// Shower parameter
    void SetShowerParams(const double length, // cm
			 const double dedx=2  // MeV/cm
			 )
    {
      _shower_length = length;
      _shower_dedx   = dedx;
    }

    /// Clear added elements
    void Clear()
    { _shower_v.clear();}
    
    


  private:

    std::vector< ertool::Shower > _shower_v;

    /// vtx point smearing
    double _vtx_sigma;

   
    // Shower params
    double _shower_length;
    double _shower_dedx;

  };
}
#endif
/** @} */ // end of doxygen group 

