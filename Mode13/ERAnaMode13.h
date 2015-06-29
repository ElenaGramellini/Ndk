/**
 * \file ERAnaMode13.h
 *
 * \ingroup Mode13
 * 
 * \brief Class def header for a class ERAnaMode13
 *
 * @author elenag
 */

/** \addtogroup Mode13

    @{*/

#ifndef ERTOOL_ERANAMODE13_H
#define ERTOOL_ERANAMODE13_H

#include "ERTool/Base/AnaBase.h"

namespace ertool {

  /**
     \class ERAnaMode13
     User custom Analysis class made by kazuhiro
   */
  class ERAnaMode13 : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaMode13(const std::string& name="ERAnaMode13");

    /// Default destructor
    virtual ~ERAnaMode13(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

  };
}
#endif

/** @} */ // end of doxygen group 
