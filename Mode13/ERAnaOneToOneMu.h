/**
 * \file ERAnaOneToOneMu.h
 *
 * \ingroup Mode13
 * 
 * \brief Class def header for a class ERAnaOneToOneMu
 *
 * @author elenag
 */

/** \addtogroup Mode13

    @{*/

#ifndef ERTOOL_ERANAONETOONEMU_H
#define ERTOOL_ERANAONETOONEMU_H

#include "ERTool/Base/AnaBase.h"
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
     \class ERAnaOneToOneMu
     User custom Analysis class made by kazuhiro
   */
  class ERAnaOneToOneMu : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaOneToOneMu(const std::string& name="ERAnaOneToOneMu");

    /// Default destructor
    virtual ~ERAnaOneToOneMu(){}

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

    /// set verbosity flag
    void setVerbose(bool on) { _verbose = on; }

  private:
    /// clear tree
    void ClearTree();
  protected:
    // verbosity flag
    bool _verbose;

    TTree* _ana1to1_tree ;
    int n_tracks = 0;
    std::vector<int>    _tracks_Pdg            ;      std::vector<int>    _tracksTru_Pdg            ;
    std::vector<int>    _tracks_size           ;      std::vector<int>    _tracksTru_size           ;
			       		  						
    std::vector<double> _tracks_begEndLength   ;      std::vector<double> _tracksTru_begEndLength   ;
    std::vector<double> _tracks_leng           ;      std::vector<double> _tracksTru_leng           ;
			       		  						
    std::vector<double> _tracks_ReducedLength  ;      std::vector<double> _tracksTru_ReducedLength  ;
    std::vector<double> _tracks_DepEnFromLength;      std::vector<double> _tracksTru_DepEnFromLength;
    std::vector<double> _tracks_DepEn          ;      std::vector<double> _tracksTru_DepEn          ;
    std::vector<double> _tracks_x     ;		      std::vector<double> _tracksTru_x     ;		
    std::vector<double> _tracks_y     ;		      std::vector<double> _tracksTru_y     ;		
    std::vector<double> _tracks_z     ;		      std::vector<double> _tracksTru_z     ;		
    std::vector<double> _tracks_xEnd  ;		      std::vector<double> _tracksTru_xEnd  ;		
    std::vector<double> _tracks_yEnd  ;		      std::vector<double> _tracksTru_yEnd  ;		
    std::vector<double> _tracks_zEnd  ;               std::vector<double> _tracksTru_zEnd  ;         

    std::vector<double> _tracks_DeltaReducedLength  ;
    std::vector<double> _tracks_DeltaDepEnFromLength;
    std::vector<double> _tracks_DeltaDepEn          ;
    std::vector<double> _tracks_Deltax     ;		
    std::vector<double> _tracks_Deltay     ;		
    std::vector<double> _tracks_Deltaz     ;		
    std::vector<double> _tracks_DeltaxEnd  ;		
    std::vector<double> _tracks_DeltayEnd  ;		
    std::vector<double> _tracks_DeltazEnd  ;         
    std::vector<double> _tracks_DeltaLength;
  };
}
#endif

/** @} */ // end of doxygen group 
