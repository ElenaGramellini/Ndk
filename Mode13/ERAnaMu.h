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

  private:
    void ClearTree();

  protected:
    // verbose flag
    bool _verbose;

    //Tree -> one entry per simulated proton
    TTree* _anaMuMC_tree;
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

  };
}
#endif

/** @} */ // end of doxygen group 
