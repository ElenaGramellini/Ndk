#ifndef ERTOOL_ERANAMODE13_CXX
#define ERTOOL_ERANAMODE13_CXX

#include "ERAnaMode13.h"

namespace ertool {

  ERAnaMode13::ERAnaMode13(const std::string& name) : AnaBase(name)
  {
    mu_energy = 0 ;
    mu_px     = 0 ;
    mu_py     = 0 ;
    mu_pz     = 0 ;
    mu_pid    = 0 ;
    mu_x      = 0 ;
    mu_y      = 0 ;
    mu_z      = 0 ;
    mu_t      = 0 ;

    muTru_energy = 0 ;
    muTru_px     = 0 ;
    muTru_py     = 0 ;
    muTru_pz     = 0 ;
    muTru_pid    = 0 ;
    muTru_x      = 0 ;
    muTru_y      = 0 ;
    muTru_z      = 0 ;
    muTru_t      = 0 ;

    gamma_energy = 0 ;
    gamma_px     = 0 ;
    gamma_py     = 0 ;
    gamma_pz     = 0 ;
    gamma_pid    = 0 ;
    gamma_x      = 0 ;
    gamma_y      = 0 ;
    gamma_z      = 0 ;
    gamma_t      = 0 ;
    
    gammaTru_energy = 0 ;
    gammaTru_px     = 0 ;
    gammaTru_py     = 0 ;
    gammaTru_pz     = 0 ;
    gammaTru_pid    = 0 ;
    gammaTru_x      = 0 ;
    gammaTru_y      = 0 ;
    gammaTru_z      = 0 ;
    gammaTru_t      = 0 ;
    
    _useRadLength = false;
    _verbose = false;
  }
  
  void ERAnaMode13::Reset()
  {}

  void ERAnaMode13::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaMode13::ProcessBegin()
  {
    InitializeHistos();
  }

