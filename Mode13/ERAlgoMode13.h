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
#include "ERTool/Algo/AlgoEMPart.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoAABox.h"
#include "GeoAlgo/GeoSphere.h"
#include <algorithm> // for std::find
#include <utility>
#include <TTree.h>


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
    virtual ~ERAlgoMode13(){}; //Ele maybe "virtual" has to come off
    
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

    /// Set verbosity
    void setVerbose(bool on){
      _verbose = on;
      _findRel.setDebug(on);
    }
    /// Use EMPart
    void useRadLength(bool on) { _useRadLength = on; }

    void setRejectLongTracks(bool on) { _rejectLongTracks = on; }
    void setVtxToTrkStartDist(double d){ _vtxToTrkStartDist = d; }
    void setVtxToTrkDist(double d){ _vtxToTrkDist = d; }
    void setVtxToShrStartDist(double d){ _vtxToShrStartDist = d; }
    void setMaxIP(double d){ _maxIP = d; }
    void setEThreshold(double E){ _Ethreshold = E; }
    void setVtxProximityCut(double d) { _vtxProximityCut = d; }
    void setBDtW(double b) { _BDtW = b; }
    void setBDtTW(double bt) { _BDtTW = bt; }
    
    
  private:
    
    ::geoalgo::AABox fTPC;
    
    /// clear tree
    void ClearTree();
    
  protected:
    
    
    /// Function to check wether a shower is e- or gamma-like
    /// Returns true if gamma-like
    bool isGammaLike(const double dedx, double radlen,bool forceRadLen=false);
    
    // verbose flag
    bool _verbose;
    // electron mass
    double _e_mass;
    // photon mass
    double _gamma_mass;
    // muon mass
    double _mu_mass;

    // flag to decide whether to use EMPart or not
    // if True -> use radiation length to calculate LL
    // if False -> use only dEdx
    bool _useRadLength;
    
    // Energy thershold. If shower has < _Ethreshold -> don't consider
    double _Ethreshold;
    
    // Reject events with long primary tracks
    bool _rejectLongTracks;
    
    // constants to be used for decision-making
    // minimum distance that a reco-vertex must be away from the
    // start of a track for the vtx to be considered "on the track"
    // and the shower to come from the track, instead of the shower
    // and track being siblings.
    double _vtxToTrkStartDist = 9999999.;
    // maximum distance from the entire track for the reco-vertex
    // for the vertex to be considered on the track and the shower
    // to be considered as coming from the track
    double _vtxToTrkDist = 9999999.;
    // Max distance from shower start that the vertex can be
    // this value should be a few radiation lengths: the cut 
    // is placed to remove cases where the shower is meters
    // away from the vertex. Probably an accidental correlation
    double _vtxToShrStartDist = 9999999.;
    // Maximum impact parameter allowed between the two objects
    // if larger it means that the two lines/segments do not come
    // within this value at any point and they therefore are 
    // assumed to not be correlated
    double _maxIP;
    // Vertex Proximity Cut:
    // if "candidate vertices" from the tracks are found
    // (needs to be a vertex in common to 2+ tracks in event)
    // require that a shower be within this distance to the
    // closest candidate vertex
    double _vtxProximityCut;
    // Backwards Distance to Wall Cut:
    // if "mode13" vertex is near to wall, it is likely due to
    // cosmic.  This becomes more important cut with small/no fid
    // volume cut. Default set off. 
    double _BDtW ;
    // Backwards Distance to Top Wall Cut:
    // if "mode13" vertex is extended backwards along its
    // trajectory, when does it hit the top wall of TPC (assuming
    // top wall is extended to infinity)? This param will be
    // negative for upwards facing showers.
    double _BDtTW ;
    // Keep track of number of neutrinos found
    int _protonsdKs;
    // Keep track of whether a sister track to the shower has
    // been found
    bool _hassister;

    // Sets of cuts, probably this is not the final location of these cuts
    // Topologycal cuts
    double _EnDepDistanceMaxCut =  99999999.;
    double _IPMaxCut            =  99999999.;
    double _OpeningAngleMinCut  =  0.;
    
    // Calorimetry loose cuts
    double _MuonEnergyMaxCut    =  99999999.;
    double _GammaEnergyMaxCut   =  99999999.;
    double _TotalEnergyMaxCut   =  99999999.;
    double _MuonEnergyMinCut    =  0.;
    double _GammaEnergyMinCut   =  0.;
    double _TotalEnergyMinCut   =  0.;
    double _TotMomXCutMax       =  99999999.;
    double _TotMomYCutMax       =  99999999.;
    double _TotMomZCutMax       =  99999999.;
    double _TotMomXCutMin       = -99999999.;
    double _TotMomYCutMin       = -99999999.;
    double _TotMomZCutMin       = -99999999.;

    // Counters for the variables on which we cut
    int _cEvt          ;//Count tot number of events
    int _cOnePlusTrack ;//Count events that have at least 1 track
    int _cOnePlusShower;//Count events that have at least 1 shower
    int _cEnDepRadius  ;//Count events that have at least 1 track, 1 shower and  pass the radius cut
    int _cIP           ;//Count events that have at least 1 track, 1 shower and  pass the IP cut
    int _cRedundant    ;
    int _cOnePlusGamma ;//Count events for which the shower is a gamma
    int _cOnePlusMu    ;//Count events for which the track  is a muon
    int _cNoVtxAct     ;//Count events for there is no other vertex activity
    int _cOpeningAngle ;//Count events that pass the opening angle cut
    int _cMuonEnergy   ;//Count events that pass the  Muon Energy cut
    int _cGammaEnergy  ;//Count events that pass the Gamma Energy cut
    int _cTotalEnergy  ;//Count events that pass the Total Energy cut
    int _cTotMom       ;//Count events that pass the Total Energy cut

    double openingAngle ;    
    
    // Other algorithms to use
    AlgoEMPart _alg_emp;
    AlgoFindRelationship _findRel;
    // GeoAlgo Tool
    ::geoalgo::GeoAlgo _geoAlgo;

    //Tree -> one entry for every time EMPart LL function
    // is called using both dEdx and rad-length
    TTree* _empart_tree;
    double _dedx;
    double _radlen;
    int    _pdg;
    
    //Tree -> one entry per shower-other comparison
    // therefore possibly multiple entries for each shower
    TTree* _alg_tree;
    double _E; // energy of shower
    int    _PDG; // PDG code assigned by AlgoEMPart
    int    _VsTrack; // comparing vs track (==1)
    double _thatE; // Energy of other shower/track
    double _dEdx;
    double _IP; // Impact Paramter with other object
    double _IPthisStart; // distance from IP to this shower start point
    double _IPthatStart; // distance from IP to that shower/track start point
    double _IPtrkBody; // distance from IP to body of track (if comparing with track)
    double _distBackAlongTraj; // distance backwards from vertex to nearest wall
    double _distToTopWall; // distance backwards along traj from vertex to top wall(extended to infinity)
    int _nTrack        ;//Count number tracks
    int _nShower       ;//Count number tracks
    int _nMu           ;//Count number muons
    int _nGamma        ;//Count number gammas
    double _EnDepDist  ; // distance between the two first energy depositions


    
  };
}
#endif




