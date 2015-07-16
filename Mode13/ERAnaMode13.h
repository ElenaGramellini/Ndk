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
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "GeoAlgo/GeoAlgo.h"
#include "DataFormat/mctruth.h"
#include "ERTool/Base/AnaBase.h"
#include "ERTool/Base/Particle.h"
#include "ERTool/Base/ParticleGraph.h"
#include "ERTool/Base/EventData.h"
#include "ERTool/Algo/AlgoEMPart.h"


//#include <algorithm> // used for std::find




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

    void InitializeHistos();

    void Finalize();

    bool isGammaLike(const double dedx, double radlen, bool forceRadLen = false);
    const geoalgo::Vector gammaEndpoint(const ertool::Shower shower);
    
    // histograms to be filled in Algo
    // Muons 
    TH1F* mu_energy;
    TH1F* mu_px    ;
    TH1F* mu_py    ;
    TH1F* mu_pz    ;
    TH1F* mu_pid   ;
    TH1F* mu_x     ;
    TH1F* mu_y     ;
    TH1F* mu_z     ;
    TH1F* mu_t     ;

    TH1F* muTru_energy;
    TH1F* muTru_px    ;
    TH1F* muTru_py    ;
    TH1F* muTru_pz    ;
    TH1F* muTru_pid   ;
    TH1F* muTru_x     ;
    TH1F* muTru_y     ;
    TH1F* muTru_z     ;
    TH1F* muTru_t     ;

    
    // Counters : 
    // True Muons
    // ParticleGraph Muons
    // Contained True Muons
    // Contained ParticleGraph Muons
    int MCMu        = 0;
    int PartGraphMu = 0;
    int Cont_MCMu        = 0;
    int Cont_PartGraphMu = 0;

 // histograms to be filled in Algo
    // Gammas
    TH1F* gamma_energy;
    TH1F* gamma_px    ;
    TH1F* gamma_py    ;
    TH1F* gamma_pz    ;
    TH1F* gamma_pid   ;
    TH1F* gamma_x     ;
    TH1F* gamma_y     ;
    TH1F* gamma_z     ;
    TH1F* gamma_t     ;

    TH1F* gammaTru_energy;
    TH1F* gammaTru_px    ;
    TH1F* gammaTru_py    ;
    TH1F* gammaTru_pz    ;
    TH1F* gammaTru_pid   ;
    TH1F* gammaTru_x     ;
    TH1F* gammaTru_y     ;
    TH1F* gammaTru_z     ;
    TH1F* gammaTru_t     ;

    // verbosity flag
    bool _verbose;
    
    // Counters : 
    // True Gamma
    // ParticleGraph Gamma
    // Contained True Gamma
    // Contained ParticleGraph Gamma
    int MCGamma        = 0;
    int PartGraphGamma = 0;
    int Cont_MCGamma        = 0;
    int Cont_PartGraphGamma = 0;
   

    // if True -> use radiation length to calculate LL
    // if False -> use only dEdx
    bool _useRadLength;

    // Other algorithms to use
    AlgoEMPart _alg_emp;
    

  };
}
#endif

/** @} */ // end of doxygen group 
