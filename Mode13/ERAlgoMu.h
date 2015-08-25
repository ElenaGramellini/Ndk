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
    int n_tracks = 0;
    std::vector<int>    _tracks_size ;
    std::vector<double> _tracks_steps    ;
    std::vector<double> _tracks_stepsDensity    ;
    std::vector<double> _tracks_den1;
    std::vector<double> _tracks_den2;
    std::vector<double> _tracks_den3;
    std::vector<double> _mu_begEndLength ;
    std::vector<double> _mu_lengthRatio  ;

    std::vector<double>_mu_ReducedLength20; std::vector<double> _mu_FakeLength20;
    std::vector<double>_mu_ReducedLength10; std::vector<double> _mu_FakeLength10;
    std::vector<double>_mu_ReducedLength5 ; std::vector<double> _mu_FakeLength5 ;
    std::vector<double>_mu_ReducedLength2 ; std::vector<double> _mu_FakeLength2 ;
    std::vector<double>_mu_ReducedLength1 ; std::vector<double> _mu_FakeLength1 ;

    std::vector<double>_mu_ReducedLengthThirds20; std::vector<double> _mu_FakeLengthThirds20;
    std::vector<double>_mu_ReducedLengthThirds10; std::vector<double> _mu_FakeLengthThirds10;
    std::vector<double>_mu_ReducedLengthThirds5 ; std::vector<double> _mu_FakeLengthThirds5 ;
    std::vector<double>_mu_ReducedLengthThirds2 ; std::vector<double> _mu_FakeLengthThirds2 ;
    std::vector<double>_mu_ReducedLengthThirds1 ; std::vector<double> _mu_FakeLengthThirds1 ;

    std::vector<double>_mu_ReducedLengthHoles20; std::vector<double> _mu_FakeLengthHoles20;
    std::vector<double>_mu_ReducedLengthHoles10; std::vector<double> _mu_FakeLengthHoles10;
    std::vector<double>_mu_ReducedLengthHoles5 ; std::vector<double> _mu_FakeLengthHoles5 ;
    std::vector<double>_mu_ReducedLengthHoles2 ; std::vector<double> _mu_FakeLengthHoles2 ;
    std::vector<double>_mu_ReducedLengthHoles1 ; std::vector<double> _mu_FakeLengthHoles1 ;

    std::vector<double> _mu_En    ;
    std::vector<double> _mu_DepEn ;
    std::vector<double> _mu_Mom   ;
    std::vector<double> _mu_px    ;
    std::vector<double> _mu_py    ;
    std::vector<double> _mu_pz    ;

    std::vector<int>    _mu_Pdg   ;
    std::vector<double> _mu_x     ;
    std::vector<double> _mu_y     ;
    std::vector<double> _mu_z     ;
    std::vector<double> _mu_xEnd  ;
    std::vector<double> _mu_yEnd  ;
    std::vector<double> _mu_zEnd  ;

    std::vector<double> _mu_Refx   ;
    std::vector<double> _mu_Refy   ;
    std::vector<double> _mu_Refz   ;
    std::vector<double> _mu_RefxEnd;
    std::vector<double> _mu_RefyEnd;
    std::vector<double> _mu_RefzEnd;
			       
    std::vector<double> _mu_leng  ;

    std::vector<double> _mu_EnMPS ;
    std::vector<double> _mu_MomMPS;
    std::vector<double> _mu_pxMPS ;
    std::vector<double> _mu_pyMPS ;
    std::vector<double> _mu_pzMPS ;

    // trkf::TrackMomentumCalculator momCalc;

    
  };
}
#endif

/** @} */ // end of doxygen group 
