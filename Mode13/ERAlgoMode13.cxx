// Incomplete list of TO DO
// [ ] Feed the cuts parameters from cgf file, not like the dick of the dog.
// [ ] Put scores!!!!!
// [ ] there's a bug!!

#ifndef ERTOOL_ERALGOMODE13_CXX
#define ERTOOL_ERALGOMODE13_CXX

#include "ERAlgoMode13.h"

namespace ertool {
  
  //Variables for checking...  
  int elect = 0;
  std::map<ertool::Track,int> muonMap;

  ERAlgoMode13::ERAlgoMode13(const std::string& name) 
  : AlgoBase(name)
    , fTPC(-10.,-126.,-10.,292.,136.,1150.)
    , _empart_tree(nullptr)
    , _alg_tree(nullptr)
  {
    _gamma_mass       = ParticleMass(22);
    _mu_mass          = ParticleMass(13);
    _verbose          = false;
    _useRadLength     = false;

    // Sets of cuts, probably this is not the final location of these cuts
    // Topologycal cuts
     _EnDepDistanceMaxCut =  99999999.;
     _IPMaxCut            =  99999999.;
     _OpeningAngleMinCut  =  0.;
    
    // Calorimetry loose cuts
     _MuonEnergyMaxCut    =  99999999.;
     _GammaEnergyMaxCut   =  99999999.;
     _TotalEnergyMaxCut   =  99999999.;
     _MuonEnergyMinCut    =  0.;
     _GammaEnergyMinCut   =  0.;
     _TotalEnergyMinCut   =  0.;
     _TotMomXCutMax       =  99999999.;
     _TotMomYCutMax       =  99999999.;
     _TotMomZCutMax       =  99999999.;
     _TotMomXCutMin       = -99999999.;
     _TotMomYCutMin       = -99999999.;
     _TotMomZCutMin       = -99999999.;


    _BDtW            = 0; 
    _BDtTW           = 0;
    _protonsdKs      = 0;
    _cOnePlusTrack   = 0;
    _cOnePlusShower  = 0;
    _cEnDepRadius    = 0;
    _cIP             = 0;
    _cOnePlusGamma   = 0;
    _cOnePlusMu      = 0;
    _cNoVtxAct       = 0;
    _cOpeningAngle   = 0;
    _cMuonEnergy     = 0;
    _cGammaEnergy    = 0;
    _cTotalEnergy    = 0;
    _cTotMom         = 0;

    /*   
     _Ethreshold       = 0;
    _hassister        = false;
    _rejectLongTracks = true;
    _vtxProximityCut = 0;
    */
  }

  void ERAlgoMode13::Reset()
  {
  }

  void ERAlgoMode13::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    // Load EMPart params
    _alg_emp.AcceptPSet(cfg);
    
