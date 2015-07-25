#ifndef ERTOOL_ERANACONTAINMENT_CXX
#define ERTOOL_ERANACONTAINMENT_CXX

#include "ERAnaContainment.h"

namespace ertool {

  ERAnaContainment::ERAnaContainment(const std::string& name) 
  : AnaBase(name)
  , fTPC(-10.,-126.,-10.,292.,136.,1150.)
  , _ana_cont_tree(nullptr)
  {}

  void ERAnaContainment::Reset()
  {}

  void ERAnaContainment::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaContainment::ProcessBegin()
  {
    if (_ana_cont_tree) { delete _ana_cont_tree; }
    _ana_cont_tree = new TTree("_ana_cont_tree","Ana Containmente Tree");

    _ana_cont_tree->Branch("_proton_x"     ,&_proton_x     ,"_proton_x/D     ");
    _ana_cont_tree->Branch("_proton_y"     ,&_proton_y     ,"_proton_y/D     ");
    _ana_cont_tree->Branch("_proton_z"     ,&_proton_z     ,"_proton_z/D     ");
    _ana_cont_tree->Branch("_proton_t"     ,&_proton_t     ,"_proton_t/D     ");
    _ana_cont_tree->Branch("_proton_px"    ,&_proton_px    ,"_proton_px/D    ");
    _ana_cont_tree->Branch("_proton_py"    ,&_proton_py    ,"_proton_py/D    ");
    _ana_cont_tree->Branch("_proton_pz"    ,&_proton_pz    ,"_proton_pz/D    ");
    _ana_cont_tree->Branch("_proton_pdg"   ,&_proton_pdg   ,"_proton_pdg/I   ");
    _ana_cont_tree->Branch("_proton_energy",&_proton_energy,"_proton_energy/D");
    _ana_cont_tree->Branch("_proton_mass"  ,&_proton_mass  ,"_proton_mass/D  ");
    _ana_cont_tree->Branch("_proton_momentum"  ,&_proton_momentum  ,"_proton_momentum/D  ");

    _ana_cont_tree->Branch("_mu_x"         ,&_mu_x         ,"_mu_x/D         ");
    _ana_cont_tree->Branch("_mu_y"         ,&_mu_y         ,"_mu_y/D         ");
    _ana_cont_tree->Branch("_mu_z"         ,&_mu_z         ,"_mu_z/D         ");
    _ana_cont_tree->Branch("_mu_xEnd"      ,&_mu_xEnd      ,"_mu_xEnd/D         ");
    _ana_cont_tree->Branch("_mu_yEnd"      ,&_mu_yEnd      ,"_mu_yEnd/D         ");
    _ana_cont_tree->Branch("_mu_zEnd"      ,&_mu_zEnd      ,"_mu_zEnd/D         ");
    _ana_cont_tree->Branch("_mu_t"         ,&_mu_t         ,"_mu_t/D         ");
    _ana_cont_tree->Branch("_mu_px"        ,&_mu_px        ,"_mu_px/D        ");
    _ana_cont_tree->Branch("_mu_py"        ,&_mu_py        ,"_mu_py/D        ");
    _ana_cont_tree->Branch("_mu_pz"        ,&_mu_pz        ,"_mu_pz/D        ");
    _ana_cont_tree->Branch("_mu_pdg"       ,&_mu_pdg       ,"_mu_pdg/I       ");
    _ana_cont_tree->Branch("_mu_energy"    ,&_mu_energy    ,"_mu_energy/D    ");
    _ana_cont_tree->Branch("_mu_mass"      ,&_mu_mass      ,"_mu_mass/D      ");
    _ana_cont_tree->Branch("_mu_momentum"  ,&_mu_momentum  ,"_mu_momentum/D  ");		                                                    

    _ana_cont_tree->Branch("_gamma_x"      ,&_gamma_x      ,"_gamma_x/D      ");
    _ana_cont_tree->Branch("_gamma_y"      ,&_gamma_y      ,"_gamma_y/D      ");
    _ana_cont_tree->Branch("_gamma_z"      ,&_gamma_z      ,"_gamma_z/D      ");
    _ana_cont_tree->Branch("_gamma_t"      ,&_gamma_t      ,"_gamma_t/D      ");
    _ana_cont_tree->Branch("_gamma_px"     ,&_gamma_px     ,"_gamma_px/D     ");
    _ana_cont_tree->Branch("_gamma_py"     ,&_gamma_py     ,"_gamma_py/D     ");
    _ana_cont_tree->Branch("_gamma_pz"     ,&_gamma_pz     ,"_gamma_pz/D     ");
    _ana_cont_tree->Branch("_gamma_energy" ,&_gamma_energy ,"_gamma_energy/D ");
    _ana_cont_tree->Branch("_gamma_pgd"    ,&_gamma_pgd    ,"_gamma_pgd/I    ");
    _ana_cont_tree->Branch("_gamma_mass"   ,&_gamma_mass   ,"_gamma_mass/D   ");
    _ana_cont_tree->Branch("_gamma_momentum"  ,&_gamma_momentum  ,"_gamma_momentum/D  ");		                                                    

    _ana_cont_tree->Branch("_mu_gamma_angle",&_mu_gamma_angle,"_mu_gamma_angle/D");
  }

