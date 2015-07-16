#ifndef ERTOOL_ERANAMU_CXX
#define ERTOOL_ERANAMU_CXX

#include "ERAnaMu.h"
#include "ERTool/Algo/AlgoEMPart.h"
#include <math.h>

namespace ertool {

  AlgoEMPart _alg_emp;
  size_t n_mu = 0;
  ERAnaMu::ERAnaMu(const std::string& name) : AnaBase(name)
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
    
    // set verbosity to be off by default
    _verbose = true;
  }

  void ERAnaMu::Reset()
  {}

  void ERAnaMu::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaMu::ProcessBegin()
  {
    InitializeHistos();
  }

  bool ERAnaMu::Analyze(const EventData &data, const ParticleGraph &ps)
 { 
    //Setting TPC borders (x,y,x)
    //geoalgo::Vector TPCIn (0.,-116.5,0.);
    //geoalgo::Vector TPCOut(256.35,116.5,1036.8);

    geoalgo::Vector TPCIn (0.+10,-116.5+10,0.+10);
    geoalgo::Vector TPCOut(256.35-10,116.5-10,1036.8-10);

    geoalgo::Vector MCEndPoint;
    geoalgo::Vector PGEndPoint;

    //    geoalgo::Vector TPCIn (5.,-110.5,5.);
    //geoalgo::Vector TPCOut(251.35,111.5,1035.8);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MCParticleGraph checks
    auto const& mc_graph = MCParticleGraph();
    auto const& mc_graph_arr = mc_graph.GetParticleArray();
    auto const& mc_data = MCEventData();
    
    if (_verbose){
      std::cout<<"########## In MCParticleGraph ########## \n";
      std::cout << "MC Particle Diagram: " << std::endl
		<< mc_graph.Diagram() << std::endl;
      std::cout<<" Graph "<<"\n";
      std::cout<<" Size In AnaMu "<<mc_graph_arr.size()<<"\n";
    }
    
    
    for (auto &p : mc_graph_arr) 
      {
	if ((p.PdgCode() == -13)&&(p.RecoType()==2))
	  {
	    // Take track corresponding to the particle
	    auto const& track1 = mc_data.Track(p.RecoID());
	   
	    
	    
	    if(_verbose) 
	      {
		std::cout<<"IN TPC \n"
			 << track1.back()<<" \n"
			 << TPCIn<<" \n"
			 << TPCOut<<" \n"
			 << ((track1.back()[0] > TPCIn[0])&&(track1.back()[1] > TPCIn[1])&&(track1.back()[2] > TPCIn[2])&&(track1.back()[0] < TPCOut[0])&&(track1.back()[1] < TPCOut[1])&&(track1.back()[2] < TPCOut[2]))<<"\n";
	      }
	    
	    MCEndPoint = track1.back();
	    std::cout<<"IN TPC \n"
		     << track1.back()<<" \n"
		     << TPCIn<<" \n"
		     << TPCOut<<" \n"
		     << ((track1.back()[0] > TPCIn[0])&&(track1.back()[1] > TPCIn[1])&&(track1.back()[2] > TPCIn[2])&&(track1.back()[0] < TPCOut[0])&&(track1.back()[1] < TPCOut[1])&&(track1.back()[2] < TPCOut[2]))<<"\n";
	    bool inTPC = ((track1.back()[0] > TPCIn[0])&&(track1.back()[1] > TPCIn[1])&&(track1.back()[2] > TPCIn[2])&&
			  (track1.back()[0] < TPCOut[0])&&(track1.back()[1] < TPCOut[1])&&(track1.back()[2] < TPCOut[2]));
	    
	    if (inTPC)  Cont_MCMu++;
	    
	    if (_verbose){
	      std::cout<<"\n\n\n";
	      std::cout<<"MCParticleGraph -- Kinematics of the muon\n";
	      std::cout<<"Loop on mc_graph_arr\n";
	      std::cout<<"p.Momentum() = "<<p.Momentum()<<" p.Energy() = "<<p.Energy()<<" p.Mass() = "<<p.Mass()<<" \n";
	      std::cout<<"(p.Momentum().mag()^2 + p.Mass()^2)^0.5  = "<<sqrt(p.Momentum().Length()*p.Momentum().Length()+p.Mass()*p.Mass())<<" \n";
	      std::cout<<"(p.Momentum().mag()^2 + 106^2)^0.5       = "<<sqrt(p.Momentum().Length()*p.Momentum().Length()+106*106)<<" \n";
	      std::cout<<"IDs "<<p.RecoID()<<" recoID "<<p.ID()<<" node\n";
	      std::cout<<"Trk front "<<track1.front()<<"; Trk back"<<track1.back()<<"; track time "<<track1._time<<"\n";
	      std::cout<<"size "<<p.Momentum().size()<<" momentum "<<p.Momentum()<<" "<<p.Energy()<<" \n";
	      std::cout<<p.Print();
	      std::cout<<"Is relation assessed? "<<p.RelationAssessed()<<"\n";
	      std::cout<<"Kids "<<mc_graph.GetAllDescendantNodes(p.ID()).size()<<"\n";
	    }

	    MCMu++;
	    //if (inTPC)
	    // {
	    muTru_energy->Fill(p.Energy());
	    muTru_px->Fill(p.Momentum()[0]);
	    muTru_py->Fill(p.Momentum()[1]);
	    muTru_pz->Fill(p.Momentum()[2]);
	    
	    muTru_x->Fill(track1.front()[0]);
	    muTru_y->Fill(track1.front()[1]);
	    muTru_z->Fill(track1.front()[2]);
	    muTru_t->Fill(track1._time);
	    
	    muTru_pid->Fill(p.PdgCode());
	    // }
	  }
      }


    if (_verbose) std::cout<<"########## Out of MCParticleGraph ########## \n";
    // End of checks on MCParticleGraph
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ParticleGraph checks
    // Loop through Particles associated with a track
    for (auto const& t : ps.GetParticleNodes(RecoType_t::kTrack))
      {
	

	if (_verbose){
	  std::cout<<"########## In PGParticleGraph ########## \n";
	  std::cout << "PG Particle Diagram: " << std::endl
		    << ps.Diagram() << std::endl;
	  std::cout<<" Graph "<<"\n";
	}
    
	// get track object
	auto const& track = data.Track(ps.GetParticle(t).RecoID());
	auto const& p = ps.GetParticle(t);
	// Is my particle inside the TPC?
	auto const& trackEnd = track.back();
	std::cout<<" Checks On p "<<p.PdgCode()<<" "<<p.Children().size()<<" "<<p.Energy()<<" \n";
	      

	if(_verbose) 
	  {
	    std::cout<<"IN TPC ParticleGraph \n"
		     << trackEnd<<" \n"
		     << TPCIn<<" \n"
		     << TPCOut<<" \n"
		     << ((trackEnd[0] > TPCIn[0])&&(trackEnd[1] > TPCIn[1])&&(trackEnd[2] > TPCIn[2])&&(trackEnd[0] < TPCOut[0])&&(trackEnd[1] < TPCOut[1])&&(trackEnd[2] < TPCOut[2]))<<"\n";
	  }

	std::cout<<"IN TPC ParticleGraph \n"
		 << trackEnd<<" \n"
		 << TPCIn<<" \n"
		 << TPCOut<<" \n"
		 << ((trackEnd[0] > TPCIn[0])&&(trackEnd[1] > TPCIn[1])&&(trackEnd[2] > TPCIn[2])&&(trackEnd[0] < TPCOut[0])&&(trackEnd[1] < TPCOut[1])&&(trackEnd[2] < TPCOut[2]))<<"\n\n\n\n";
	bool inTPC = ((trackEnd[0] > TPCIn[0])&&(trackEnd[1] > TPCIn[1])&&(trackEnd[2] > TPCIn[2])&&
		      (trackEnd[0] < TPCOut[0])&&(trackEnd[1] < TPCOut[1])&&(trackEnd[2] < TPCOut[2]));

	if (inTPC)  Cont_PartGraphMu++;
	PGEndPoint = track.back();

	//	std::cout<<"puppa"<< trackEnd < TPCOut<<"\n";

	
	
	// finally, if in verbose mode, cout some info
	if (_verbose){
	  std::cout << "We found a muon!" << std::endl
		    << "Track end point: " << trackEnd << std::endl
		    << "Track energy: " << track._energy << " [MeV]" << std::endl
		    << std::endl;
	}
	
	
      if (track._pid == 4) //muon code
	{
	  PartGraphMu++;
	  //	  if (inTPC)
	  // {
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
	  //  }
	}
  
      }//End loop over tracks

    std::cout<<"MCEndPoint "<< MCEndPoint<<" PGEndPoint "<< PGEndPoint<<"\n";    
    std::cout<<"Diff "<< MCEndPoint - PGEndPoint<<"\n";    


    return true; 
  }


  void ERAnaMu::ProcessEnd(TFile* fout)
  {
    //Write output files    
    if(fout)
      {
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
      }

    //Wrap up, how many muons did we find?
    std::cout<<"Found:\n";
    std::cout<<MCMu<<" MC muons \n";
    std::cout<<PartGraphMu<<" PartGraph muons \n";
    std::cout<<Cont_MCMu<<" MC Contained muons \n";
    std::cout<<Cont_PartGraphMu<<" PartGraph Contained muons \n";
  }

  void ERAnaMu::InitializeHistos()
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

  }



  void ERAnaMu::Finalize()
  {

  }

}

#endif


      /*
	for (auto const& s : ps.GetParticleNodes(RecoType_t::kShower)){
	auto const& shower = data.Shower(ps.GetParticle(s).RecoID());
	if( _alg_emp.LL(true, shower._dedx, -1) > _alg_emp.LL(false, shower._dedx, -1))
	En = shower._energy;
	}
      */
