#ifndef ERTOOL_ERANAMCTRUTHMODE13_CXX
#define ERTOOL_ERANAMCTRUTHMODE13_CXX

#include "ERAnaMCTruthMode13.h"

namespace ertool {

  ERAnaMCTruthMode13::ERAnaMCTruthMode13(const std::string& name) 
  : AnaBase(name)
  , _anaMC_tree(nullptr)
  {
    _verbose = true;
  }

  void ERAnaMCTruthMode13::Reset()
  {}

  void ERAnaMCTruthMode13::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaMCTruthMode13::ProcessBegin()
  {
    
    if (_anaMC_tree) { delete _anaMC_tree; }
    _anaMC_tree = new TTree("_anaMC_tree","AnaMC Mode13 Tree");

    _anaMC_tree->Branch("_MCproton_x"     ,&_MCproton_x     ,"_MCproton_x/D     ");
    _anaMC_tree->Branch("_MCproton_y"     ,&_MCproton_y     ,"_MCproton_y/D     ");
    _anaMC_tree->Branch("_MCproton_z"     ,&_MCproton_z     ,"_MCproton_z/D     ");
    _anaMC_tree->Branch("_MCproton_t"     ,&_MCproton_t     ,"_MCproton_t/D     ");
    _anaMC_tree->Branch("_MCproton_px"    ,&_MCproton_px    ,"_MCproton_px/D    ");
    _anaMC_tree->Branch("_MCproton_py"    ,&_MCproton_py    ,"_MCproton_py/D    ");
    _anaMC_tree->Branch("_MCproton_pz"    ,&_MCproton_pz    ,"_MCproton_pz/D    ");
    _anaMC_tree->Branch("_MCproton_pdg"   ,&_MCproton_pdg   ,"_MCproton_pdg/I   ");
    _anaMC_tree->Branch("_MCproton_energy",&_MCproton_energy,"_MCproton_energy/D");
    _anaMC_tree->Branch("_MCproton_mass"  ,&_MCproton_mass  ,"_MCproton_mass/D  ");
		                                                          
    _anaMC_tree->Branch("_MCmu_x"         ,&_MCmu_x         ,"_MCmu_x/D         ");
    _anaMC_tree->Branch("_MCmu_y"         ,&_MCmu_y         ,"_MCmu_y/D         ");
    _anaMC_tree->Branch("_MCmu_z"         ,&_MCmu_z         ,"_MCmu_z/D         ");
    _anaMC_tree->Branch("_MCmu_t"         ,&_MCmu_t         ,"_MCmu_t/D         ");
    _anaMC_tree->Branch("_MCmu_px"        ,&_MCmu_px        ,"_MCmu_px/D        ");
    _anaMC_tree->Branch("_MCmu_py"        ,&_MCmu_py        ,"_MCmu_py/D        ");
    _anaMC_tree->Branch("_MCmu_pz"        ,&_MCmu_pz        ,"_MCmu_pz/D        ");
    _anaMC_tree->Branch("_MCmu_pdg"       ,&_MCmu_pdg       ,"_MCmu_pdg/I       ");
    _anaMC_tree->Branch("_MCmu_energy"    ,&_MCmu_energy    ,"_MCmu_energy/D    ");
    _anaMC_tree->Branch("_MCmu_mass"      ,&_MCmu_mass      ,"_MCmu_mass/D      ");
		                                                          
    _anaMC_tree->Branch("_MCgamma_x"      ,&_MCgamma_x      ,"_MCgamma_x/D      ");
    _anaMC_tree->Branch("_MCgamma_y"      ,&_MCgamma_y      ,"_MCgamma_y/D      ");
    _anaMC_tree->Branch("_MCgamma_z"      ,&_MCgamma_z      ,"_MCgamma_z/D      ");
    _anaMC_tree->Branch("_MCgamma_t"      ,&_MCgamma_t      ,"_MCgamma_t/D      ");
    _anaMC_tree->Branch("_MCgamma_px"     ,&_MCgamma_px     ,"_MCgamma_px/D     ");
    _anaMC_tree->Branch("_MCgamma_py"     ,&_MCgamma_py     ,"_MCgamma_py/D     ");
    _anaMC_tree->Branch("_MCgamma_pz"     ,&_MCgamma_pz     ,"_MCgamma_pz/D     ");
    _anaMC_tree->Branch("_MCgamma_energy" ,&_MCgamma_energy ,"_MCgamma_energy/D ");
    _anaMC_tree->Branch("_MCgamma_pgd"    ,&_MCgamma_pgd    ,"_MCgamma_pgd/I    ");
    _anaMC_tree->Branch("_MCgamma_mass"   ,&_MCgamma_mass   ,"_MCgamma_mass/D   ");
			    		       		        
    _anaMC_tree->Branch("_MCmu_gamma_angle",&_MCmu_gamma_angle,"_MCmu_gamma_angle/D");
  }