    std::cout << this->Name().c_str() << " called... " << std::endl;
    // Look for a configuration
    if(cfg.contains_pset(this->Name())) {
      std::cout << "I found my config... ("
		<< this->Name()
		<< ") ... dumping it!"
		<< std::endl;
      
      auto const& pset = cfg.get_pset(this->Name());
      
      std::cout << pset.dump() << std::endl;
      
      //      _vtx_min_radius = pset.get<double>("vtx_min_radius");
      
      _verbose = pset.get<bool>("verbosity");

      _EnDepDistanceMaxCut = pset.get<double>("EnDepDistanceMaxCut") ;
      _IPMaxCut            = pset.get<double>("IPMaxCut")            ;
      _OpeningAngleMinCut  = pset.get<double>("OpeningAngleMinCut")  ;
      _MuonEnergyMaxCut    = pset.get<double>("MuonEnergyMaxCut")    ;
      _GammaEnergyMaxCut   = pset.get<double>("GammaEnergyMaxCut")   ;
      _TotalEnergyMaxCut   = pset.get<double>("TotalEnergyMaxCut")   ;
      _MuonEnergyMinCut    = pset.get<double>("MuonEnergyMinCut")    ;
      _GammaEnergyMinCut   = pset.get<double>("GammaEnergyMinCut")   ;
      _TotalEnergyMinCut   = pset.get<double>("TotalEnergyMinCut")   ;
      _TotMomXCutMax       = pset.get<double>("TotMomXCutMax")       ;
      _TotMomYCutMax       = pset.get<double>("TotMomYCutMax")       ;
      _TotMomZCutMax       = pset.get<double>("TotMomZCutMax")       ;
      _TotMomXCutMin       = pset.get<double>("TotMomXCutMin")       ;
      _TotMomYCutMin       = pset.get<double>("TotMomYCutMin")       ;
      _TotMomZCutMin       = pset.get<double>("TotMomZCutMin")       ;

      
    }else{
      std::cout << "oops I didn't find my config... ("
		<< this->Name()
		<< ")"
		<< std::endl;
      throw ERException();
    }
  }
  
  void ERAlgoMode13::ProcessBegin()
  {
    _alg_emp.ProcessBegin();
    _alg_emp.SetMode(true);

    _nMu = 0;
    _nGamma = 0;

    if (_alg_tree) { delete _alg_tree; }
    _alg_tree = new TTree("_alg_tree","Algo Mode13 Tree");
    _alg_tree->Branch("_E",&_E,"_E/D");
    _alg_tree->Branch("_PDG",&_PDG,"PDG/I");
    _alg_tree->Branch("_VsTrack",&_VsTrack,"_VsTrack/I");
    _alg_tree->Branch("_thatE",&_thatE,"thatE/D");
    _alg_tree->Branch("_dEdx",&_dEdx,"_dEdx/D");
    _alg_tree->Branch("_IP",&_IP,"_IP/D");
    _alg_tree->Branch("_IPthisStart",&_IPthisStart,"_IPthisStart/D");
    _alg_tree->Branch("_IPthatStart",&_IPthatStart,"_IPthatStart/D");
    _alg_tree->Branch("_IPtrkBody",&_IPtrkBody,"_IPtrkBody/D");
    _alg_tree->Branch("_distBackAlongTraj",&_distBackAlongTraj,"_distBackAlongTraj/D");
    _alg_tree->Branch("_distToTopWall",&_distToTopWall,"_distToTopWall/D");

    _alg_tree->Branch("_nTrack",&_nTrack,"_nTrack/I");
    _alg_tree->Branch("_nShower",&_nShower,"_nShower/I");
    _alg_tree->Branch("_nMu",&_nMu,"_nMu/I");
    _alg_tree->Branch("_nGamma",&_nGamma,"_nTrack/I");
    _alg_tree->Branch("_EnDepDist",&_EnDepDist,"_EnDepDist/D");
    
    if (_empart_tree) { delete _empart_tree; }
    _empart_tree = new TTree("_empart_tree","EMPart gamma/electron Tagging Tree");
    _empart_tree->Branch("_dedx",&_dedx,"dedx/D");
    _empart_tree->Branch("_radlen",&_radlen,"radlen/D");
    _empart_tree->Branch("_pdg",&_pdg,"pdg/I");

    return;

  }

  bool ERAlgoMode13::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
    auto datacpy = data;
    if (_verbose) { 
      std::cout << "*********** BEGIN PdK RECONSTRUCTION ************" << std::endl;
      std::cout << "Showers in event  : " << data.Shower().size() << std::endl;
      std::cout << "Tracks  in event  : " << data.Track().size() << std::endl;
    }

    // Let's take the number of showers and tracks
    // we will make cuts on showers and tracks quantities (e.g minimum lenght)
    // acceptable showers and tracks < tot showers and tracks
    _nShower = data.Shower().size();
    _nTrack = data.Track().size();

    //Flags to determine if the event passes the cuts
    bool _cEnDepRadiusFlag  = false;
    bool _cIPFlag           = false;
    bool _cOnePlusGammaFlag = false;
    bool _cOnePlusMuFlag    = false;
    bool _cNoVtxActFlag     = false;
    bool _cOpeningAngleFlag = false;

    bool _cMuonEnergyFlag   =  false;
    bool _cGammaEnergyFlag  =  false;
    bool _cTotalEnergyFlag  =  false;
    bool _cTotMomFlag       =  false;


    // This map will store interesting paricles for our topology 
    // This might be a good idea for cosmic sample... maybe not now...
    // std::map<ertool::Particle,int> mode13Map;


    // We have a list of showers.
    // Filter them to select gamma-mu showers
    for (auto const& p : graph.GetParticleNodes(RecoType_t::kShower)){
      auto const& thisShower = datacpy.Shower(graph.GetParticle(p).RecoID());
      // keep track of whether it is mode13
      bool mode13 = true;

      // if we find that this shower shares a vertex with a track -> change "_hassister" to true.
      //      _hassister = false;

      // Keep track of list of siblings found for the shower (will be used to reconstruct full proton decay)
      std::vector<int> siblings;
      
      if (_verbose) { std::cout << "This shower: (" << p << ")" << "\tE: " << thisShower._energy << std::endl; }


      // Make sure the event satisfies pdk conditions: 
      // 1) events has a shower
      
      // 2) events has a track which is at least 3 mm
      for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){
	auto const& thatTrack = datacpy.Track(graph.GetParticle(t).RecoID());
	// make sure track has a length of at least 0.3 cm (wire spacing)
	// greater longer than 3 mm
	if (thatTrack.Length() < 0.3) {_nTrack--; continue;}
	if (_verbose) { std::cout << "Comparing with track (" << t << ")" << std::endl; }
	
	
	// The decay vtx has to correspond with the first energy deposition of the muon
	// unless big screw up with the muon reco
	geoalgo::Point_t vtxPdK(3);
	vtxPdK = thatTrack.front();
	// Calculate the distance between 
	// the first energy deposition of the track and 
	// the first energy deposition of the shower
	_EnDepDist = TMath::Sqrt(vtxPdK.SqDist(thisShower.Start()));
	// check if the event passes the cut on this distance 
	// if not, skip this couple, if yes set the flag to true

	// 3) the shower and the track are within a decent radius
	if (_EnDepDist > _EnDepDistanceMaxCut) continue;
	_cEnDepRadiusFlag = true;

	// I'm using vtx(3) for nothing but the IP calculation. 
	geoalgo::Point_t vtx(3);
	// Calculate the distance impact parameter
	//// get IP for this shower and the entire track
	//// but constrain the search along the track
	//// coordinates for closest points on the two objects
	// 4) the IP of the shower and the track is decent
	double IP =  _findRel.FindClosestApproach(thisShower,thatTrack,vtx); 
	// check if the event passes the cut on the impact parameter 
	// if not, skip this couple, if yes set the flag to true
	if (IP > _IPMaxCut) continue;
	_cIPFlag = true;

	// our shower has a common origin with this track
	if (_verbose) { std::cout << "common origin w/ track!" << std::endl; }

	// 5) Opening angle > OA_min
	openingAngle = thisShower.Dir().Angle(thatTrack.Dir());
	if(openingAngle < _OpeningAngleMinCut) continue;
	_cOpeningAngleFlag = true;
	
	//Now we have a mu and a gamma that passed geometry cuts, 
	//Let play with kinematics!!!

	// taking the muon!
	auto& muon = graph.GetParticle(t);
	TLorentzVector muon4Mom((muon.Momentum())[0],(muon.Momentum())[1],(muon.Momentum())[2], muon.Energy());
	// fill in a particle the gamma properties
	double momGamma = thisShower._energy; //I'm not sure if I should re-set the property of this shower, but ok...
	if (momGamma < 0) { momGamma = 1; throw ERException("Something is very wrong with this shower energy!");}
	if (_verbose) { std::cout << "Getting shower " << p << std::endl; }
	auto& gamma = graph.GetParticle(p);
	if (_verbose) { std::cout << " and modifying properties..." << std::endl; }
	gamma.SetParticleInfo(22,_gamma_mass,thisShower.Start(),thisShower.Dir()*momGamma);
	TLorentzVector gamma4Mom((gamma.Momentum())[0],(gamma.Momentum())[1],(gamma.Momentum())[2], gamma.Energy());
      
	// 6)   MuonEn > MuonEnMin   &&   MuonEn < MuonEnMax
	if( muon.Energy() < _MuonEnergyMinCut) continue;
	if( muon.Energy() > _MuonEnergyMaxCut) continue;
	_cMuonEnergyFlag = true;

	// 7) GammaEn > GammaEnMin  &&  GammaEn < MuonEnMax
	if( gamma.Energy() < _GammaEnergyMinCut) continue;
	if( gamma.Energy() > _GammaEnergyMaxCut) continue;
	_cGammaEnergyFlag = true;
	
	// store siblings only if 
	// shower is a gamma,  
	// track is muons and
	// the have right topology 
	_empart_tree->Fill();
	siblings.push_back(t);
	
	//The following 2 lines might be helful in the future...
	//mode13Map[thisShower] = thisShower.RecoID();
	//mode13Map[thatTrack] = thatTrack.RecoID();

	// 8) the track must be a muon
	if (thatTrack._pid !=4)  continue;
	_cOnePlusMuFlag = true;
	muonMap[thatTrack] = thatTrack.RecoID();  

	// 9) the shower is a gamma
	if (!isGammaLike(thisShower._dedx,vtxPdK.Dist(thisShower.Start()))){
	  _dedx   = thisShower._dedx;
	  _radlen = vtxPdK.Dist(thisShower.Start());
	  _pdg    = 11;
	  if (_verbose) { std::cout << "Shower is electron-like. Ignore " << std::endl; }
	  mode13 = false;
	  elect++;
	  break;
	}else{
	  _cOnePlusGammaFlag = true;
	  _nGamma++;
	  _dedx   = thisShower._dedx;
	  _radlen = vtxPdK.Dist(thisShower.Start());
	  _pdg    = 22;
	  mode13 = true;
	}
	if (!mode13) continue;
	
	
	// Let's pinpoint where we are now.
	// I have 1 shower and 1 mu that passed my constraints
	// now I want to create a common parent, but I've gotta be smart!
        // PAY ATTENTION TO DOUBLE COUNTING ==> maybe create a map of particles? this is especially problematic for cosmics (see note 3)). We'll see. 
	// This is where I'm forming my candidate proton.      
	// Right now, we need to create the sibiling relationship
	// we need to be smart in creating that relationship: 
	//  1] if nor muon nor gamma have parent: asses the relation and you're done
	//  2] if either of the 2 assigned      : calculate score of then new couple
	//                                     check which score is higher old or new?
	//                                        Old is higher: ignore new couple
	//                                        New is higher: distroy old relations, create new one
	
	// If mode13 still true -> we found a candidate! Proceed!
	// The particle with all it's info was already
	// created, do the checking
	if (mode13){
	  if (_verbose) { std::cout << "Mu + Gamma might be Mode13!" << std::endl; }
	  
	  // prepare holder for proton momentum
	  //::geoalgo::Vector_t protonMom(0,0,0);
	  double protonMom = 0;

	  if ((muon.ID() == muon.Parent())&&
	      (gamma.ID() == gamma.Parent()))
	    {
	      // if neither muon and gamma has parent 
	      // implementing 1] : create a new particle: the proton 
	      if (_verbose) { std::cout << "Number of partciles before: " << graph.GetNumParticles() << std::endl; }
	      if (_verbose) { std::cout << "Making original proton..." << std::endl; }
	      Particle& proton = graph.CreateParticle();
	      if (_verbose) { std::cout << "made proton with ID " << proton.ID() << " and PDG: " << proton.PdgCode() << std::endl; }
	      if (_verbose) { std::cout << "number of partciles after: " << graph.GetNumParticles() << std::endl; }
	      
	      TLorentzVector proton4Mom = muon4Mom+gamma4Mom;
	      if (_verbose) {
		std::cout<<"\n\n Checking if energy and momentum of muon, gamma and proton make sense \n";
		std::cout<<" Muon   X: "<<muon4Mom.X()  <<" Y: "<<muon4Mom.Y()  <<"  Z: "<<muon4Mom.Z()  
			 <<" T: "       <<muon4Mom.T()  <<" Mag: "<<muon4Mom.Mag()  <<"\n";
		std::cout<<" Gamma  X: "<<gamma4Mom.X() <<" Y: "<<gamma4Mom.Y() <<"  Z: "<<gamma4Mom.Z() 
			 <<" T: "       <<gamma4Mom.T() <<" Mag: "<<gamma4Mom.Mag() <<"\n";
		std::cout<<" Proton X: "<<proton4Mom.X()<<" Y: "<<proton4Mom.Y()<<"  Z: "<<proton4Mom.Z()
			 <<" T: "       <<proton4Mom.T()<<" Mag: "<<proton4Mom.Mag()<<"\n";
		std::cout<<"\n\n ";
	      }
	      
	      //
	      // 10)   TotEn > TotEnMin    &&    TotEn < TotEnMax
	      if( proton4Mom.T() < _TotalEnergyMinCut) continue;
	      if( proton4Mom.T() > _TotalEnergyMaxCut) continue;
	      _cTotalEnergyFlag = true;
	      // 11) TotMom > TotMomMin    &&    TotMom < TotMomMax
	      if( proton4Mom.X() < _TotMomXCutMin) continue;
	      if( proton4Mom.X() > _TotMomXCutMax) continue;
	      if( proton4Mom.Y() < _TotMomYCutMin) continue;
	      if( proton4Mom.Y() > _TotMomYCutMax) continue;
	      if( proton4Mom.Z() < _TotMomZCutMin) continue;
	      if( proton4Mom.Z() > _TotMomZCutMax) continue;
	      _cTotMomFlag = true;
	      
	      _protonsdKs += 1;
	      ::geoalgo::Vector_t protonMom(proton4Mom.X(),proton4Mom.Y(),proton4Mom.Z()); 
	      proton.SetParticleInfo(2212,proton4Mom.Mag(),thatTrack.front(),protonMom);
	      // the score of the proton tells us how good is the couple...
	      // the further we are from 930 the worst is the couple
	      double scoreMode13 = std::abs(930 - proton4Mom.T());
	      //Modify particle graph so to add all the info of the primary proton
	      graph.SetParentage(proton.ID(),p,scoreMode13);
	      graph.SetParentage(proton.ID(),t,scoreMode13);
	      graph.SetPrimary(proton.ID());
	      muon  = graph.GetParticle(t);
	      gamma = graph.GetParticle(p);

	      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	      ////////////////////////////  I need to understand this part better!!!  ///////////////////////////////////////
	      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	      ::geoalgo::Point_t vtx(3);
	      double score = -1;
	      auto const& rel = _findRel.FindRelation(thisShower,thatTrack,vtx,score);
	      if(_verbose) std::cout<<"\n\n\n Muon-Proton  score: "<<  muon.RelationshipScore(proton.ID())<<"\n"
				    <<" Gamma-Proton score: "<< gamma.RelationshipScore(proton.ID())<<"\n"
				    <<" Muon-Gamma   score: "<<  muon.RelationshipScore(gamma.ID() )<<"\n"
				    <<" Muon-Gamma   Relat: "<< rel<<", if sibilings = "<<kSibling<<"\n"
				    <<" MuonID   : "<< muon.ID()  <<" MuonParent ID   : "<<muon.Parent()<<"\n"
				    <<" GammaID  : "<< gamma.ID() <<" GammaParent ID  : "<<gamma.Parent()<<"\n"
				    <<" ProtonID : "<< proton.ID()<<" ProtonParent ID : "<<muon.Parent()<<"\n";
	      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////	     
	      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////	    
	      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	      
	    }else 
	    {
	      // muon or gamma has parents 
	      // mode 2] needs implementation
	      if ( muon.ID() !=  muon.Parent()) std::cout <<"PUPPA, THE  MUON ALREADY HAS PARENT!!! \n";
	      if (gamma.ID() != gamma.Parent()) std::cout <<"PUPPA, THE GAMMA ALREADY HAS PARENT!!! \n";
	      // throw ERException("Implement this part of the code, you lazy ass!");
	    }
	  
	}// if mode13
	// if not mode13
	else
	  if (_verbose) { std::cout << "Topology is not mode13." << std::endl; }
	
      }// for all tracks
      
    }// for all showers
      
    if (_nShower > 0 )      _cOnePlusShower++; 
    if (_nTrack  > 0 )      _cOnePlusTrack++ ;
    if (_cEnDepRadiusFlag)  _cEnDepRadius++  ;
    if (_cIPFlag)           _cIP++           ;
    if (_cOnePlusGammaFlag) _cOnePlusGamma++ ;
    if (_cOnePlusMuFlag)    _cOnePlusMu++    ;
    if (_cOpeningAngleFlag) _cOpeningAngle++ ;
    if (_cMuonEnergyFlag )  _cMuonEnergy++   ;
    if (_cGammaEnergyFlag)  _cGammaEnergy++  ;
    if (_cTotalEnergyFlag)  _cTotalEnergy++  ;
    if (_cTotMomFlag     )  _cTotMom++       ;

	
    return true;
  }
  
  void ERAlgoMode13::ProcessEnd(TFile* fout)
  {
    std::cout << "Events that have at least 1 track .................................... "<< _cOnePlusTrack << std::endl;
    std::cout << "Events that have at least 1 shower ................................... "<< _cOnePlusShower<< std::endl;
    std::cout << "Events that have at least 1 track, 1 shower and  pass the radius cut . "<< _cEnDepRadius  << std::endl;
    std::cout << "Events that have at least 1 track, 1 shower and  pass the IP cut ..... "<< _cIP           << std::endl;
    std::cout << "Events that pass the opening angle cut ............................... "<< _cOpeningAngle << std::endl;
    // std::cout << "Events for there is no other vertex activity ......................... "<< _cNoVtxAct     << std::endl;
    std::cout << "Events that pass the  Muon Energy cut ................................ "<< _cMuonEnergy   << std::endl;
    std::cout << "Events that pass the Gamma Energy cut ................................ "<< _cGammaEnergy  << std::endl;
    std::cout << "Events for which the track  is a muon ................................ "<< _cOnePlusMu    << std::endl;
    std::cout << "Events for which the shower is a gamma ............................... "<< _cOnePlusGamma << std::endl;
    std::cout << "Events that pass the Total Energy cut ................................ "<< _cTotalEnergy  << std::endl;
    std::cout << "Events that pass the Total Momentum cut .............................. "<< _cTotMom       << std::endl;


    std::cout << "Muons  ........... "<< muonMap.size() << std::endl;
    std::cout << "Gammas ........... "<< _nGamma        << std::endl;
    std::cout << "ProtonDk ......... "<< _protonsdKs    << std::endl;
    
    
    if(fout){
      fout->cd();
      if (_alg_tree)
	_alg_tree->Write();
      if (_empart_tree)
	_empart_tree->Write();
    }
    
    return;
  }
  
  
  bool ERAlgoMode13::isGammaLike(const double dedx, double radlen, bool forceRadLen)
  {
    if ( !_useRadLength && !forceRadLen )
      radlen = -1;
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
  
  
  
  void ERAlgoMode13::ClearTree(){

    _E                 = -1;
    _PDG               = -1;
    _VsTrack           = -1;
    _thatE             = -1;
    _dEdx              = -1;
    _IP                = -1;
    _IPthisStart       = -1;
    _IPthatStart       = -1;
    _IPtrkBody         = -1;
    _distBackAlongTraj = -1;
    _distToTopWall     = -999999;

    _nTrack            = -1;
    _nShower           = -1;
    _nMu               = -1;
    _nGamma            = -1;
    _EnDepDist         = -1;


    return;
  }
 
}

