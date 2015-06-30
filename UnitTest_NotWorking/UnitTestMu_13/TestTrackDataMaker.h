/**
 * \file TestTrackDataMaker.h
 *
 * \ingroup App
 * 
 * \brief Class def header for a class TestTrackaMaker
 *
 * @author elena
 */

/** \addtogroup App
    
    @{*/
#ifndef TESTTRACKDATAMAKER_H
#define TESTTRACKDATAMAKER_H

#include <iostream>
#include "ERTool/Base/Track.h"
#include "ERTool/Base/UtilFunc.h"

namespace ertool {
  /**
     \class TestTrackDataMaker
     User defined class TestTrackDataMaker ... these comments are used to generate
     doxygen documentation!
  */
  class TestTrackDataMaker{
    
  public:
    /// Default constructor
    TestTrackDataMaker(){}
    
    /// Default destructor
    ~TestTrackDataMaker(){}
    
    /// Track maker
    Track MakeTrack(double x,  double y,  double z,
		    double px, double py, double pz,
		    int pdg_code)
    {
      Track t;
      t.push_back(::geoalgo::Vector(x,y,z));
      auto mass = ParticleMass(pdg_code);
      t._energy = sqrt(pow(mass,2) + pow(px,2) + pow(py,2) + pow(pz,2) );
      return t;
    }
    
    /// Trajectory parameter
    void SetTrajectoryParams(const double step_size   = 5, // cm
			     const double step_sigma  = 1, // cm
			     const double angle_sigma = 5, // degree
			     const double dedx = 2 // MeV/cm 
			     )
    {
      _track_step_size  = step_size;
      _track_step_sigma = step_sigma;
      _track_step_angle_sigma = angle_sigma;
      _track_step_dedx = dedx;
    }
    
    void Clear()
    { _track_v.clear();}

  private:
    std::vector< ertool::Track  > _track_v;
    //
    // Track params
    //
    double _track_step_size;        ///< step size of a trajectory in cm 
    double _track_step_sigma;       ///< that for sigma 
    double _track_step_angle_sigma; ///< angle obviously in degree
    double _track_step_dedx;        ///< guess your unit
  };
}
#endif
/** @} */ // end of doxygen group 
    
