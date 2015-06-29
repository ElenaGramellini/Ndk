/**
 * \file ERAlgoMode13.h
 *
 * \ingroup Mode13
 * 
 * \brief Class def header for a class ERAlgoMode13
 *
 * @author elenag
 */

/** \addtogroup Mode13

    @{*/

#ifndef ERTOOL_ERALGOMODE13_H
#define ERTOOL_ERALGOMODE13_H

#include "ERTool/Base/AlgoBase.h"

namespace ertool {

  /**
     \class ERAlgoMode13
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoMode13 : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoMode13(const std::string& name="ERAlgoMode13");

    /// Default destructor
    virtual ~ERAlgoMode13(){};

    /// Reset function
    void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

  };
}
#endif

/** @} */ // end of doxygen group 
