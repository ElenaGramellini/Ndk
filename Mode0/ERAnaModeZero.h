#ifndef ERTOOL_ERANAMODEZERO_H
#define ERTOOL_ERANAMODEZERO_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "GeoAlgo/GeoAlgo.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "ERTool/Base/AlgoBase.h"

namespace ertool {

  /**
     \class ERAnaModeZero
     User custom Analysis class made by david caratelli
   */
  class ERAnaModeZero : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaModeZero(const std::string& name="ERAnaModeZero");

    /// Default destructor
    virtual ~ERAnaModeZero(){};

    /// Accept Pset
    void AcceptPSet(const ::fcllite::PSet& cfg) {}

    /// Reset function
    virtual void Reset();

    /// Function to evaluate input showers and determine a score
    virtual bool Analyze(const EventData &data, const ParticleGraph &graph);

    void SetDebug(bool on) { _debug = on; }

    void SetMC(bool on) { _mc = on; }

    virtual void ProcessBegin() {}

    virtual void ProcessEnd(TFile* fout);

    /// Function to re-set TTree variables
    void ResetTreeVariables();

    /// Function to save efficiency vs. variable to histogram
    void MakeEffPlot(std::string varname,
		     int nbins, double xmin, double xmax);

    /// get reported efficiency
    double getEfficiency() { return _eff; }

    /// set the energy cut to be used when counting particles
    void SetECut(double c) { _eCut = c; }

    // /// set tree name
    // void SetTreeName(std::string tree_name) { _tree_name = tree_name };

  private:

    // debug flag
    bool _debug;
    // do we want MC comparison?
    bool _mc;

    // // tree name
    // std::string _tree_name = "_result_tree";

    // energy cut for counting particles
    float _eCut;

    // efficiency
    double _eff = 1;

    // Result tree comparison for reconstructed events
    TTree* _result_tree;
    
    TH1D* _h_e_pion; 
    TH1D* _h_p_pion;
    TH1D* _h_m_pion;
    TH1D* _h_e_eplus;
    TH1D* _h_p_eplus;
    TH1D* _h_m_eplus;
    TH1D* _h_angle;

    TH1D* _h_e_pion_truth; 
    TH1D* _h_p_pion_truth;
    TH1D* _h_m_pion_truth;
    TH1D* _h_e_eplus_truth;
    TH1D* _h_p_eplus_truth;
    TH1D* _h_m_eplus_truth;
    TH1D* _h_angle_truth;

    int _n_pion;
    int _n_eplus;
    std::vector<double> _e_pion, _p_pion, _m_pion;
    std::vector<double> _x_pion, _y_pion, _z_pion;
    std::vector<double> _px_pion, _py_pion, _pz_pion;
    std::vector<double> _e_eplus, _p_eplus, _m_eplus;
    std::vector<double> _x_eplus, _y_eplus, _z_eplus;
    std::vector<double> _px_eplus, _py_eplus, _pz_eplus;
    std::vector<double> _x_ph1, _y_ph1, _z_ph1;
    std::vector<double> _x_ph2, _y_ph2, _z_ph2;
    std::vector<double> _angle;

    int _n_pion_truth;
    int _n_eplus_truth;
    std::vector<double> _e_pion_truth, _p_pion_truth, _m_pion_truth;
    std::vector<double> _x_pion_truth, _y_pion_truth, _z_pion_truth;
    std::vector<double> _px_pion_truth, _py_pion_truth, _pz_pion_truth;
    std::vector<double> _e_eplus_truth, _p_eplus_truth, _m_eplus_truth;
    std::vector<double> _x_eplus_truth, _y_eplus_truth, _z_eplus_truth;
    std::vector<double> _px_eplus_truth, _py_eplus_truth, _pz_eplus_truth;
    std::vector<double> _x_ph1_truth, _y_ph1_truth, _z_ph1_truth;
    std::vector<double> _x_ph2_truth, _y_ph2_truth, _z_ph2_truth;
    std::vector<double> _angle_truth;
    int _correctlyIDed;

    /// counters to cout to screen after running
    int _numEvts;
    int _numPions, _numEplus, _numGamma;
    int _numPions_truth, _numEplus_truth, _numGamma_truth;
    int _missedEs, _missedPions;
    int _numPi2eey;
    int _misID_e, _misID_gamma, _badPions;

    bool _badPion;

    ::geoalgo::GeoAlgo _geoAlgo;
    ::geoalgo::AABox fTPC;
    AlgoFindRelationship _findRel;
   
  };
}
#endif

/** @} */ // end of doxygen group 