#endif

// Make sure it satisfies pdk conditions: 
// 1)  events has a shower
// 2)  events has a track which is at least 3 mm
// 3)  the shower and the track are within a decent radius
// 4)  the IP of the shower and the track is decent
// 5)  Opening angle > OA_min
// Let's play with kinematics
// 6)  MuonEn  > MuonEnMin   &&   MuonEn < MuonEnMax
// 7)  GammaEn > GammaEnMin  &&  GammaEn < MuonEnMax
// 8)  the track must be a muon
// 9)  the shower is a gamma
// 10) TotEn   > TotEnMin    &&    TotEn < TotEnMax
// 11) TotMom  > TotMomMin   &&   TotMom < TotMomMax
/// Not implemented yet
// 9) No vtx activity

// Notes:
// 1) There is an important CAVEAT regarding the order of the cut flow. 
//    The starting position of the muon is used to identify wheter or not
//    the shower associated with the vertex is a gamma or an electron.
//       
// 2) Do we really want to implement the no vtx activity cut? 
//    Pro : the physics of pdk requires it
//    Pro : the background in the cosmics will give us vtx activity
//    Cons: in recostruction the showers are going to be a mess, I might be cutting out "good events"
//    Thought: since reco of showers is crappy, we can end up asking for the right kinematics of  muon + a shower (no cuts on the energy)
//    Right now, I DON'T WANT TO IMPLEMENT VTX ACTIVITY
// 3) Creating a map that will store interesting paricles for our topology 
//    might be a good idea for cosmic sample... maybe not now...
//    std::map<ertool::Particle,int> mode13Map; or something similar.
//    map might be an overkill in the case of signal only, but might be necessary in the case of
//    cosmics. The idea being that you can end up with more than one "shower + track topology" per event.


