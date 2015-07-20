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
#include "ERTool/Algo/AlgoEMPart.h"

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
    /// Set verbosity
    void setVerbose(bool on){_verbose = on;}
    
  private:
    /// clear tree
    void ClearTree();
    
  protected:
    // verbose flag
    bool _verbose;
    
    //Tree -> one entry per primary proton
    TTree* _ana_tree;

    double _proton_x     ;
    double _proton_y     ;
    double _proton_z     ;
    double _proton_t     ;
    double _proton_px    ;
    double _proton_py    ;
    double _proton_pz    ;
    int    _proton_pdg   ;
    double _proton_energy;
    double _proton_mass  ;

    double _mu_x         ;
    double _mu_y         ;
    double _mu_z         ;
    double _mu_t         ;
    double _mu_px        ;
    double _mu_py        ;
    double _mu_pz        ;
    int    _mu_pdg       ;
    double _mu_energy    ;
    double _mu_mass      ;

    double _gamma_x      ;
    double _gamma_y      ;
    double _gamma_z      ;
    double _gamma_t      ;
    double _gamma_px     ;
    double _gamma_py     ;
    double _gamma_pz     ;
    double _gamma_energy ;
    int    _gamma_pgd    ;
    double _gamma_mass   ;

    double _mu_gamma_angle;

  };
}
#endif

/** @} */ // end of doxygen group 
