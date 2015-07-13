#ifndef ERTOOL_ERANAMODEZERO_CXX
#define ERTOOL_ERANAMODEZERO_CXX

#include "ERAnaModeZero.h"

namespace ertool {

  ERAnaModeZero::ERAnaModeZero(const std::string& name)
  : AnaBase(name)
  , _result_tree(nullptr)
  , fTPC(0.,-115.5,0.,254.8,117.5,1036.92)

  {
    _result_tree = new TTree("_result_tree","Result Tree");

    _h_e_pion = new TH1D("_h_e_pion", "#pi^{0} Energy; Energy (MeV); Count", 60, 0, 600);
    _h_p_pion = new TH1D("_h_p_pion", "#pi^{0} Momentum; Momentum (MeV); Count", 60, 0, 600);
    _h_m_pion = new TH1D("_h_m_pion", "#pi^{0} Mass; Mass (MeV); Count", 60, 0, 600);
    _h_e_eplus = new TH1D("_h_e_eplus", "e^{+} Energy; Energy (MeV); Count", 60, 0, 600);
    _h_p_eplus = new TH1D("_h_p_eplus", "e^{+} Momentum; Momentum (MeV); Count", 60, 0, 600);
    _h_m_eplus = new TH1D("_h_m_eplus", "e^{+} Mass; Mass (MeV); Count", 20, 0, 1);

    _h_e_pion_truth = new TH1D("_h_e_pion_truth", "Truth #pi^{0} Energy; Energy (MeV); Count", 60, 0, 600);
    _h_p_pion_truth = new TH1D("_h_p_pion_truth", "Truth #pi^{0} Momentum; Momentum (MeV); Count", 60, 0, 600);
    _h_m_pion_truth = new TH1D("_h_m_pion_truth", "Truth #pi^{0} Mass; Mass (MeV); Count", 60, 0, 600);
    _h_e_eplus_truth = new TH1D("_h_e_eplus_truth", "Truth e^{+} Energy; Energy (MeV); Count", 60, 0, 600);
    _h_p_eplus_truth = new TH1D("_h_p_eplus_truth", "Truth e^{+} Momentum; Momentum (MeV); Count", 60, 0, 600);
    _h_m_eplus_truth = new TH1D("_h_m_eplus_truth", "Truth e^{+} Mass; Mass (MeV); Count", 20, 0, 1);

    _result_tree->Branch("_n_pion", &_n_pion, "n_pion/I");
    _result_tree->Branch("_e_pion", "vector<double>", &_e_pion);
    _result_tree->Branch("_x_pion", "vector<double>", &_x_pion);
    _result_tree->Branch("_y_pion", "vector<double>", &_y_pion);
    _result_tree->Branch("_z_pion", "vector<double>", &_z_pion);
    _result_tree->Branch("_px_pion", "vector<double>", &_px_pion);
    _result_tree->Branch("_py_pion", "vector<double>", &_py_pion);
    _result_tree->Branch("_pz_pion", "vector<double>", &_pz_pion);
    _result_tree->Branch("_p_pion", "vector<double>", &_p_pion);
    _result_tree->Branch("_m_pion", "vector<double>", &_m_pion); 
    _result_tree->Branch("_n_eplus", &_n_eplus, "n_eplus/I");   
    _result_tree->Branch("_e_eplus", "vector<double>", &_e_eplus);
    _result_tree->Branch("_x_eplus", "vector<double>", &_x_eplus);
    _result_tree->Branch("_y_eplus", "vector<double>", &_y_eplus);
    _result_tree->Branch("_z_eplus", "vector<double>", &_z_eplus);
    _result_tree->Branch("_px_eplus", "vector<double>", &_px_eplus);
    _result_tree->Branch("_py_eplus", "vector<double>", &_py_eplus);
    _result_tree->Branch("_pz_eplus", "vector<double>", &_pz_eplus); 
    _result_tree->Branch("_p_eplus", "vector<double>", &_p_eplus);
    _result_tree->Branch("_m_eplus", "vector<double>", &_m_eplus);

    _result_tree->Branch("_n_pion_truth", &_n_pion_truth, "n_pion_truth/I");
    _result_tree->Branch("_e_pion_truth", "vector<double>", &_e_pion_truth);
    _result_tree->Branch("_x_pion_truth", "vector<double>", &_x_pion_truth);
    _result_tree->Branch("_y_pion_truth", "vector<double>", &_y_pion_truth);
    _result_tree->Branch("_z_pion_truth", "vector<double>", &_z_pion_truth);
    _result_tree->Branch("_px_pion_truth", "vector<double>", &_px_pion_truth);
    _result_tree->Branch("_py_pion_truth", "vector<double>", &_py_pion_truth);
    _result_tree->Branch("_pz_pion_truth", "vector<double>", &_pz_pion_truth);
    _result_tree->Branch("_p_pion_truth", "vector<double>", &_p_pion_truth);
    _result_tree->Branch("_m_pion_truth", "vector<double>", &_m_pion_truth); 
    _result_tree->Branch("_n_eplus_truth", &_n_eplus_truth, "n_eplus_truth/I");   
    _result_tree->Branch("_e_eplus_truth", "vector<double>", &_e_eplus_truth);
    _result_tree->Branch("_x_eplus_truth", "vector<double>", &_x_eplus_truth);
    _result_tree->Branch("_y_eplus_truth", "vector<double>", &_y_eplus_truth);
    _result_tree->Branch("_z_eplus_truth", "vector<double>", &_z_eplus_truth);
    _result_tree->Branch("_px_eplus_truth", "vector<double>", &_px_eplus_truth);
    _result_tree->Branch("_py_eplus_truth", "vector<double>", &_py_eplus_truth);
    _result_tree->Branch("_pz_eplus_truth", "vector<double>", &_pz_eplus_truth); 
    _result_tree->Branch("_p_eplus_truth", "vector<double>", &_p_eplus_truth);
    _result_tree->Branch("_m_eplus_truth", "vector<double>", &_m_eplus_truth);

    // keep track of number of events gone by
    _numEvts = 0;
    _numPions = 0;
    _numPions_truth = 0;
    _numEplus = 0;
    _numEplus_truth = 0;
    _numGamma = 0;
    _numGamma_truth = 0;
    _numPi2eey = 0;
    _missedPions = 0;
    _missedEs = 0;
    _misID_e = 0;
    _misID_gamma = 0;
    _badPions = 0;

    _debug = false;
    _mc = true;

    // set default energy cut (for counting) to 0
    _eCut = 0;

  }

