#ifndef ERTOOL_ERANAGAMMA_CXX
#define ERTOOL_ERANAGAMMA_CXX

#include "ERAnaGamma.h"
#include <math.h>
#include "ERTool/Algo/AlgoEMPart.h"


namespace ertool {
  
  ERAnaGamma::ERAnaGamma(const std::string& name) : AnaBase(name)
  {
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
    _verbose = true;
    
  }

  void ERAnaGamma::Reset()
  {}

  void ERAnaGamma::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaGamma::ProcessBegin()
  {
    InitializeHistos();
  }

  bool ERAnaGamma::Analyze(const EventData &data, const ParticleGraph &ps)
  {     
    
    //Setting TPC borders (x,y,x)
    //geoalgo::Vector TPCIn (0.,-116.5,0.);
    //geoalgo::Vector TPCOut(256.35,116.5,1036.8);
    
    geoalgo::Vector TPCIn (0.+10,-116.5+10,0.+10);
    geoalgo::Vector TPCOut(256.35-10,116.5-10,1036.8-10);
    
    geoalgo::Vector MCEndPoint;
    geoalgo::Vector PGEndPoint;
    
    bool isGammaMCDangGoodContained;
    bool isGammaPGDangGoodContained;

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
      std::cout<<" Size In AnaGamma "<<mc_graph_arr.size()<<"\n";
    }
    
    
    for (auto &p : mc_graph_arr) 
      {
	//take only gamma shower (possible only in MCParticle graph)
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

	    if(_verbose) 
	      {
		std::cout<<"RecoType"<< p.RecoType()<<" recoID "<<p.RecoID()<<"\n";
		std::cout<<"IN TPC \n"
			 << MCEndPoint<<" \n"
			 << TPCIn<<" \n"
			 << TPCOut<<" \n"
			 << isGammaMCDangGoodContained <<": 0 if not contained, 1 if contained\n";
		std::cout<<"\n\n\n";
		std::cout<<"MCParticleGraph -- Kinematics of the gamma\n";
		std::cout<<"Loop on mc_graph_arr\n";
		std::cout<<"p.Momentum() = "<<p.Momentum()<<" p.Energy() = "<<p.Energy()<<" p.Mass() = "<<p.Mass()<<" \n";
		std::cout<<"(p.Momentum().mag()^2 + p.Mass()^2)^0.5  = "<<sqrt(p.Momentum().Length()*p.Momentum().Length()+p.Mass()*p.Mass())<<" \n";
		std::cout<<"(p.Momentum().mag()^2 + 106^2)^0.5       = "<<sqrt(p.Momentum().Length()*p.Momentum().Length()+106*106)<<" \n";
		std::cout<<"IDs "<<p.RecoID()<<" recoID "<<p.ID()<<" node\n";
		std::cout<<"Trk front "<<shower1.Start()<<"; Trk back"<<MCEndPoint<<"; shower time "<<shower1._time<<"\n";
		std::cout<<"size "<<p.Momentum().size()<<" momentum "<<p.Momentum()<<" "<<p.Energy()<<" \n";
		std::cout<<p.Print();
		std::cout<<"Is relation assessed? "<<p.RelationAssessed()<<"\n";
		std::cout<<"Kids "<<mc_graph.GetAllDescendantNodes(p.ID()).size()<<"\n";
	      }
	    
	    //Count how many gammas are dang good contained
	    if (isGammaMCDangGoodContained)  Cont_MCGamma++;
	    //Count how many gammas in MCParticleGraph	    
	    MCGamma++;
	    // if (isGammaMCDangGoodContained)
	    // {
	    //Fill histos with all possible info
	    gammaTru_energy->Fill(p.Energy());
	    gammaTru_px->Fill(p.Momentum()[0]);
	    gammaTru_py->Fill(p.Momentum()[1]);
	    gammaTru_pz->Fill(p.Momentum()[2]);
	    gammaTru_x->Fill(shower1.Start()[0]);
	    gammaTru_y->Fill(shower1.Start()[1]);
	    gammaTru_z->Fill(shower1.Start()[2]);
	    gammaTru_t->Fill(shower1._time);
	    gammaTru_pid->Fill(p.PdgCode());
	       
	    //  }
	  }
      }
    if (_verbose) std::cout<<"########## Out of MCParticleGraph ########## \n";
    // End of checks on MCParticleGraph
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ParticleGraph checks
    // Loop through Particles associated with a shower
    for (auto const& t : ps.GetParticleNodes(RecoType_t::kShower))
      {	
	// get shower object
	auto const& shower = data.Shower(ps.GetParticle(t).RecoID());
	auto const& p = ps.GetParticle(t);
	
	// Is my particle inside the TPC?
	auto const& PGEndPoint  = gammaEndpoint(shower);
	// Check if the endpoint of the shower is in TPC
	isGammaPGDangGoodContained = ((PGEndPoint[0] > TPCIn[0])&&
				      (PGEndPoint[1] > TPCIn[1])&&
				      (PGEndPoint[2] > TPCIn[2])&&
				      (PGEndPoint[0] < TPCOut[0])&&
				      (PGEndPoint[1] < TPCOut[1])&&
				      (PGEndPoint[2] < TPCOut[2]));


	if(_verbose) 
	  {
	    std::cout<<"IN TPC ParticleGraph \n"
		     << PGEndPoint<<" \n"
		     << TPCIn<<" \n"
		     << TPCOut<<" \n"
		     << isGammaPGDangGoodContained<<"\n";
	    std::cout << "We found a gamma!" << std::endl
		      << "Shower end point: " << PGEndPoint << std::endl
		      << "Shower energy: " << shower._energy << " [MeV]" << std::endl
		      << std::endl;
	  }

	std::cout<<"RecoID "<<p.RecoID()<<" \n";
	//check if gamma 
	if (isGammaLike(shower._dedx,-1)) 
	  {
	    

	    //Count how many gammas are dang good contained
	    if (isGammaPGDangGoodContained)  Cont_PartGraphGamma++;
	    // Count Gammas in ParticleGraph
	    PartGraphGamma++;
	    //if (isGammaPGDangGoodContained)
	    //  {
	    //Fill histos with all possible info
	    gamma_energy->Fill(shower._energy);
	    gamma_pid->Fill(isGammaLike(shower._dedx,-1));
	    gamma_px->Fill(p.Momentum()[0]);
	    gamma_py->Fill(p.Momentum()[1]);
	    gamma_pz->Fill(p.Momentum()[2]);
	    gamma_x->Fill(shower.Start()[0]);
	    gamma_y->Fill(shower.Start()[1]);
	    gamma_z->Fill(shower.Start()[2]);
	    gamma_t->Fill(shower._time);
	    
	    //}
	  }
	
      }//End loop over showers

    std::cout<<"MCEndPoint "<< MCEndPoint<<" PGEndPoint "<< PGEndPoint<<"\n";    
    std::cout<<"Diff "<< MCEndPoint - PGEndPoint<<"\n";    


    std::cout<<"Found:\n";
    std::cout<<MCGamma<<" MC gamma \n";
    std::cout<<PartGraphGamma<<" PartGraph gamma \n";

    return true; 
  }

  void ERAnaGamma::ProcessEnd(TFile* fout)
  {
    if(fout){
      fout->cd();
      
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

    //Wrap up, how many gammas did we find?
    std::cout<<"Found:\n";
    std::cout<<MCGamma<<" MC Gamma \n";
    std::cout<<PartGraphGamma<<" PartGraph Gamma \n";
    std::cout<<Cont_MCGamma<<" MC Contained Gamma \n";
    std::cout<<Cont_PartGraphGamma<<" PartGraph Contained Gamma \n";
  }

  void ERAnaGamma::InitializeHistos()
  {
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

  bool ERAnaGamma::isGammaLike(const double dedx, double radlen, bool forceRadLen)
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

  const geoalgo::Vector ERAnaGamma::gammaEndpoint(const ertool::Shower shower)
  {
    geoalgo::Vector Endpoint;
    Endpoint = shower.Start() + shower.Dir()*shower.Length();
    return Endpoint;
  }

}

#endif
