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

  private:
    /// clear tree
    void ClearTree();
    /// Reserve the local vector size prior to push-back
    void ReserveVector(const size_t ntracks, const size_t nshowers);

  protected:
    
    bool _verbose;
    bool _useRadLength;
    bool isGammaLike(const double dedx, double radlen,bool forceRadLen=false);
    AlgoEMPart _alg_emp;
    AlgoFindRelationship _findRel;

    TTree* _ProtonSel_tree ;
    int _run_n       = 0 ;
    int _subrun_n    = 0 ;
    int _evt_n       = 0 ;

    int n_tracks = 0;
    int n_mu = 0;
    //Track Length
    std::vector<int>    _tracks_size         ;
    std::vector<double> _track_begEndLength  ;
    std::vector<double> _track_leng          ;
    std::vector<double> _track_lengthRatio   ;
    std::vector<double> _track_ReducedLength ; 
    std::vector<double> _track_FakeLength    ;
    std::vector<double> _tracks_stepsLength  ;
    std::vector<double> _tracks_stepsDensity ;
    //Track Calorimetry & PID
    std::vector<int>    _track_Pdg       ;
    std::vector<double> _track_DepEnCal  ;
    std::vector<double> _track_DepEnRange;
    std::vector<double> _track_EnCal     ;
    std::vector<double> _track_EnRange   ;
    std::vector<double> _track_Mom       ;
    std::vector<double> _track_px        ;
    std::vector<double> _track_py        ;
    std::vector<double> _track_pz        ;
    std::vector<double> _track_EnMPS     ;
    std::vector<double> _track_MomMPS    ;
    std::vector<double> _track_pxMPS     ;
    std::vector<double> _track_pyMPS     ;
    std::vector<double> _track_pzMPS     ;
    //Track Position
    std::vector<double> _track_x     ;
    std::vector<double> _track_y     ;
    std::vector<double> _track_z     ;
    std::vector<double> _track_xEnd  ;
    std::vector<double> _track_yEnd  ;
    std::vector<double> _track_zEnd  ;


    int n_showers = 0;
    int n_gamma = 0;
    //Shower Calorimetry & PID
    std::vector<int>    _shower_Pdg       ;
    std::vector<double> _shower_DepEn     ;
    std::vector<double> _shower_DeDx      ;
    std::vector<double> _shower_Mom       ;
    std::vector<double> _shower_px        ;
    std::vector<double> _shower_py        ;
    std::vector<double> _shower_pz        ;
    //Shower Position
    std::vector<double> _shower_x     ;
    std::vector<double> _shower_y     ;
    std::vector<double> _shower_z     ;
    std::vector<double> _shower_radius;
    std::vector<double> _shower_lenght;

    int n_Protons = 0;
    //Proton Calorimetry & PID
    std::vector<int>    _proton_Pdg   ;
    std::vector<double> _proton_En    ;
    std::vector<double> _proton_Mom   ;
    std::vector<double> _proton_px    ;
    std::vector<double> _proton_py    ;
    std::vector<double> _proton_pz    ;
    //Proton Position
    std::vector<double> _proton_x     ;
    std::vector<double> _proton_y     ;
    std::vector<double> _proton_z     ;
    //Properties of the couple
    std::vector<double> _proton_openingAngle ;
    std::vector<double> _proton_IP           ;
    std::vector<double> _protonEnDepDistance ;    


  };
}
#endif

/** @} */ // end of doxygen group 