  void ERAnaModeZero::Reset()
  {}

  bool ERAnaModeZero::Analyze(const EventData &data, const ParticleGraph &graph)
  {

    if (_debug)
      std::cout << "******  Begin ERAnaModeZero Analysis  ******" << std::endl;

     // Get particle set
    auto const& graph_arr = graph.GetParticleArray();

    if (_debug){
      std::cout << "Particle Diagram: " << std::endl
      << graph.Diagram() << std::endl;
    }

    // Reset tree variables
    // Assume we will mis-ID
    ResetTreeVariables();

    _numEvts++;


    if (_mc){
    // Get MC EventData (showers/tracks...)
      auto const& mc_data = MCEventData();
      auto const& mc_graph = MCParticleGraph();
      auto const& mc_graph_arr = mc_graph.GetParticleArray();


      if (_debug){
        std::cout << "MCParticleGraph Diagram: " << std::endl
        << mc_graph.Diagram() << std::endl;
      }

    // Here we determine how well we're doing in the event!

    // The following is a hack. Do not apply to non-signal Mode0 samples.

      auto &datshowers = graph.GetParticleNodes(kShower);
      auto &trushowers = mc_graph.GetParticleNodes(kShower);

      _badPion = false;

      for (int i=0; i<datshowers.size(); i++){
        auto& truth_part = mc_graph.GetParticle(trushowers[i]);
        auto& data_part = graph.GetParticle(datshowers[i]);
        auto& truth_id = truth_part.PdgCode();
        auto& data_id = data_part.PdgCode();

        if (data_part.Energy() < _eCut) {continue;}

      // check for a mismatch
        if (abs(truth_id) != abs(data_id)) { 
          if (data_id==22){ _misID_gamma++; }
          if (abs(data_id)==11){ _misID_e++; }

          if (mc_graph.GetParticle(truth_part.Parent()).PdgCode() == 111){
        // This means we're looking at a reconstructed pion
            if (graph.GetParticle(data_part.Parent()).PdgCode() != 111){
              _badPion = true;
            }
          // check for Dalitz decay
            if (mc_graph.GetParticle(truth_part.Parent()).Children().size() > 2){
              _numPi2eey++;
              if (_debug) { 
                auto& sibs = mc_graph.GetParticle(truth_part.Parent()).Children();
                std::cout<<"DALITZ DECAY..."<<std::endl;
                std::cout<<mc_graph.GetParticle(sibs[0]).PdgCode()<<std::endl;
                std::cout<<mc_graph.GetParticle(sibs[1]).PdgCode()<<std::endl;
                std::cout<<mc_graph.GetParticle(sibs[2]).PdgCode()<<std::endl;
              }
              _badPion = true;
            }
          }

        }
      }

      if (_badPion) {_badPions++; }

        // NOW WE SAVE ALL THE TRUTH INFORMATION
      for (auto &p : mc_graph_arr){

      // Find the pion and store its energy
        if ((p.PdgCode() == 111) && (p.Energy() > _eCut)){
          _numPions_truth++;
          _n_pion_truth++;
          _e_pion_truth.push_back(p.Energy());
          _x_pion_truth.push_back(p.Vertex()[0]);
          _y_pion_truth.push_back(p.Vertex()[1]);
          _z_pion_truth.push_back(p.Vertex()[2]);
          _px_pion_truth.push_back(p.Momentum()[0]);
          _py_pion_truth.push_back(p.Momentum()[1]);
          _pz_pion_truth.push_back(p.Momentum()[2]);
          _p_pion_truth.push_back(sqrt(pow(p.Momentum()[0],2) + pow(p.Momentum()[1],2) + pow(p.Momentum()[2],2)));
          _m_pion_truth.push_back(p.Mass());

          _h_e_pion_truth->Fill(p.Energy());
          _h_p_pion_truth->Fill(sqrt(pow(p.Momentum()[0],2) + pow(p.Momentum()[1],2) + pow(p.Momentum()[2],2)));
          _h_m_pion_truth->Fill(p.Mass());


        }

      // Find the e+ and store its energy
        if ((p.PdgCode() == -11) && (p.Energy() > _eCut) && (p.Parent()==0)){
          _numEplus_truth++;
          _n_eplus_truth++;
          _e_eplus_truth.push_back(p.Energy());
          _x_eplus_truth.push_back(p.Vertex()[0]);
          _y_eplus_truth.push_back(p.Vertex()[1]);
          _z_eplus_truth.push_back(p.Vertex()[2]);
          _px_eplus_truth.push_back(p.Momentum()[0]);
          _py_eplus_truth.push_back(p.Momentum()[1]);
          _pz_eplus_truth.push_back(p.Momentum()[2]);
          _p_eplus_truth.push_back(sqrt(pow(p.Momentum()[0],2) + pow(p.Momentum()[1],2) + pow(p.Momentum()[2],2)));
          _m_eplus_truth.push_back(p.Mass());

          _h_e_eplus_truth->Fill(p.Energy());
          _h_p_eplus_truth->Fill(sqrt(pow(p.Momentum()[0],2) + pow(p.Momentum()[1],2) + pow(p.Momentum()[2],2)));
          _h_m_eplus_truth->Fill(p.Mass());


        }
      }

    } // end _mc stuff

    // NOW WE SAVE ALL RECONSTRUCTED INFORMATION

    for (auto &p : graph_arr){

      if ((p.PdgCode() == 22) && (p.Energy() > _eCut)){
        _numGamma++;
      }

      // Find the pion and store its energy
      if ((p.PdgCode() == 111) && (p.Energy() > _eCut)){
        _numPions++;
        _n_pion++;
        _e_pion.push_back(p.Energy());
        _x_pion.push_back(p.Vertex()[0]);
        _y_pion.push_back(p.Vertex()[1]);
        _z_pion.push_back(p.Vertex()[2]);
        _px_pion.push_back(p.Momentum()[0]);
        _py_pion.push_back(p.Momentum()[1]);
        _pz_pion.push_back(p.Momentum()[2]);
        _p_pion.push_back(sqrt(pow(p.Momentum()[0],2) + pow(p.Momentum()[1],2) + pow(p.Momentum()[2],2)));
        _m_pion.push_back(p.Mass());

        _h_e_pion->Fill(p.Energy());
        _h_p_pion->Fill(sqrt(pow(p.Momentum()[0],2) + pow(p.Momentum()[1],2) + pow(p.Momentum()[2],2)));
        _h_m_pion->Fill(p.Mass());


      }

      // Find the e+ and store its energy
      if ((abs(p.PdgCode()) == 11) && (p.Energy() > _eCut)){
        _numEplus++;
        _n_eplus++;
        _e_eplus.push_back(p.Energy());
        _x_eplus.push_back(p.Vertex()[0]);
        _y_eplus.push_back(p.Vertex()[1]);
        _z_eplus.push_back(p.Vertex()[2]);
        _px_eplus.push_back(p.Momentum()[0]);
        _py_eplus.push_back(p.Momentum()[1]);
        _pz_eplus.push_back(p.Momentum()[2]);
        _p_eplus.push_back(sqrt(pow(p.Momentum()[0],2) + pow(p.Momentum()[1],2) + pow(p.Momentum()[2],2)));
        _m_eplus.push_back(p.Mass());

        _h_e_eplus->Fill(p.Energy());
        _h_p_eplus->Fill(sqrt(pow(p.Momentum()[0],2) + pow(p.Momentum()[1],2) + pow(p.Momentum()[2],2)));
        _h_m_eplus->Fill(p.Mass());

       
      }
    }



    // fill the tree
    _result_tree->Fill();

    return true;
  }
  
