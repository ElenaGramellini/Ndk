#ifndef ERTOOL_ERANAMODE13_CXX
#define ERTOOL_ERANAMODE13_CXX

#include "ERAnaMode13.h"

namespace ertool {

  ERAnaMode13::ERAnaMode13(const std::string& name) : AnaBase(name)
    , _ProtonSel_tree(nullptr)
  {
    _verbose          = false;
    _useRadLength     = false;
  }
  
  void ERAnaMode13::Reset()
  {}

  void ERAnaMode13::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    // Load EMPart params
    _alg_emp.AcceptPSet(cfg);
  }

  void ERAnaMode13::ProcessBegin()
  {
    _alg_emp.ProcessBegin();
    _alg_emp.SetMode(true);
    if (_ProtonSel_tree) { delete _ProtonSel_tree; }				  
    _ProtonSel_tree = new TTree("_ProtonSel_tree","Proton Selection Tree");

    _ProtonSel_tree->Branch("n_tracks"  , &n_tracks, "n_tracks/I");
    _ProtonSel_tree->Branch("n_mu"      , &n_mu    , "n_mu/I");

    _ProtonSel_tree->Branch("_run_n"   , &_run_n       , "_run_n/I");
    _ProtonSel_tree->Branch("_subrun_n", &_subrun_n    , "_subrun_n/I");
    _ProtonSel_tree->Branch("_evt_n"   , &_evt_n       , "_evt_n/I");

    _ProtonSel_tree->Branch("_tracks_size"        ,"vector<int>"   ,&_tracks_size        );
    _ProtonSel_tree->Branch("_track_begEndLength" ,"vector<double>",&_track_begEndLength  );
    _ProtonSel_tree->Branch("_track_leng"         ,"vector<double>",&_track_leng          );
    _ProtonSel_tree->Branch("_track_lengthRatio"  ,"vector<double>",&_track_lengthRatio   );
    _ProtonSel_tree->Branch("_track_ReducedLength","vector<double>",&_track_ReducedLength ); 
    _ProtonSel_tree->Branch("_track_FakeLength"   ,"vector<double>",&_track_FakeLength    );
    _ProtonSel_tree->Branch("_tracks_stepsLength" ,"vector<double>",&_tracks_stepsLength  );
    _ProtonSel_tree->Branch("_track_Pdg"          ,"vector<int>"   ,&_track_Pdg       );	   
    _ProtonSel_tree->Branch("_track_DepEnCal"     ,"vector<double>",&_track_DepEnCal  );	   
    _ProtonSel_tree->Branch("_track_DepEnRange"   ,"vector<double>",&_track_DepEnRange);	   
    _ProtonSel_tree->Branch("_track_EnCal"        ,"vector<double>",&_track_EnCal     );	   
    _ProtonSel_tree->Branch("_track_EnRange"      ,"vector<double>",&_track_EnRange   );	   
    _ProtonSel_tree->Branch("_track_Mom"          ,"vector<double>",&_track_Mom       );	   
    _ProtonSel_tree->Branch("_track_px"           ,"vector<double>",&_track_px        );	   
    _ProtonSel_tree->Branch("_track_py"    	  ,"vector<double>",&_track_py        );	   
    _ProtonSel_tree->Branch("_track_pz"    	  ,"vector<double>",&_track_pz        );	   
    _ProtonSel_tree->Branch("_track_EnMPS"        ,"vector<double>",&_track_EnMPS     );	   
    _ProtonSel_tree->Branch("_track_MomMPS"       ,"vector<double>",&_track_MomMPS    );	   
    _ProtonSel_tree->Branch("_track_pxMPS"        ,"vector<double>",&_track_pxMPS     );	   
    _ProtonSel_tree->Branch("_track_pyMPS"        ,"vector<double>",&_track_pyMPS     );	   
    _ProtonSel_tree->Branch("_track_pzMPS"        ,"vector<double>",&_track_pzMPS     );	   
    _ProtonSel_tree->Branch("_track_x"   	  ,"vector<double>",&_track_x     );	   
    _ProtonSel_tree->Branch("_track_y"            ,"vector<double>",&_track_y     );	   
    _ProtonSel_tree->Branch("_track_z"            ,"vector<double>",&_track_z     );	   
    _ProtonSel_tree->Branch("_track_xEnd"         ,"vector<double>",&_track_xEnd  );	   
    _ProtonSel_tree->Branch("_track_yEnd"         ,"vector<double>",&_track_yEnd  );	   
    _ProtonSel_tree->Branch("_track_zEnd"         ,"vector<double>",&_track_zEnd  );              


    _ProtonSel_tree->Branch("n_showers"  , &n_showers, "n_showers/I");
    _ProtonSel_tree->Branch("n_gamma"    , &n_gamma  , "n_gamma/I");
    _ProtonSel_tree->Branch("_shower_Pdg"    ,"vector<int>"   ,&_shower_Pdg   );  
    _ProtonSel_tree->Branch("_shower_DepEn"  ,"vector<double>",&_shower_DepEn );  
    _ProtonSel_tree->Branch("_shower_DeDx "  ,"vector<double>",&_shower_DeDx  );  
    _ProtonSel_tree->Branch("_shower_Mom"    ,"vector<double>",&_shower_Mom   );  
    _ProtonSel_tree->Branch("_shower_px"     ,"vector<double>",&_shower_px    );  
    _ProtonSel_tree->Branch("_shower_py"     ,"vector<double>",&_shower_py    );  
    _ProtonSel_tree->Branch("_shower_pz"     ,"vector<double>",&_shower_pz    );  
    _ProtonSel_tree->Branch("_shower_x"      ,"vector<double>",&_shower_x     );
    _ProtonSel_tree->Branch("_shower_y"      ,"vector<double>",&_shower_y     );
    _ProtonSel_tree->Branch("_shower_z"      ,"vector<double>",&_shower_z     );
    _ProtonSel_tree->Branch("_shower_radius" ,"vector<double>",&_shower_radius);
    _ProtonSel_tree->Branch("_shower_lenght" ,"vector<double>",&_shower_lenght);
	  
    _ProtonSel_tree->Branch("n_protons"  , &n_Protons, "n_protons/I");
    _ProtonSel_tree->Branch("_proton_Pdg"          ,"vector<int>"   ,&_proton_Pdg          );
    _ProtonSel_tree->Branch("_proton_En"           ,"vector<double>",&_proton_En           );
    _ProtonSel_tree->Branch("_proton_Mom"          ,"vector<double>",&_proton_Mom          );
    _ProtonSel_tree->Branch("_proton_px"           ,"vector<double>",&_proton_px           );
    _ProtonSel_tree->Branch("_proton_py"           ,"vector<double>",&_proton_py           );
    _ProtonSel_tree->Branch("_proton_pz"           ,"vector<double>",&_proton_pz           );
    _ProtonSel_tree->Branch("_proton_x"            ,"vector<double>",&_proton_x            );
    _ProtonSel_tree->Branch("_proton_y"            ,"vector<double>",&_proton_y            );
    _ProtonSel_tree->Branch("_proton_z"            ,"vector<double>",&_proton_z            );
    _ProtonSel_tree->Branch("_proton_openingAngle" ,"vector<double>",&_proton_openingAngle );
    _ProtonSel_tree->Branch("_proton_IP"           ,"vector<double>",&_proton_IP           );
    _ProtonSel_tree->Branch("_protonEnDepDistance" ,"vector<double>",&_protonEnDepDistance );    
											       
  }											       

  bool ERAnaMode13::Analyze(const EventData &data, const ParticleGraph &graph)
  { 
    ClearTree();
    _run_n    = data.Run(); 
    _subrun_n = data.SubRun();
    _evt_n    = data.Event_ID();

    auto datacpy = data;
    if (!datacpy.Shower().size()) std::cout<<"No Showers"<<"\n";
    if (!datacpy.Track().size())  std::cout<<"No Tracks"<<"\n";
    n_showers = data.Shower().size();
    n_tracks  = data.Track().size();
    if (_verbose) std::cout<<"N Showers "<<n_showers<<" \n";
    if (_verbose) std::cout<<"N Tracks  "<<n_tracks<<" \n";

    
    //LOOP ON TRACKS
    for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){
      auto const& thatTrack = datacpy.Track(graph.GetParticle(t).RecoID());
      int Pdg = -1; 
      //Assign particle identification
      if ((thatTrack._pid_score[Track::kProton]<thatTrack._pid_score[Track::kPion])&&
	  (thatTrack._pid_score[Track::kProton]<thatTrack._pid_score[Track::kKaon])&&
	  (thatTrack._pid_score[Track::kProton]<thatTrack._pid_score[Track::kMuon]))      Pdg = 2212;
      
      if ((thatTrack._pid_score[Track::kPion]<thatTrack._pid_score[Track::kProton])&&
	  (thatTrack._pid_score[Track::kPion]<thatTrack._pid_score[Track::kKaon])&&
	  (thatTrack._pid_score[Track::kPion]<thatTrack._pid_score[Track::kMuon]))        Pdg = 211; 
      
      if ((thatTrack._pid_score[Track::kKaon]<thatTrack._pid_score[Track::kProton])&&
	  (thatTrack._pid_score[Track::kKaon]<thatTrack._pid_score[Track::kPion])&&
	  (thatTrack._pid_score[Track::kKaon]<thatTrack._pid_score[Track::kMuon]))        Pdg = 321;

      if ((thatTrack._pid_score[Track::kMuon]<thatTrack._pid_score[Track::kProton])&&
	  (thatTrack._pid_score[Track::kMuon]<thatTrack._pid_score[Track::kPion])&&
	  (thatTrack._pid_score[Track::kMuon]<thatTrack._pid_score[Track::kKaon]))        Pdg = 13;      



      if (_verbose) std::cout<<"Track Size "<<thatTrack.size()<<"\n";      
      // Before doing anything else, let's fix the lenght of the track
      if (thatTrack.size() < 100) continue;   // I keep only "long" tracks

      double ReducedLength = 0;
      double fakeLength    = 0; 
      double length        = 0; 
      double begEndLength  = 0;
      auto edge1  = thatTrack.front();
      auto edge2  = thatTrack.back();
      auto trBeg  = thatTrack.front();
      auto trEnd  = thatTrack.back();
      auto trBeg2 = thatTrack.front();

      ///////////////////////////////////////////////////////////
      ///In this block I refine my track
      double chunck = (double) thatTrack.size()/5.;
      int holeIni  = 0 ;  int holeFin  = 0 ;
      _tracks_stepsLength.clear();
      // In this block I fix the real begin and end of the track
      for (int i = 0; i < thatTrack.size()-1; i++){
	auto trajBit = thatTrack[i] - thatTrack[i+1];
	auto stepLength = trajBit.Length();	
	if (i < chunck) {
	  if (stepLength > 5 ) {holeIni = i+1; fakeLength  += stepLength ;}
	}else if (i > chunck*4){ 
	  if (stepLength > 5 ) {fakeLength  += stepLength ; if (!holeFin ) holeFin = i; }
	}
      }// end of points loop
      if (!holeFin) holeFin = thatTrack.size()-1;
      for (int i = holeIni; i < thatTrack.size()-1; i++){
	if ((i >= holeFin )&&(holeFin)) break; 
	auto trajBit = thatTrack[i] - thatTrack[i+1];
	auto stepLength = trajBit.Length();	
	ReducedLength+= stepLength ;
	_tracks_stepsLength.push_back(stepLength);
      }
      edge1 = thatTrack[holeIni];
      edge2 = thatTrack[holeFin];
      ///End of the block where refine my track
      ///////////////////////////////////////////////////////////

      length        = thatTrack.Length();      // track trajectory length from reco obj
      begEndLength  = (edge1-edge2).Length();  // track trajectory length new start to new end

      double EdepCal     = thatTrack._energy;  // track deposited energy from calorimetry obj
      double EdepRange   = ReducedLength*2.3;  // track deposited energy from length obj

      if (EdepRange>2000) continue;
      double mass        = ParticleMass(Pdg);
      double EnergyCal   = EdepCal + mass;
      double EnergyRange = ReducedLength*2.3 + mass;
      double Mom_Mag = sqrt( EnergyRange*EnergyRange - mass*mass );      
      double STShortest ;
      n_mu++;
      //LOOP ON SHOWERS
      for (auto const& p : graph.GetParticleNodes(RecoType_t::kShower)){
	auto const& thisShower = datacpy.Shower(graph.GetParticle(p).RecoID());

	/////////////////////////////////////////////////////////////////
	// In this block I determine the direction of the track
	// (and so the px, py, pt) from the distance to the shower
	if((edge1-thisShower.Start()).Length() < (edge2-thisShower.Start()).Length()){
	  trBeg  = edge1;
	  trEnd  = edge2;
	  trBeg2 = thatTrack[holeIni+1];
	  STShortest = (edge1-thisShower.Start()).Length();
	}
	else  {
	  trBeg = edge2;
	  trEnd = edge1;
	  trBeg2 = thatTrack[holeFin-1];
	  STShortest = (edge2-thisShower.Start()).Length();
	}
	
	if ((trBeg[0]<3.   ) && (trEnd[0]> 250.)) continue;
	if ((trBeg[1]<-113.) && (trEnd[1]> 113.)) continue;
	if ((trBeg[2]<3.   ) && (trEnd[2]>1050.)) continue; 

	if ((trEnd[0]<3.   ) && (trBeg[0]> 250.)) continue;
	if ((trEnd[1]<-113.) && (trBeg[1]> 113.)) continue;
	if ((trEnd[2]<3.   ) && (trBeg[2]>1050.)) continue; 


	if (thisShower._energy>2000) continue;
	geoalgo::Vector_t Dir = (trBeg2-trBeg); 
	Dir.Normalize();
	geoalgo::Vector_t Mom = Dir *  Mom_Mag; 

	double shower_Pdg   ;
	double shower_DepEn ;
	double shower_DeDx  ;
	double shower_Mom   ;
	double shower_px    ;
	double shower_py    ;
	double shower_pz    ;

	
	if (!isGammaLike(thisShower._dedx,trBeg.Dist(thisShower.Start()))){
	  if (_verbose) std::cout<<"It's a gamma!\n";
	  shower_Pdg   = 22;
	  shower_DepEn = thisShower._energy;
	  shower_DeDx  = thisShower._dedx;
	  shower_Mom   = shower_DepEn;
	  shower_px    = (thisShower.Dir()*shower_DepEn)[0];
	  shower_py    = (thisShower.Dir()*shower_DepEn)[1];
	  shower_pz    = (thisShower.Dir()*shower_DepEn)[2];
	}else {
	  shower_Pdg   = 11;
	  shower_DepEn = thisShower._energy + ParticleMass(shower_Pdg); ;
	  shower_DeDx  = thisShower._dedx;
	  shower_Mom   = sqrt(shower_DepEn*shower_DepEn - ParticleMass(shower_Pdg)*ParticleMass(shower_Pdg));
	  shower_px    = (thisShower.Dir()*shower_Mom)[0];
	  shower_py    = (thisShower.Dir()*shower_Mom)[1];
	  shower_pz    = (thisShower.Dir()*shower_Mom)[2];
	}
	
	double openingAngle = thisShower.Dir().Angle(Dir);

	double IP =  _findRel.FindClosestApproach(thisShower,thatTrack,trBeg); 
	TLorentzVector muon4Mom(Mom[0],Mom[1],Mom[2], EnergyRange);
	TLorentzVector gamma4Mom;
	if (openingAngle < 1.) 	{gamma4Mom[0]= -shower_px; gamma4Mom[1]= -shower_py; gamma4Mom[2]=-shower_pz; gamma4Mom[3]=shower_DepEn;}
	else {gamma4Mom[0]=shower_px; gamma4Mom[1]=shower_py; gamma4Mom[2]=shower_pz; gamma4Mom[3]=shower_DepEn;}


	TLorentzVector proton4Mom = muon4Mom+gamma4Mom;
	//	std::cout<<"s "<<thisShower.Radius()<<"\n" ;
	//if ((Pdg!=13)&&(Pdg!=211)) continue;
	//Filling the tree information
	if (holeFin)	{_tracks_size.push_back(holeFin-holeIni+1);}
	else _tracks_size.push_back(thatTrack.size() - holeIni + 1);
	_track_begEndLength .push_back(begEndLength);
	_track_leng         .push_back(length);
	_track_lengthRatio  .push_back(begEndLength/length);
	_track_ReducedLength.push_back(ReducedLength); 
	_track_FakeLength   .push_back(fakeLength);
	_track_Pdg          .push_back(Pdg);
	_track_DepEnCal     .push_back(EdepCal);
	_track_DepEnRange   .push_back(EdepRange);
	_track_EnCal        .push_back(EnergyCal);
	_track_EnRange      .push_back(EnergyRange);
	_track_Mom          .push_back(Mom_Mag);
	_track_px           .push_back(Mom[0]);
	_track_py           .push_back(Mom[1]);
	_track_pz           .push_back(Mom[2]);
	_track_x            .push_back(trBeg[0]);
	_track_y            .push_back(trBeg[1]);
	_track_z            .push_back(trBeg[2]);
	_track_xEnd         .push_back(trEnd[0]);
	_track_yEnd         .push_back(trEnd[1]);
	_track_zEnd         .push_back(trEnd[2]);
	_shower_Pdg         .push_back(shower_Pdg   );
	_shower_DepEn       .push_back(shower_DepEn );
	_shower_DeDx        .push_back(shower_DeDx  );
	_shower_Mom         .push_back(shower_Mom   );
	_shower_px          .push_back(shower_px    );
	_shower_py          .push_back(shower_py    );
	_shower_pz          .push_back(shower_pz    );
	_shower_x           .push_back(thisShower.Start()[0]);
	_shower_y           .push_back(thisShower.Start()[1]);
	_shower_z           .push_back(thisShower.Start()[2]);
	_shower_radius      .push_back(thisShower.Radius());
	_shower_lenght      .push_back(thisShower.Length());
	_proton_openingAngle.push_back(openingAngle);
	_proton_IP          .push_back(IP);
	_protonEnDepDistance.push_back(STShortest);    

	
	_proton_Pdg   .push_back(2212);
	_proton_En    .push_back(EdepRange+shower_DepEn);
	_proton_px    .push_back(proton4Mom.X());
	_proton_py    .push_back(proton4Mom.Y());
	_proton_pz    .push_back(proton4Mom.Z());
	_proton_Mom   .push_back(proton4Mom.P());
	_proton_x     .push_back(trBeg[0]);
	_proton_y     .push_back(trBeg[1]);
	_proton_z     .push_back(trBeg[2]);
	
	
      }
    }
    
    _ProtonSel_tree->Fill();
  
    return true; 
  }

  void ERAnaMode13::ProcessEnd(TFile* fout)
  {
    if(fout){
      if (_ProtonSel_tree)
	_ProtonSel_tree->Write();
    }
  }



  void ERAnaMode13::ClearTree(){

    n_tracks  = 0;
    n_showers = 0;
    n_mu      = 0;
    n_gamma   = 0;
    n_Protons = 0;
    _run_n    = 0;
    _subrun_n = 0;
    _evt_n    = 0;

    _tracks_size.clear();        _track_ReducedLength.clear(); 
    _track_begEndLength.clear(); _track_FakeLength.clear();    
    _track_leng.clear();         _tracks_stepsLength.clear();  
    _track_lengthRatio.clear();  
    _track_Pdg.clear();          _track_EnCal.clear();   
    _track_DepEnCal.clear();     _track_EnRange.clear();  
    _track_DepEnRange.clear();   _track_EnMPS.clear();   
    _track_Mom.clear();          _track_MomMPS.clear();        
    _track_px.clear();           _track_pxMPS.clear();         
    _track_py.clear();           _track_pyMPS.clear();         
    _track_pz.clear();           _track_pzMPS.clear();         	           
    _track_x.clear();            _track_xEnd.clear();   
    _track_y.clear();            _track_yEnd.clear();   
    _track_z.clear();            _track_zEnd.clear();   

    _shower_Pdg  .clear();       _shower_DeDx .clear();
    _shower_DepEn.clear();       _shower_Mom  .clear();
    _shower_px.clear();          _shower_x.clear();
    _shower_py.clear();          _shower_y.clear();
    _shower_pz.clear();          _shower_z.clear();
    _shower_radius.clear();      _shower_lenght.clear();

    _proton_Pdg.clear();         _proton_x.clear();		     
    _proton_En.clear() ;         _proton_y.clear();		     
    _proton_Mom.clear();         _proton_z.clear();		     
    _proton_px.clear() ;         _proton_openingAngle.clear();    
    _proton_py.clear() ;         _proton_IP.clear();		     
    _proton_pz.clear() ;         _protonEnDepDistance.clear();    
  }

  bool ERAnaMode13::isGammaLike(const double dedx, double radlen, bool forceRadLen)
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