// A draft of no vtx activity to be implemented... if we see fit.

      /*
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // NOT YET IMPLEMENTED Becaus I'm not sure we need it with the new strategy
      // 9) No vtx activity
      if (!mode13) continue;
      // if still mode13 go over showers. To find potential siblings (and kill that event)
      for (auto const& p2 : graph.GetParticleNodes(RecoType_t::kShower)){	
	auto const& thatShower = datacpy.Shower(graph.GetParticle(p2).RecoID());
	
	// make sure we don't use "thisShower" in the loop
	if (thatShower.RecoID() == thisShower.RecoID()) continue;
	if (_verbose) { std::cout << "Comparing with shower (" << p2 << ")" << std::endl; }
	
	
	// Right now, I don't care if that shower is e-like or gamma-like: 
	// any other showers associated to the vertex are BAD! No other 
	// vertex activity!!!! 

	// is "thatshower" gamma or e-like?
	// if gamma-like maybe a nearby pi0 -> ok if close
	

	// If the two are correlated and siblings
	// then we have two showers at the vertex -> do not count
	// "thisShower" as a belonging to the PdK
	// compare the two showers
	geoalgo::Point_t vtx(3);
	// let's calculate the vtx between the two showers 
	// and the impact parameter
	double IP = _findRel.FindClosestApproach(thisShower,thatShower,vtx); 
	_VsTrack = 0;
	_thatE   = thatShower._energy;
	_IP = IP;
	_IPthisStart = vtx.Dist(thisShower.Start());
	_IPthatStart = vtx.Dist(thatShower.Start());
	_alg_tree->Fill();
	if (_verbose)
	  std::cout << "\tImpact Parameter      : " << _IP << std::endl
		    << "\tIP to other Shr Start : " << _IPthatStart << std::endl
		    << "\tIP to this Shr Start  : " << _IPthisStart << std::endl;
	if ( (IP < _maxIP) && ( vtx.Dist(thisShower.Start()) < _vtxToShrStartDist) && ( vtx.Dist(thatShower.Start()) < _vtxToShrStartDist) ){
	  mode13 = false;
	  if (_verbose) { std::cout << "NOT mode13 due to showers" << std::endl; }
	  break;
	}
      }// loop over all showers in event
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      */

      /* // We'll think about these cuts  later!!!
      // If mode13 and there are "track-candidate-vertices" formed by 1 shower and 1 track
      // compare shower start distance to these vertices.
      // if more than some threshold not mode13

      double distmin = 1036;
      // get list of potential vertices that come from 2 or more objects
      // sharing a start point
      auto const& candidateVertices = _findRel.GetVertices(graph,2);
      for (auto const& vertex : candidateVertices){
	double thisdist = thisShower.Start().Dist(vertex);
	if ( thisdist < distmin)
	  distmin = thisdist;
      }
      
      if ( mode13 and !_hassister and (_vtxProximityCut != 0) ){
	if ( (distmin != 1036) and (distmin > _vtxProximityCut) ){
	  if(_verbose) { std::cout << "Trk-Vtx found @ distance of " << distmin << ". Shower not mode13!" << std::endl; }
	  mode13 = false;
	}
      }


      // Try and remove any shower that is on top of a track
      // this could be due to a track mis-reconstructed as a 
      // shower.

      for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){
	
	auto const& track = datacpy.Track(graph.GetParticle(t).RecoID());
	
	// check if track and shower start points are close
	if (thisShower.Start().Dist(track[0]) > 1 )
	  continue;
	// if their start point is very close:
	// calculate a stupid track direction
	size_t nsteps = track.size();
	::geoalgo::Vector_t trackDir = track[int(nsteps/2.)]-track[0];
	trackDir.Normalize();
	// get dot product
	double dot = thisShower.Dir()*trackDir;
	if (dot > 0.9)
	  mode13 = false;
      }
      
      // if still mode13 (and no sister track) look at
      // dEdx to determine if e-like
      if (mode13 && !_hassister){
	if ( isGammaLike(thisShower._dedx,-1) || (thisShower._dedx <= 0) || (thisShower._dedx > 10.) ){
	  if (_verbose) { std::cout << "Shower is mode13 but gamma-like -> reject" << std::endl; }
	  mode13 = false;
	}
      }

      */


	      /*	      
	      // Now look for all potential siblins: I don't think I want this part of the code
	      // using AlgoFindRelationship
	      for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){
		
		auto const& track = datacpy.Track(graph.GetParticle(t).RecoID());
		// make sure track has a length of at least 0.3 cm (wire spacing)
		// greater longer than 3 mm
		if (track.Length() < 0.3)
		  continue;
		
		
		::geoalgo::Point_t vtx(3);
		double score = -1;
		auto const& rel = _findRel.FindRelation(thisShower,track,vtx,score);
		if (rel == kSibling)
		  { // add this track to PaticleTree
		    auto &trackParticle = graph.GetParticle(t);
		    // if not primary according to primary finder -> don't add
		    if (!trackParticle.Primary())
		      continue;
		    // track deposited energy
		    double Edep = track._energy;
		    // track direction
		    geoalgo::Vector_t Dir = (track[1]-track[0]);
		    Dir.Normalize();
		    double mass = _findRel.GetMass(track);
		    geoalgo::Vector_t Mom = Dir * ( sqrt( Edep * (Edep+2*mass) ) );
		    trackParticle.SetParticleInfo(_findRel.GetPDG(track),mass,track[0],Mom);
		    protonMom += sqrt( Edep * ( Edep + 2*mass ) );
		    graph.SetParentage(proton.ID(),t);
		  }
	      }
	      */