  void ERAnaModeZero::ProcessEnd(TFile* fout)
  {
    
    std::cout << "RESULTS: " << std::endl << std::endl
    << "Reconstructed Pions: " << _numPions << std::endl;
    if (_mc) { std::cout<< "Correctly IDed Pions: " << (_numPions-_badPions) << std::endl 
    << "Total Truth Pions: " << _numPions_truth << std::endl << std::endl; }
    std::cout<< "Reconstructed Electrons: " << _numEplus << std::endl;
    if (_mc) { std::cout<< "Correctly IDed Es: " << (_numEplus - _misID_e) << std::endl 
    << "Total Truth Es: " << _numEplus_truth << std::endl << std::endl
    << "Correctly IDed Photons: " << (_numGamma - _misID_gamma) << std::endl
    << "Dalitz decays (included in 'badPion' def): " << _numPi2eey << std::endl; }

    if (fout){
      fout->cd();
      _result_tree->Write();
      _h_e_pion->Write();
      _h_p_pion->Write();
      _h_m_pion->Write();
      _h_e_eplus->Write();
      _h_p_eplus->Write();
      _h_m_eplus->Write();
      if (_mc){
      _h_e_pion_truth->Write();
      _h_p_pion_truth->Write();
      _h_m_pion_truth->Write();
      _h_e_eplus_truth->Write();
      _h_p_eplus_truth->Write();
      _h_m_eplus_truth->Write();
    }
    }

    return;
  }


