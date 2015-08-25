/**
 * \file ERAnaOneToOneGamma.h
 *
 * \ingroup Mode13
 * 
 * \brief Class def header for a class ERAnaOneToOneGamma
 *
 * @author elenag
 */

/** \addtogroup Mode13

    @{*/

#ifndef ERTOOL_ERANAONETOONEGAMMA_H
#define ERTOOL_ERANAONETOONEGAMMA_H


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



namespace ertool {
  /**
     \class ERAnaOneToOneGamma
     User custom Analysis class made by kazuhiro
  */
  class ERAnaOneToOneGamma : public AnaBase {
  public:

    /// Default constructor
    ERAnaOneToOneGamma(const std::string& name="ERAnaOneToOneGamma");
    
    /// Default destructor
    virtual ~ERAnaOneToOneGamma(){}
    

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
    /// Function to check wether a shower is e- or gamma-like
    /// Returns true if gamma-like
    bool isGammaLike(const double dedx, double radlen,bool forceRadLen=false);
    
  private:
    /// clear tree
    void ClearTree();
  protected:
    // verbosity flag
    bool _verbose;

    // flag to decide whether to use EMPart or not
    // if True -> use radiation length to calculate LL
    // if False -> use only dEdx
    bool _useRadLength;
    
    TTree* _ana1to1_tree ;
    int n_shower = 0;
    int _shower_size = 0;      int    _showerTru_size= 0;

    std::vector<int>    _shower_Pdg            ;      std::vector<int>    _showerTru_Pdg            ;
    std::vector<double> _shower_leng           ;      std::vector<double> _showerTru_leng           ;			       		  						
    std::vector<double> _shower_DepEn          ;      std::vector<double> _showerTru_DepEn          ;
    std::vector<double> _shower_x     ;		      std::vector<double> _showerTru_x     ;		
    std::vector<double> _shower_y     ;		      std::vector<double> _showerTru_y     ;		
    std::vector<double> _shower_z     ;		      std::vector<double> _showerTru_z     ;		


    std::vector<double> _shower_DeltaDepEn ;
    std::vector<double> _shower_Deltax     ;		
    std::vector<double> _shower_Deltay     ;		
    std::vector<double> _shower_Deltaz     ;		
    std::vector<double> _shower_DeltaLength;
    // Other algorithms to use
    AlgoEMPart _alg_emp;
    AlgoFindRelationship _findRel;
    //Tree -> one entry for every time EMPart LL function
    // is called using both dEdx and rad-length
    TTree* _empart_tree;
    double _dedx;
    double _radlen;
    int    _pdg;
  };
}
#endif

/** @} */ // end of doxygen group 