  bool ERAnaMCTruthMode13::Analyze(const EventData &data, const ParticleGraph &ps)
  { 
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MCParticleGraph checks
    auto const& mc_graph = MCParticleGraph();
    auto const& mc_graph_arr = mc_graph.GetParticleArray();
    auto const& mc_data = MCEventData();

    Particle showerParticle;
    Particle trackParticle;
    Track  thatTrack;
    Shower thisShower;

    if (_verbose){
      std::cout<<"########## In MCParticleGraph ########## \n";
      std::cout << "MC Particle Diagram: " << std::endl
		<< mc_graph.Diagram() << std::endl;
      std::cout<<" Graph "<<"\n";
      std::cout<<" Size In ERAnaMCTruth "<<mc_graph_arr.size()<<"\n";
    }
    

    if (_verbose) std::cout<<"Number of showers: "<<mc_data.Shower().size()<<"\n";
    for (auto const& p : mc_graph.GetParticleNodes(RecoType_t::kShower)){
      
      thisShower = mc_data.Shower(mc_graph.GetParticle(p).RecoID());
      showerParticle = mc_graph.GetParticle(p);
      
      if (_verbose) { 
	std::cout << "Shower:             (" << p << ")" << "\tE: " << thisShower._energy << std::endl; 
	std::cout << "Shower particle:    (" << p << ")" << "\tE: " << showerParticle.Energy() << std::endl; }     
    }
    
    
    if (_verbose) std::cout<<"Number of Tracks: "<<mc_data.Track().size()<<"\n";
    for (auto const& t : mc_graph.GetParticleNodes(RecoType_t::kTrack)){
      thatTrack = mc_data.Track(mc_graph.GetParticle(t).RecoID());
      trackParticle = mc_graph.GetParticle(t);
      if (_verbose) { 
	std::cout << "Track:              (" << t << ")" << "\tE: " << thatTrack._energy << std::endl; 
	std::cout << "Track particle:     (" << t << ")" << "\tE: " << trackParticle.Energy() << std::endl; }      
    }
    
    if ((trackParticle.PdgCode() == -13)&&(showerParticle.PdgCode() == 22)){


      _MCgamma_x      =  showerParticle.Vertex()[0]  ;
      _MCgamma_y      =  showerParticle.Vertex()[1]  ;
      _MCgamma_z      =  showerParticle.Vertex()[2]  ;
      //      _MCgamma_t      =  DO WE HAVE THIS INFO???
      _MCgamma_px     =  showerParticle.Momentum()[0];
      _MCgamma_py     =  showerParticle.Momentum()[1];
      _MCgamma_pz     =  showerParticle.Momentum()[2];
      _MCgamma_energy =  showerParticle.PdgCode()    ;
      _MCgamma_pgd    =  showerParticle.Energy()     ;
      _MCgamma_mass   =  showerParticle.Mass()       ;


      _MCmu_x         = trackParticle.Vertex()[0]  ;
      _MCmu_y         = trackParticle.Vertex()[1]  ;
      _MCmu_z         = trackParticle.Vertex()[2]  ;
      //_MCmu_t         = ; DO WE HAVE THIS INFO???
      _MCmu_px        = trackParticle.Momentum()[0];
      _MCmu_py        = trackParticle.Momentum()[1];
      _MCmu_pz        = trackParticle.Momentum()[2];
      _MCmu_pdg       = trackParticle.PdgCode()    ;    
      _MCmu_energy    = trackParticle.Energy()     ;    
      _MCmu_mass      = trackParticle.Mass()       ;      
      


      _MCmu_gamma_angle = thisShower.Dir().Angle(thatTrack.Dir());
      _MCproton_x     = trackParticle.Vertex()[0];
      _MCproton_y     = trackParticle.Vertex()[1];
      _MCproton_z     = trackParticle.Vertex()[2];

      TLorentzVector shower4Mom((showerParticle.Momentum())[0],(showerParticle.Momentum())[1],(showerParticle.Momentum())[2], showerParticle.Energy());
      TLorentzVector track4Mom((trackParticle.Momentum())[0],(trackParticle.Momentum())[1],(trackParticle.Momentum())[2], trackParticle.Energy());
      TLorentzVector proton4Mom = shower4Mom + track4Mom ;
      
      
      //    _MCproton_t     = ;
      _MCproton_px    = proton4Mom.X();
      _MCproton_py    = proton4Mom.Y();
      _MCproton_pz    = proton4Mom.Z();
      //    _MCproton_pdg   = ;
      _MCproton_energy= proton4Mom.T();
      _MCproton_mass  = proton4Mom.Mag();
      
    }
    
    
    for (auto const& I : mc_graph.GetParticleNodes(RecoType_t::kInvisible)){
      //      auto const& thatInvisible = mc_data.Invisible(mc_graph.GetParticle(I).RecoID());
      //if (_verbose) { std::cout << "That invisible: (" << I << ")" << "\tE: " << thatInvisible._energy << std::endl; }
      auto const& invisibleParticle = mc_graph.GetParticle(I);
      if (_verbose) { 
	std::cout << "Invisible particle: (" << I << ")" << "\tE: " << invisibleParticle.Energy() << std::endl; }
    }

    _anaMC_tree->Fill();

    // End of checks on MCParticleGraph
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    return true; 
  }

  void ERAnaMCTruthMode13::ProcessEnd(TFile* fout)
  {
    if(fout){
      fout->cd();
      if (_anaMC_tree)
	_anaMC_tree->Write();
    }
  }

 void ERAnaMCTruthMode13::ClearTree(){

    _MCproton_x     = -1 ;
    _MCproton_y     = -1 ;
    _MCproton_z     = -1 ;
    _MCproton_t     = -1 ;
    _MCproton_px    = -1 ;
    _MCproton_py    = -1 ;
    _MCproton_pz    = -1 ;
    _MCproton_pdg   = -1 ;
    _MCproton_energy= -1 ;
    _MCproton_mass  = -1 ;
       
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
       
    _MCgamma_x      = -1 ;
    _MCgamma_y      = -1 ;
    _MCgamma_z      = -1 ;
    _MCgamma_t      = -1 ;
    _MCgamma_px     = -1 ;
    _MCgamma_py     = -1 ;
    _MCgamma_pz     = -1 ;
    _MCgamma_energy = -1 ;
    _MCgamma_pgd    = -1 ;
    _MCgamma_mass   = -1 ;
       
    _MCmu_gamma_angle = -1 ;

    return;
  }

}

#endif