  bool ERAnaMode13::Analyze(const EventData &data, const ParticleGraph &ps)
  { 
    geoalgo::Vector TPCIn (0.+10,-116.5+10,0.+10);
    geoalgo::Vector TPCOut(256.35-10,116.5-10,1036.8-10);

    geoalgo::Vector MCEndPoint;
    geoalgo::Vector PGEndPoint;


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MCParticleGraph checks
    auto const& mc_graph = MCParticleGraph();
    auto const& mc_graph_arr = mc_graph.GetParticleArray();
    auto const& mc_data = MCEventData();
   
    bool isGammaMCDangGoodContained = false;
    bool isGammaPGDangGoodContained = false;
    bool isMuonMCDangGoodContained = false;
    bool isMuonPGDangGoodContained = false;
    
    for (auto &p : mc_graph_arr) 
      {
	if ((p.PdgCode() == -13)&&(p.RecoType()==2))
	  {
	    // Take track corresponding to the particle
	    auto const& track1 = mc_data.Track(p.RecoID());

	    MCEndPoint = track1.back();

	    isMuonMCDangGoodContained = ((track1.back()[0] > TPCIn[0])&&
					      (track1.back()[1] > TPCIn[1])&&
					      (track1.back()[2] > TPCIn[2])&&
					      (track1.back()[0] < TPCOut[0])&&
					      (track1.back()[1] < TPCOut[1])&&
					      (track1.back()[2] < TPCOut[2]));
	    
	    if (isMuonMCDangGoodContained)  Cont_MCMu++;
	    MCMu++;
	    muTru_energy->Fill(p.Energy());
	    muTru_px->Fill(p.Momentum()[0]);
	    muTru_py->Fill(p.Momentum()[1]);
	    muTru_pz->Fill(p.Momentum()[2]);
	    muTru_x->Fill(track1.front()[0]);
	    muTru_y->Fill(track1.front()[1]);
	    muTru_z->Fill(track1.front()[2]);
	    muTru_t->Fill(track1._time);
	    muTru_pid->Fill(p.PdgCode());
	  }

	if (p.PdgCode() == 22)
	  {
	    // Take shower corresponding to the particle
	    auto const& shower1 = mc_data.Shower(p.RecoID());

	    // Define the endpoint of the shower
	    geoalgo::Vector MCEndPoint = gammaEndpoint(shower1);
	    // Check if the endpoint of the shower is in TPC
	    isGammaMCDangGoodContained = ((MCEndPoint[0] > TPCIn[0])&&
					  (MCEndPoint[1] > TPCIn[1])&&
					  (MCEndPoint[2] > TPCIn[2])&&
					  (MCEndPoint[0] < TPCOut[0])&&
					  (MCEndPoint[1] < TPCOut[1])&&
					  (MCEndPoint[2] < TPCOut[2]));
	    
	    //Count how many gammas are dang good contained
	    if (isGammaMCDangGoodContained)  Cont_MCGamma++;
	    //Count how many gammas in MCParticleGraph	    
	    MCGamma++;

	    gammaTru_energy->Fill(p.Energy());
	    gammaTru_px->Fill(p.Momentum()[0]);
	    gammaTru_py->Fill(p.Momentum()[1]);
	    gammaTru_pz->Fill(p.Momentum()[2]);
	    gammaTru_x->Fill(shower1.Start()[0]);
	    gammaTru_y->Fill(shower1.Start()[1]);
	    gammaTru_z->Fill(shower1.Start()[2]);
	    gammaTru_t->Fill(shower1._time);
	    gammaTru_pid->Fill(p.PdgCode());
	  
	  }
      }


    if (_verbose) std::cout<<"########## Out of MCParticleGraph ########## \n";
    // End of checks on MCParticleGraph
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ParticleGraph checks
    // Loop through Particles associated with a track
    for (auto const& t : ps.GetParticleNodes(RecoType_t::kTrack))
      {
	
	// get track object
	auto const& track = data.Track(ps.GetParticle(t).RecoID());
	auto const& p = ps.GetParticle(t);
	std::cout<<"Trk front "<<track.front()<<"; Trk back"<<track.back()<<"; energy "<<p.Energy()<<"\n";
	// Is my particle inside the TPC?
	auto const& trackEnd = track.back();

	isMuonPGDangGoodContained = ((trackEnd[0] > TPCIn[0])&&
				     (trackEnd[1] > TPCIn[1])&&
				     (trackEnd[2] > TPCIn[2])&&
				     (trackEnd[0] < TPCOut[0])&&
				     (trackEnd[1] < TPCOut[1])&&
				     (trackEnd[2] < TPCOut[2]));

	if (isMuonPGDangGoodContained)  {Cont_PartGraphMu++;}
	PGEndPoint = track.back();
	
	if (track._pid == 4) //muon code
	  {
	    PartGraphMu++;
	    mu_energy->Fill(track._energy + 106);  
	    mu_pid->Fill(track._pid);
	    mu_px->Fill(p.Momentum()[0]);
	    mu_py->Fill(p.Momentum()[1]);
	    mu_pz->Fill(p.Momentum()[2]);
	    mu_x->Fill(track.front()[0]);
	    mu_y->Fill(track.front()[1]);
	    mu_z->Fill(track.front()[2]);
	    mu_t->Fill(track._time);
	    mu_pid->Fill(track._pid);
	  }  
      }//End loop over tracks
    
    // Loop through Particles associated with a shower
    for (auto const& t : ps.GetParticleNodes(RecoType_t::kShower))
      {	
	// get shower object
	auto const& shower = data.Shower(ps.GetParticle(t).RecoID());
	auto const& p = ps.GetParticle(t);
	std::cout<<"Shower front "<<shower.Start()<<" "<<p.Energy()<<"\n";
	// Is my particle inside the TPC?
	auto const& PGEndPoint  = gammaEndpoint(shower);
	// Check if the endpoint of the shower is in TPC
	isGammaPGDangGoodContained = ((PGEndPoint[0] > TPCIn[0])&&
				      (PGEndPoint[1] > TPCIn[1])&&
				      (PGEndPoint[2] > TPCIn[2])&&
				      (PGEndPoint[0] < TPCOut[0])&&
				      (PGEndPoint[1] < TPCOut[1])&&
				      (PGEndPoint[2] < TPCOut[2]));

	//check if gamma 
	if (isGammaLike(shower._dedx,-1)) 
	  {
	    //Count how many gammas are dang good contained
	    if (isGammaPGDangGoodContained)  {Cont_PartGraphGamma++;}
	    // Count Gammas in ParticleGraph
	    PartGraphGamma++;
	    //Fill histos with all possible info
	    gamma_energy->Fill(shower._energy);
	    // gamma_pid->Fill(isGammaLike(shower._dedx,-1));
	    gamma_px->Fill(p.Momentum()[0]);
	    gamma_py->Fill(p.Momentum()[1]);
	    gamma_pz->Fill(p.Momentum()[2]);
	    gamma_x->Fill(shower.Start()[0]);
	    gamma_y->Fill(shower.Start()[1]);
	    gamma_z->Fill(shower.Start()[2]);
	    gamma_t->Fill(shower._time);
	    
	    // geoalgo::HalfLine_t showerProj(shower.Start(),-shower.Dir());
	    //std::cout<<"PUPPPPAA "<<showerProj<<"\n";
	  }//else 
	//{if (!p.RecoID()) std::cout<<"PUPPPPAA "<<shower._dedx<<"\n";}
	
      }//End loop over showers


    return true; 
  }

