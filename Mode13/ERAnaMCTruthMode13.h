/**
 * \file ERAnaMCTruthMode13.h
 *
 * \ingroup Mode13
 * 
 * \brief Class def header for a class ERAnaMCTruthMode13
 *
 * @author elenag
 */

/** \addtogroup Mode13

    @{*/

#ifndef ERTOOL_ERANAMCTRUTHMODE13_H
#define ERTOOL_ERANAMCTRUTHMODE13_H

#include "ERTool/Base/AnaBase.h"

#include "ERTool/Base/AnaBase.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoAABox.h"
#include "GeoAlgo/GeoSphere.h"
#include <algorithm> // for std::find
#include <utility>
#include <TTree.h>
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "DataFormat/mctruth.h"
#include "ERTool/Base/AnaBase.h"
#include "ERTool/Base/Particle.h"
#include "ERTool/Base/ParticleGraph.h"
#include "ERTool/Base/EventData.h"


namespace ertool {

  /**
     \class ERAnaMCTruthMode13
     User custom Analysis class made by kazuhiro
   */
  class ERAnaMCTruthMode13 : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaMCTruthMode13(const std::string& name="ERAnaMCTruthMode13");

    /// Default destructor
    virtual ~ERAnaMCTruthMode13(){}

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
    void ClearTree();

  protected:
    // verbose flag
    bool _verbose;

    //Tree -> one entry per simulated proton
    TTree* _anaMC_tree;
    double _MCproton_x     ;
    double _MCproton_y     ;
    double _MCproton_z     ;
    double _MCproton_t     ;
    double _MCproton_px    ;
    double _MCproton_py    ;
    double _MCproton_pz    ;
    int    _MCproton_pdg   ;
    double _MCproton_energy;
    double _MCproton_mass  ;
	      
    double _MCmu_x         ;
    double _MCmu_y         ;
    double _MCmu_z         ;
    double _MCmu_t         ;
    double _MCmu_px        ;
    double _MCmu_py        ;
    double _MCmu_pz        ;
    int    _MCmu_pdg       ;
    double _MCmu_energy    ;
    double _MCmu_mass      ;
    double _MCmu_momentum  ;

    double _MCgamma_x      ;
    double _MCgamma_y      ;
    double _MCgamma_z      ;
    double _MCgamma_t      ;
    double _MCgamma_px     ;
    double _MCgamma_py     ;
    double _MCgamma_pz     ;
    double _MCgamma_energy ;
    int    _MCgamma_pgd    ;
    double _MCgamma_mass   ;
    double _MCgamma_momentum  ;

    double _MCmu_gamma_angle;


  };
}
#endif

/** @} */ // end of doxygen group 
