#ifndef ERTOOL_ERANAONETOONEGAMMA_CXX
#define ERTOOL_ERANAONETOONEGAMMA_CXX

#include "ERAnaOneToOneGamma.h"

namespace ertool {

  ERAnaOneToOneGamma::ERAnaOneToOneGamma(const std::string& name) : AnaBase(name)  , _ana1to1_tree(nullptr)
  {
    _verbose = false;
  }

  void ERAnaOneToOneGamma::Reset()
  {}

  void ERAnaOneToOneGamma::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    // Load EMPart params
    _alg_emp.AcceptPSet(cfg);
  }

  void ERAnaOneToOneGamma::ProcessBegin()
  {
    _alg_emp.ProcessBegin();
    _alg_emp.SetMode(true);
    if (_ana1to1_tree) { delete _ana1to1_tree; }
    _ana1to1_tree = new TTree("_ana1to1_tree","Ana 1 to 1 Comparison Tree");

    _ana1to1_tree->Branch("n_shower"       , &n_shower, "n_shower/I");
    _ana1to1_tree->Branch("_shower_size"   , &_shower_size, "_shower_size/I");
    _ana1to1_tree->Branch("_showerTru_size", &_showerTru_size, "_showerTru_size/I");

    _ana1to1_tree->Branch("_shower_Pdg"            , "vector<int>"    , &_shower_Pdg);                                
    _ana1to1_tree->Branch("_shower_leng"           , "vector<double>"    , &_shower_leng);                             		  	  	  
    _ana1to1_tree->Branch("_shower_DepEn"          , "vector<double>"    , &_shower_DepEn);                 
    _ana1to1_tree->Branch("_shower_x"              , "vector<double>"    , &_shower_x);     		   	
    _ana1to1_tree->Branch("_shower_y"              , "vector<double>"    , &_shower_y);     		   	
    _ana1to1_tree->Branch("_shower_z"              , "vector<double>"    , &_shower_z);     		   	


    _ana1to1_tree->Branch("_showerTru_Pdg"            , "vector<int>"       , &_showerTru_Pdg);                                
    _ana1to1_tree->Branch("_showerTru_leng"           , "vector<double>"    , &_showerTru_leng);                             		  	  	  
    _ana1to1_tree->Branch("_showerTru_DepEn"          , "vector<double>"    , &_showerTru_DepEn);                 
    _ana1to1_tree->Branch("_showerTru_x"              , "vector<double>"    , &_showerTru_x);     		   	
    _ana1to1_tree->Branch("_showerTru_y"              , "vector<double>"    , &_showerTru_y);     		   	
    _ana1to1_tree->Branch("_showerTru_z"              , "vector<double>"    , &_showerTru_z);  
   		   	
    _ana1to1_tree->Branch("_shower_DeltaDepEn"        , "vector<double>"    , &_shower_DeltaDepEn);          
    _ana1to1_tree->Branch("_shower_Deltax"            , "vector<double>"    , &_shower_Deltax);     		
    _ana1to1_tree->Branch("_shower_Deltay"            , "vector<double>"    , &_shower_Deltay);     		
    _ana1to1_tree->Branch("_shower_Deltaz"            , "vector<double>"    , &_shower_Deltaz);     		
    _ana1to1_tree->Branch("_shower_DeltaLength"       , "vector<double>"    , &_shower_DeltaLength);           

  }

  bool ERAnaOneToOneGamma::Analyze(const EventData &data, const ParticleGraph &graph)
  { 
    ClearTree();
    Particle showerMCParticle;           Particle showerParticle;
    Shower   MCShower;        	         Shower   Shower;       
 
    int    MCPdg            = 0;         int    Pdg            = 0;	 
    double MCleng           = 0;         double leng           = 0;	 
    double MCxShower = 0;	      	 double xShower= 0;	      	 
    double MCyShower = 0;	      	 double yShower= 0;	      	 
    double MCzShower = 0;	      	 double zShower= 0;	      	 
    double MCDepEnMCShower = 0;          double DepEnShower = 0;
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MCParticleGraph checks
    auto const& mc_graph = MCParticleGraph();
    auto const& mc_graph_arr = mc_graph.GetParticleArray();
    auto const& mc_data = MCEventData();
    if (!mc_data.Shower().size()) std::cout<<"No MCShowers"<<"\n";
    
    if (_verbose){
      std::cout<<"########## In MCParticleGraph ########## \n";
      std::cout << "MC Particle Diagram: " << std::endl
		<< mc_graph.Diagram() << std::endl;
      std::cout<<" Graph "<<"\n";
      std::cout<<" Size In ERAnaMCTruth "<<mc_graph_arr.size()<<"\n";
      std::cout<<"Number of Shower: "<<mc_data.Shower().size()<<"\n";
    }
    _showerTru_size = mc_data.Shower().size();
    for (auto const& t : mc_graph.GetParticleNodes(RecoType_t::kShower)){
      if (mc_graph.GetParticle(t).PdgCode() == 22){
	MCShower = mc_data.Shower(mc_graph.GetParticle(t).RecoID());
	showerMCParticle = mc_graph.GetParticle(t);
	
	MCPdg         = showerMCParticle.PdgCode();	  	  
	MCxShower     = showerMCParticle.Vertex()[0];	      		  
	MCyShower     = showerMCParticle.Vertex()[1];	      		  
	MCzShower     = showerMCParticle.Vertex()[2];	      		  
	MCDepEnMCShower = showerMCParticle.Energy();	      		  
	MCleng          = MCShower.Length();	  
        
	      
      if (_verbose) { 
	std::cout << "Shower:              (" << t << ")" << "\tE: " << MCShower._energy << std::endl; 
	std::cout << "Shower particle:     (" << t << ")" << "\tE: " << showerMCParticle.Energy() << std::endl; }      

      if (mc_data.Shower().size())
	{
	  _showerTru_Pdg.push_back(MCPdg)                         ;
	  _showerTru_leng.push_back(MCleng)                       ;
	  _showerTru_DepEn.push_back(MCDepEnMCShower)              ;
	  
	  _showerTru_x.push_back   (MCxShower     )  ;	      
	  _showerTru_y.push_back   (MCyShower     )  ;	      
	  _showerTru_z.push_back   (MCzShower     )  ;	      
	}
      
      }
    }

    
    // End of checks on MCParticleGraph
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  
    // Loop through Particles associated with a shower
    //LOOP ON SHOWER
    auto const shower_nodes = graph.GetParticleNodes(RecoType_t::kShower);
    for (auto const& t : shower_nodes){
      
      auto const& showerParticle = graph.GetParticle(t);
      auto const& Shower = data.Shower(graph.GetParticle(t).RecoID());
      //Assign particle identification
      
      xShower = Shower.Start()[0];	      	 
      yShower = Shower.Start()[1];	      	 
      zShower = Shower.Start()[2];	      	 
      leng    = Shower.Length();	      	 
      DepEnShower = Shower._energy;
      if (!isGammaLike(Shower._dedx,0.,0)){
	Pdg   = 22;
      }else{
	Pdg   = 11;
      }
      
      _showerTru_size = data.Shower().size();
      if (data.Shower().size())
	{
	  _shower_Pdg.push_back(Pdg)          ;
	  _shower_leng.push_back(leng)        ;
	  _shower_DepEn.push_back(DepEnShower);
	  
	  _shower_x.push_back(xShower)  ;	      
	  _shower_y.push_back(yShower)  ;	      
	  _shower_z.push_back(zShower)  ;	      
	  

	  if (mc_data.Shower().size())
	    {
	      _shower_DeltaLength.push_back(leng - MCleng)     ;		
	      _shower_DeltaDepEn.push_back(DepEnShower - MCDepEnMCShower)              ;	
	      _shower_Deltax.push_back   (xShower    - MCxShower    )  ;	      	
	      _shower_Deltay.push_back   (yShower    - MCyShower    )  ;	      	
	      _shower_Deltaz.push_back   (zShower    - MCzShower    )  ;	      	
	      
	    }
	}
      
     }
    


    _ana1to1_tree->Fill();
    return true; 
  }

  void ERAnaOneToOneGamma::ProcessEnd(TFile* fout)
  {
    if(fout){
      fout->cd();
      if (_ana1to1_tree)
	_ana1to1_tree->Write();
    }
  }

  void ERAnaOneToOneGamma::ClearTree(){

    n_shower = 0;
    _shower_size = 0;       _showerTru_size = 0;
   
    _shower_Pdg.clear()            ;       _showerTru_Pdg.clear()            ;
    _shower_leng.clear()           ;       _showerTru_leng.clear()           ;
           		  	  			
    _shower_DepEn.clear()          ;       _showerTru_DepEn.clear()          ;
    _shower_x.clear()     ;		   _showerTru_x.clear()     ;		
    _shower_y.clear()     ;		   _showerTru_y.clear()     ;		
    _shower_z.clear()     ;		   _showerTru_z.clear()     ;		


    _shower_DeltaDepEn.clear()          ;
    _shower_Deltax.clear()     ;		
    _shower_Deltay.clear()     ;		
    _shower_Deltaz.clear()     ;		
    _shower_DeltaLength.clear()  ;         
  }

  bool ERAnaOneToOneGamma::isGammaLike(const double dedx, double radlen, bool forceRadLen)
  {
    if ( !_useRadLength && !forceRadLen )
      radlen = -1;
    if ( _alg_emp.LL(true, dedx, radlen) > _alg_emp.LL(false, dedx, radlen) )
      {
	if (_verbose) {
	  std::cout << "Shower has dEdx = " << dedx
		    << "\tRadLen = " << radlen
		    << "\tIgnore for comparison." << std::endl;
	}
	return true;
      }
    
    return false;
  }

}

#endif
