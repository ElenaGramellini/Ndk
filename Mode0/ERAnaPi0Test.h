/**
 * \file ERAnaPi0Test.h
 *
 * \ingroup Mode0
 * 
 * \brief Class def header for a class ERAnaPi0Test
 *
 * @author wderocco
 */

/** \addtogroup Mode0

    @{*/

#ifndef ERTOOL_ERANAPI0TEST_H
#define ERTOOL_ERANAPI0TEST_H

#include "ERTool/Base/ParticleGraph.h"
#include "ERTool/Base/AnaBase.h"

namespace ertool {

  /**
     \class ERAnaPi0Test
     User custom Analysis class made by kazuhiro
   */
  class ERAnaPi0Test : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaPi0Test(const std::string& name="ERAnaPi0Test");

    /// Default destructor
    virtual ~ERAnaPi0Test(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

  };
}
#endif

/** @} */ // end of doxygen group 
