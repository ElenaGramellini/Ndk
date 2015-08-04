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

#include "ERTool/Base/AlgoBase.h"
#include "ERTool/Algo/AlgoEMPart.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoAABox.h"
#include "GeoAlgo/GeoSphere.h"
#include <algorithm> // for std::find
#include <utility>
#include <TTree.h>

#include "RecoAlg/TrackMomentumCalculator.h"

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
    
  private:
    /// clear tree
    void ClearTree();

    
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

    TTree* _algoMu_tree ;
    int n_mu = 0;

    double _mu_En    = -999.9;
    double _mu_DepEn = -999.9;
    double _mu_Mom   = -999.9;
    double _mu_px    = -999.9;
    double _mu_py    = -999.9;
    double _mu_pz    = -999.9;

    double _mu_x     = -999.9;
    double _mu_y     = -999.9;
    double _mu_z     = -999.9;

    double _mu_xEnd  = -999.9;
    double _mu_yEnd  = -999.9;
    double _mu_zEnd  = -999.9;
    double _mu_leng  = -999.9;

    double _mu_EnMPS = -999.9;
    double _mu_MomMPS= -999.9;
    double _mu_pxMPS = -999.9;
    double _mu_pyMPS = -999.9;
    double _mu_pzMPS = -999.9;

    // trkf::TrackMomentumCalculator momCalc;

    
  };
}
#endif

/** @} */ // end of doxygen group 
