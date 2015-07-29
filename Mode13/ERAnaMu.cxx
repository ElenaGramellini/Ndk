#ifndef ERTOOL_ERANAMU_CXX
#define ERTOOL_ERANAMU_CXX

#include "ERAnaMu.h"

namespace ertool {

  ERAnaMu::ERAnaMu(const std::string& name) 
  : AnaBase(name)
  , _anaMuMC_tree(nullptr)
  {
    _verbose = false;
  }

  void ERAnaMu::Reset()
  {}

  void ERAnaMu::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaMu::ProcessBegin()
  {
    
    if (_anaMuMC_tree) { delete _anaMuMC_tree; }
    _anaMuMC_tree = new TTree("_anaMuMC_tree","AnaMC Mode13 Tree");
    _anaMuMC_tree->Branch("_MCmu_x"         ,&_MCmu_x         ,"_MCmu_x/D         ");
    _anaMuMC_tree->Branch("_MCmu_y"         ,&_MCmu_y         ,"_MCmu_y/D         ");
    _anaMuMC_tree->Branch("_MCmu_z"         ,&_MCmu_z         ,"_MCmu_z/D         ");
    _anaMuMC_tree->Branch("_MCmu_t"         ,&_MCmu_t         ,"_MCmu_t/D         ");
    _anaMuMC_tree->Branch("_MCmu_px"        ,&_MCmu_px        ,"_MCmu_px/D        ");
    _anaMuMC_tree->Branch("_MCmu_py"        ,&_MCmu_py        ,"_MCmu_py/D        ");
    _anaMuMC_tree->Branch("_MCmu_pz"        ,&_MCmu_pz        ,"_MCmu_pz/D        ");
    _anaMuMC_tree->Branch("_MCmu_pdg"       ,&_MCmu_pdg       ,"_MCmu_pdg/I       ");
    _anaMuMC_tree->Branch("_MCmu_energy"    ,&_MCmu_energy    ,"_MCmu_energy/D    ");
    _anaMuMC_tree->Branch("_MCmu_mass"      ,&_MCmu_mass      ,"_MCmu_mass/D      ");
    _anaMuMC_tree->Branch("_MCmu_momentum"  ,&_MCmu_momentum  ,"_MCmu_momentum/D  ");

  }

  bool ERAnaMu::Analyze(const EventData &data, const ParticleGraph &ps)
  { 
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MCParticleGraph checks
    auto const& mc_graph = MCParticleGraph();
    auto const& mc_graph_arr = mc_graph.GetParticleArray();
    auto const& mc_data = MCEventData();

    Particle trackParticle;
    Track  thatTrack;

    if (!mc_data.Shower().size()) std::cout<<"No MCShowers"<<"\n";
    if (!mc_data.Track().size()) std::cout<<"No MCTracks"<<"\n";

    if (_verbose) std::cout<<"In ANAMU "<<"\n";
    if (_verbose){
      std::cout<<"########## In MCParticleGraph ########## \n";
      std::cout << "MC Particle Diagram: " << std::endl
		<< mc_graph.Diagram() << std::endl;
      std::cout<<" Graph "<<"\n";
      std::cout<<" Size In ERAnaMCTruth "<<mc_graph_arr.size()<<"\n";
    }
    
    
    if (_verbose) std::cout<<"Number of Tracks: "<<mc_data.Track().size()<<"\n";
    for (auto const& t : mc_graph.GetParticleNodes(RecoType_t::kTrack)){
      thatTrack = mc_data.Track(mc_graph.GetParticle(t).RecoID());
      if (mc_graph.GetParticle(t).PdgCode() == -13) trackParticle = mc_graph.GetParticle(t);
      if (_verbose) { 
	std::cout << "Track:              (" << t << ")" << "\tE: " << thatTrack._energy << std::endl; 
	std::cout << "Track particle:     (" << t << ")" << "\tE: " << trackParticle.Energy() << std::endl; 
      }      
    }
    
    
    _MCmu_x         = trackParticle.Vertex()[0]  ;
    _MCmu_y         = trackParticle.Vertex()[1]  ;
    _MCmu_z         = trackParticle.Vertex()[2]  ;
    _MCmu_px        = trackParticle.Momentum()[0];
    _MCmu_py        = trackParticle.Momentum()[1];
    _MCmu_pz        = trackParticle.Momentum()[2];
    _MCmu_pdg       = trackParticle.PdgCode()    ;    
    _MCmu_energy    = trackParticle.Energy()     ;    
    _MCmu_mass      = trackParticle.Mass()       ;      
    _MCmu_momentum  = sqrt(trackParticle.Momentum()[0]*trackParticle.Momentum()[0] +
			   trackParticle.Momentum()[1]*trackParticle.Momentum()[1] +
			   trackParticle.Momentum()[2]*trackParticle.Momentum()[2] ) ;      

    _anaMuMC_tree->Fill();

    // End of checks on MCParticleGraph
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    return true; 
  }

  void ERAnaMu::ProcessEnd(TFile* fout)
  {
    if(fout){
      fout->cd();
      if (_anaMuMC_tree)
	_anaMuMC_tree->Write();
    }
  }

 void ERAnaMu::ClearTree(){
       
    _MCmu_x         = -1 ;
    _MCmu_y         = -1 ;
    _MCmu_z         = -1 ;
    _MCmu_t         = -1 ;
    _MCmu_px        = -1 ;
    _MCmu_py        = -1 ;
    _MCmu_pz        = -1 ;
    _MCmu_pdg       = -1 ;
    _MCmu_energy    = -1 ;
    _MCmu_mass      = -1 ;
    _MCmu_momentum  = -1 ;      

    return;
  }

}

#endif