  void ERAnaMode13::ProcessEnd(TFile* fout)
  {
    if(fout){
      fout->cd();
      if(mu_energy) mu_energy->Write();
      if(mu_px    ) mu_px    ->Write();
      if(mu_py    ) mu_py    ->Write();
      if(mu_pz    ) mu_pz    ->Write();
      if(mu_pid   ) mu_pid   ->Write();
      if(mu_x     ) mu_x     ->Write();
      if(mu_y     ) mu_y     ->Write();
      if(mu_z     ) mu_z     ->Write();
      if(mu_t     ) mu_t     ->Write();
      if(muTru_energy) muTru_energy->Write();
      if(muTru_px    ) muTru_px    ->Write();
      if(muTru_py    ) muTru_py    ->Write();
      if(muTru_pz    ) muTru_pz    ->Write();
      if(muTru_pid   ) muTru_pid   ->Write();	
      if(muTru_x     ) muTru_x     ->Write();
      if(muTru_y     ) muTru_y     ->Write();
      if(muTru_z     ) muTru_z     ->Write();
      if(muTru_t     ) muTru_t     ->Write();
      
      if(gamma_energy) gamma_energy->Write();
      if(gamma_px    ) gamma_px    ->Write();
      if(gamma_py    ) gamma_py    ->Write();
      if(gamma_pz    ) gamma_pz    ->Write();
      if(gamma_pid   ) gamma_pid   ->Write();
      if(gamma_x     ) gamma_x     ->Write();
      if(gamma_y     ) gamma_y     ->Write();
      if(gamma_z     ) gamma_z     ->Write();
      if(gamma_t     ) gamma_t     ->Write();
      if(gammaTru_energy) gammaTru_energy->Write();
      if(gammaTru_px    ) gammaTru_px    ->Write();
      if(gammaTru_py    ) gammaTru_py    ->Write();
      if(gammaTru_pz    ) gammaTru_pz    ->Write();
      if(gammaTru_pid   ) gammaTru_pid   ->Write();
      if(gammaTru_x     ) gammaTru_x     ->Write();
      if(gammaTru_y     ) gammaTru_y     ->Write();
      if(gammaTru_z     ) gammaTru_z     ->Write();
      if(gammaTru_t     ) gammaTru_t     ->Write();
     	      
     }
     
     //Wrap up, how many muons and gammas did we find?
     std::cout<<"Found:\n";
     std::cout<<MCMu<<" MC muons \n";
     std::cout<<PartGraphMu<<" PartGraph muons \n";
     std::cout<<Cont_MCMu<<" MC Contained muons \n";
     std::cout<<Cont_PartGraphMu<<" PartGraph Contained muons \n";
     std::cout<<MCGamma<<" MC Gamma \n";
     std::cout<<PartGraphGamma<<" PartGraph Gamma \n";
     std::cout<<Cont_MCGamma<<" MC Contained Gamma \n";
     std::cout<<Cont_PartGraphGamma<<" PartGraph Contained Gamma \n";

  }

void ERAnaMode13::InitializeHistos()
  {
       //Initialize histograms
    if(!mu_energy) mu_energy = new TH1F("mu_energy","mu_energy", 200  , 0    , 2000  );
    if(!mu_px ) mu_px        = new TH1F("mu_px"    ,"mu_px"    , 100  ,-500  , 500   );
    if(!mu_py ) mu_py        = new TH1F("mu_py"    ,"mu_py"    , 100  ,-500  , 500   );
    if(!mu_pz ) mu_pz        = new TH1F("mu_pz"    ,"mu_pz"    , 100  ,-500  , 500   );
    if(!mu_pid) mu_pid       = new TH1F("mu_pid"   ,"mu_pid"   , 10   ,-0.5  , 9.5   );
    if(!mu_x  ) mu_x         = new TH1F("mu_x"     ,"mu_x"     , 256  , 0    , 256.35);
    if(!mu_y  ) mu_y         = new TH1F("mu_y"     ,"mu_y"     , 233  ,-116.5, 116.5 ); 
    if(!mu_z  ) mu_z         = new TH1F("mu_z"     ,"mu_z"     , 1036 , 0.   , 1036.8); 
    if(!mu_t  ) mu_t         = new TH1F("mu_t"     ,"mu_t"     , 100  ,-500  , 500   ); 

    if(!muTru_energy) muTru_energy = new TH1F("muTru_energy","muTru_energy",200, 0  ,2000);
    if(!muTru_px    ) muTru_px     = new TH1F("muTru_px","muTru_px"        ,50 ,-500,500 );
    if(!muTru_py    ) muTru_py     = new TH1F("muTru_py","muTru_py"        ,50 ,-500,500 );
    if(!muTru_pz    ) muTru_pz     = new TH1F("muTru_pz","muTru_pz"        ,50 ,-500,500 );
    if(!muTru_pid   ) muTru_pid    = new TH1F("muTruPid","muTruPid"        ,10 ,-0.5,9.5 );
    if(!muTru_x     ) muTru_x      = new TH1F("muTru_x" ,"muTru_x"         ,256  , 0    , 256.35);
    if(!muTru_y     ) muTru_y      = new TH1F("muTru_y" ,"muTru_y"         ,233  ,-116.5, 116.5 ); 
    if(!muTru_z     ) muTru_z      = new TH1F("muTru_z" ,"muTru_z"         ,1036 , 0.   , 1036.8); 
    if(!muTru_t     ) muTru_t      = new TH1F("muTru_t" ,"muTru_t"         ,100  ,-500000  , 500000   ); 

    if(!gamma_energy) gamma_energy= new TH1F("gamma_energy","gamma_energy", 200  , 0    , 2000  );	
    if(!gamma_px    ) gamma_px    = new TH1F("gamma_px"    ,"gamma_px"    , 100  ,-500  , 500   );	
    if(!gamma_py    ) gamma_py    = new TH1F("gamma_py"    ,"gamma_py"    , 100  ,-500  , 500   );	
    if(!gamma_pz    ) gamma_pz    = new TH1F("gamma_pz"    ,"gamma_pz"    , 100  ,-500  , 500   );	
    if(!gamma_pid   ) gamma_pid   = new TH1F("gamma_pid"   ,"gamma_pid"   , 10   ,-0.5  , 9.5   );	
    if(!gamma_x     ) gamma_x     = new TH1F("gamma_x"     ,"gamma_x"     , 256  , 0    , 256.35);	
    if(!gamma_y     ) gamma_y     = new TH1F("gamma_y"     ,"gamma_y"     , 233  ,-116.5, 116.5 ); 
    if(!gamma_z     ) gamma_z     = new TH1F("gamma_z"     ,"gamma_z"     , 1036 , 0.   , 1036.8); 
    if(!gamma_t     ) gamma_t     = new TH1F("gamma_t"     ,"gamma_t"     , 100  ,-500  , 500   ); 
    
    if(!gammaTru_energy) gammaTru_energy = new TH1F("gammaTru_energy","gammaTru_energy"    ,200, 0  ,2000);
    if(!gammaTru_px    ) gammaTru_px     = new TH1F("gammaTru_px    ","gammaTru_px"        ,50 ,-500,500 );
    if(!gammaTru_py    ) gammaTru_py     = new TH1F("gammaTru_py    ","gammaTru_py"        ,50 ,-500,500 );
    if(!gammaTru_pz    ) gammaTru_pz     = new TH1F("gammaTru_pz    ","gammaTru_pz"        ,50 ,-500,500 );
    if(!gammaTru_pid   ) gammaTru_pid    = new TH1F("gamma_TruPid   ","gamma_TruPid"       ,10 ,-0.5,9.5 );
    if(!gammaTru_x     ) gammaTru_x      = new TH1F("gammaTru_x"     ,"gammaTru_x"     , 256  , 0    , 256.35);	
    if(!gammaTru_y     ) gammaTru_y      = new TH1F("gammaTru_y"     ,"gammaTru_y"     , 233  ,-116.5, 116.5 ); 
    if(!gammaTru_z     ) gammaTru_z      = new TH1F("gammaTru_z"     ,"gammaTru_z"     , 1036 , 0.   , 1036.8); 
    if(!gammaTru_t     ) gammaTru_t      = new TH1F("gammaTru_t"     ,"gammaTru_t"     , 100  ,-500  , 500   ); 

  }

 bool ERAnaMode13::isGammaLike(const double dedx, double radlen, bool forceRadLen)
  {
    if ( !_useRadLength && !forceRadLen ) radlen = -1;
    if ( _alg_emp.LL(true, dedx, radlen) < _alg_emp.LL(false, dedx, radlen) )
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


  const geoalgo::Vector ERAnaMode13::gammaEndpoint(const ertool::Shower shower)
  {
    geoalgo::Vector Endpoint;
    Endpoint = shower.Start() + shower.Dir()*shower.Length();
    return Endpoint;
  }

}

#endif