  void ERAnaModeZero::MakeEffPlot(std::string varname,
   int nbins, double xmin, double xmax){

    // Efficiency vs. variable "varname"
    int Nall, Nok;
    TH1D *hist = new TH1D(Form("_hEffvs%s",varname.c_str()),Form("Efficiency vs %s",varname.c_str()),nbins,xmin,xmax);
    TH1D *All = new TH1D("All","All",nbins,xmin,xmax); // Histogram of all entries as a function of Elep
    TH1D *Ok = new TH1D("Ok","Ok",nbins,xmin,xmax);  // Histogram of all entries that are not mis-ID vs Elep
    Nall = _result_tree->Draw(Form("%s>>All",varname.c_str()),"");
    Nok = _result_tree->Draw(Form("%s>>Ok",varname.c_str()),"misID==0");
    for (int n=0; n < nbins; n++){
      if (All->GetBinContent(n+1) > 0){
       double eff = Ok->GetBinContent(n+1)/All->GetBinContent(n+1);
       hist->SetBinContent(n+1, eff);
       hist->SetBinError(n+1, sqrt( eff*(1-eff) / All->GetBinContent(n+1) ) );
     }
     else{
       hist->SetBinContent(n+1,0);
       hist->SetBinError(n+1,0.);
     }
   }
   delete All;
   delete Ok;
   hist->Write();

   return;
 }

 void ERAnaModeZero::ResetTreeVariables(){

  _n_pion = 0; _n_pion_truth = 0;
  _n_eplus = 0; _n_eplus_truth = 0;
  _e_pion.clear(); _p_pion.clear(); _m_pion.clear();
  _x_pion.clear(); _y_pion.clear(); _z_pion.clear();
  _px_pion.clear(); _py_pion.clear(); _pz_pion.clear();
  _e_eplus.clear(); _p_eplus.clear(); _m_eplus.clear();
  _x_eplus.clear(); _y_eplus.clear(); _z_eplus.clear();
  _px_eplus.clear(); _py_eplus.clear(); _pz_eplus.clear();
  _e_pion_truth.clear(); _p_pion_truth.clear(); _m_pion_truth.clear();
  _x_pion_truth.clear(); _y_pion_truth.clear(); _z_pion_truth.clear();
  _px_pion_truth.clear(); _py_pion_truth.clear(); _pz_pion_truth.clear();
  _e_eplus_truth.clear(); _p_eplus_truth.clear(); _m_eplus_truth.clear();
  _x_eplus_truth.clear(); _y_eplus_truth.clear(); _z_eplus_truth.clear();
  _px_eplus_truth.clear(); _py_eplus_truth.clear(); _pz_eplus_truth.clear();

  return;
}

}

#endif