  bool ERAnaContainment::Analyze(const EventData &data, const ParticleGraph &graph)
  { 
    if (_verbose) { 
      std::cout << "*********** BEGIN PdK RECONSTRUCTION ************" << std::endl;
      std::cout << "Showers in event  : " << data.Shower().size() << std::endl;
      std::cout << "Tracks  in event  : " << data.Track().size() << std::endl;
    }
    

    // Filter them to select gamma-mu showers
    for (auto const& p : graph.GetPrimaryNodes(RecoType_t::kInvisible)){ 
      //Let's fill the proton quantities...
      auto& proton = graph.GetParticle(p);
      _proton_x      = (proton.Vertex())[0]   ;
      _proton_y      = (proton.Vertex())[1]   ;
      _proton_z      = (proton.Vertex())[2]   ;
      //_proton_t     ;
      _proton_px     = (proton.Momentum())[0] ;
      _proton_py     = (proton.Momentum())[1] ;
      _proton_pz     = (proton.Momentum())[2] ;
      _proton_pdg    = proton.PdgCode()       ;
      _proton_energy = proton.Energy()        ;
      _proton_mass   = proton.Mass()          ;
      _proton_momentum = sqrt(_proton_px*_proton_px+_proton_py*_proton_py+_proton_pz*_proton_pz);
      Particle muon;
      Particle gamma;
      for (auto const& c : proton.Children()) 
	{

	  auto& children = graph.GetParticle(c);
	  if (children.PdgCode() == 22)
	    {
	      gamma = children;
	      _gamma_x      = (gamma.Vertex())[0]   ;
	      _gamma_y      = (gamma.Vertex())[1]   ;
	      _gamma_z      = (gamma.Vertex())[2]   ;
	      // _gamma_t             ;		     
	      _gamma_px     = (gamma.Momentum())[0] ;
	      _gamma_py     = (gamma.Momentum())[1] ;
	      _gamma_pz     = (gamma.Momentum())[2] ;
	      _gamma_energy = gamma.Energy()        ;
	      _gamma_pgd    = gamma.PdgCode()       ;
	      _gamma_mass   = gamma.Mass()          ;
	      //	      _gamma_momentum   = gamma.Mass()          ;
	      _gamma_momentum = sqrt(_gamma_px*_gamma_px+_gamma_py*_gamma_py+_gamma_pz*_gamma_pz);

	    } else
	    {
	      
	      auto const& ttrack = data.Track(graph.GetParticle(c).RecoID());
	      muon = children;
	      _mu_x         = (muon.Vertex())[0]   ;
	      _mu_y         = (muon.Vertex())[1]   ;
	      _mu_z         = (muon.Vertex())[2]   ;
	      _mu_xEnd      = (ttrack.back())[0]   ;
	      _mu_yEnd      = (ttrack.back())[1]   ;
	      _mu_zEnd      = (ttrack.back())[2]   ;
	      //    _mu_t          ;			      
	      _mu_px        = (muon.Momentum())[0] ;
	      _mu_py        = (muon.Momentum())[1] ;
	      _mu_pz        = (muon.Momentum())[2] ;
	      _mu_pdg       = muon.PdgCode()       ;
	      _mu_energy    = muon.Energy()        ;
	      _mu_mass      = muon.Mass()          ;   
	      _mu_momentum = sqrt(_mu_px*_mu_px+_mu_py*_mu_py+_mu_pz*_mu_pz);


	    }
	  //_mu_gamma_angle;
	  
	}


      if ((_mu_xEnd > 3    )&&(_mu_xEnd < 250)&&
	  (_mu_yEnd > -113 )&&(_mu_yEnd < 113))
	_ana_cont_tree->Fill();

    }
    return true; 
  }

  void ERAnaContainment::ProcessEnd(TFile* fout)
  {
    if(fout){
      fout->cd();
      if (_ana_cont_tree)
	_ana_cont_tree->Write();
    }
  }

  void ERAnaContainment::ClearTree(){

    _proton_x     = -1 ;
    _proton_y     = -1 ;
    _proton_z     = -1 ;
    _proton_t     = -1 ;
    _proton_px    = -1 ;
    _proton_py    = -1 ;
    _proton_pz    = -1 ;
    _proton_pdg   = -1 ;
    _proton_energy= -1 ;
    _proton_mass  = -1 ;

    _mu_x         = -1 ;
    _mu_y         = -1 ;
    _mu_z         = -1 ;
    _mu_t         = -1 ;
    _mu_px        = -1 ;
    _mu_py        = -1 ;
    _mu_pz        = -1 ;
    _mu_pdg       = -1 ;
    _mu_energy    = -1 ;
    _mu_mass      = -1 ;

    _gamma_x      = -1 ;
    _gamma_y      = -1 ;
    _gamma_z      = -1 ;
    _gamma_t      = -1 ;
    _gamma_px     = -1 ;
    _gamma_py     = -1 ;
    _gamma_pz     = -1 ;
    _gamma_energy = -1 ;
    _gamma_pgd    = -1 ;
    _gamma_mass   = -1 ;

    _mu_gamma_angle = -1 ;

    return;
  }
}

#endif