// REDUNDANT
	/*
	_VsTrack = 1;
	_thatE   = thatTrack._energy;
	_IP = IP;
	_IPthisStart = vtx.Dist(thisShower.Start());// Distance between the first shower energy deposition and vtx as calculated by the IP
	_IPthatStart = vtx.Dist(thatTrack.front()); // Distance between the first track energy deposition and vtx as calculated by the IP
	_IPtrkBody = sqrt(_geoAlgo.SqDist(vtx,thatTrack)); // Distance between the track and vtx as calculated by the IP
	_alg_tree->Fill();
	if (_verbose)
	std::cout << "\tImpact Parameter: " << _IP << std::endl
		  << "\tIP to Trk Start : " << _IPthatStart << std::endl
		  << "\tIP to Trk Body  : " << _IPtrkBody << std::endl
		  << "\tIP to Shr Start : " << _IPthisStart << std::endl;
	
	
	// 6) the distance of the shower and the track is decent (may be redundant)
	/////////////////////////////////////////////////////// This cut might be superfluous....
	if  (vtx.Dist(thatTrack.front())  > _vtxToTrkStartDist) continue ;   // vertex close to track start  
	if  (vtx.Dist(thisShower.Start()) > _vtxToShrStartDist) continue ;   // vertex not unreasonably far from shower start
	_cRedundant = true;
	*/
