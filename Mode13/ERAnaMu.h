/**
 * \file ERAnaMu.h
 *
 * \ingroup Mode13
 * 
 * \brief Class def header for a class ERAnaMu
 *
 * @author elenag
 */

/** \addtogroup Mode13

    @{*/

#ifndef ERTOOL_ERANAMU_H
#define ERTOOL_ERANAMU_H

#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "GeoAlgo/GeoAlgo.h"
#include "DataFormat/mctruth.h"
#include "ERTool/Base/AnaBase.h"
#include "ERTool/Base/Particle.h"
#include "ERTool/Base/ParticleGraph.h"
#include "ERTool/Base/EventData.h"



namespace ertool {

  /**
     \class ERAnaMu
     User custom Analysis class made by kazuhiro
   */
  class ERAnaMu : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaMu(const std::string& name="ERAnaMu");

    /// Default destructor
    virtual ~ERAnaMu(){}

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


  protected:
    
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

    // verbosity flag
    bool _verbose;
    
    // Counters : 
    // True Muons
    // ParticleGraph Muons
    // Contained True Muons
    // Contained ParticleGraph Muons
    int MCMu        = 0;
    int PartGraphMu = 0;
    int Cont_MCMu        = 0;
    int Cont_PartGraphMu = 0;
    

  };
}
#endif

/** @} */ // end of doxygen group 
