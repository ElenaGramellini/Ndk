/**
 * \file ERAlgoMu.h
 *
 * \ingroup Mode13
 * 
 * \brief Class def header for a class ERAlgoMu
 *
 * @author elenag
 */

/** \addtogroup Mode13

    @{*/

#ifndef ERTOOL_ERALGOMU_H
#define ERTOOL_ERALGOMU_H

#include "ERTool/Algo/AlgoEMPart.h"
#include "ERTool/Base/AlgoBase.h"

namespace ertool {

  /**
     \class ERAlgoMu
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoMu : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoMu(const std::string& name="ERAlgoMu");

    /// Default destructor
    virtual ~ERAlgoMu(){};

    /// Reset function
    void Reset();

    void Finalize();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

    void InitializeHistos();
   
    /// set verbosity flag
    void setVerbose(bool on) { _verbose = on; }

    /*
    /// Function to set minumum required length for a muon track
    void setMinMuonLength(double l) { _minMuonLength = l; }
    
    /// Function to set the maximum allowed muon-michel separation
    void setMaxDistance(double d) { _maxDistance = d; }
    */
    
    
  protected:
    
    // Instance of EMPart Algorithm
    // AlgoEMPart _alg_emp;
    
    // histograms to be filled in Algo

    /*
    TH1F* shower_st_to_track_end;
    
    // cut values that are settable by user
    double _minMuonLength;
    double _maxDistance;
    */

    // verbosity flag
    bool _verbose;
    int n_mu = 0;
    
  };
}
#endif

/** @} */ // end of doxygen group 
