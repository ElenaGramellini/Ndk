// Incomplete list of TO DO
// [ ] Feed the cuts parameters from cgf file, not like the dick of the dog.

#ifndef ERTOOL_ERALGOMODE13_CXX
#define ERTOOL_ERALGOMODE13_CXX

#include "ERAlgoMode13.h"

namespace ertool {
  size_t single_e_counter = 0;
  size_t total_e_showers = 0;
  size_t total_g_showers = 0;
  size_t nonzero_dedx_counter = 0;

  double EnDepMax        = 99999999.;
  double IPMax           = 99999999.;
  double OpeningAngleMax = 99999999.;

  
  ERAlgoMode13::ERAlgoMode13(const std::string& name) 
  : AlgoBase(name)
    , fTPC(-10.,-126.,-10.,292.,136.,1150.)
    , _empart_tree(nullptr)
    , _alg_tree(nullptr)
  {
    _e_mass           = ParticleMass(11);
    _Ethreshold       = 0;
    _verbose          = false;
    _useRadLength     = false;
    _hassister        = false;
    _rejectLongTracks = true;
    _vtxProximityCut = 0;
    _BDtW            = 0; 
    _BDtTW           = 0;
    _neutrinos       = 0;

    _cOnePlusTrack   = 0;
    _cOnePlusShower  = 0;
    _cEnDepRadius    = 0;
    _cIP             = 0;
    _cOnePlusGamma   = 0;
    _cOnePlusMu      = 0;
    _cNoVtxAct       = 0;
    _cOpeningAngle   = 0;

  }

  void ERAlgoMode13::Reset()
  {
    std::cout<<__FUNCTION__<<" found "<<single_e_counter<<" events with a single electron in it."<<std::endl;
    std::cout<<"and "<<nonzero_dedx_counter<<" nonzero dedx showers"<<std::endl;
    std::cout<<"Found "<<total_e_showers<<" total e showers"<<std::endl;
    std::cout<<"Found "<<total_g_showers<<" total g showers"<<std::endl;
  }

  void ERAlgoMode13::AcceptPSet(const ::fcllite::PSet& cfg)
  {
     // Load EMPart params
    _alg_emp.AcceptPSet(cfg);
  }

  void ERAlgoMode13::ProcessBegin()
  {
    _alg_emp.ProcessBegin();
    _alg_emp.SetMode(true);

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
    //  acceptable showers and tracks < tot showers and tracks
    _nShower = data.Shower().size();
    _nTrack = data.Track().size();

    //Flags to determine if the event passes the cuts
    bool _cEnDepRadiusFlag  = false;
    bool _cIPFlag           = false;
    bool _cOnePlusGammaFlag = false;
    bool _cOnePlusMuFlag    = false;
    bool _cNoVtxActFlag     = false;
    bool _cOpeningAngleFlag = false;


    // We have a list of showers.
    // Filter them to select gamma-mu showers
    for (auto const& p : graph.GetParticleNodes(RecoType_t::kShower)){
      auto const& thisShower = datacpy.Shower(graph.GetParticle(p).RecoID());
      // keep track of whether it is mode13
      bool mode13 = true;
      // if we find that this shower shares a vertex with a track -> change "_hassister" to true.
      _hassister = false;
      // Keep track of list of siblings found for the shower (will be used to reconstruct full proton decay)
      std::vector<int> siblings;
      
      if (_verbose) { std::cout << "This shower: (" << p << ")" << "\tE: " << thisShower._energy << std::endl; }
      


      // Make sure it satisfies pdk conditions 
      // 1) loop over all tracks in event to make a vtx
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
	if (_EnDepDist > EnDepMax) continue;
	_cEnDepRadiusFlag = true;

	// !!! CAVEAT !!!
	// I'm using vtx(3) for nothing but the IP calculation. 
	// Possible room for improvement: this vtx should be in between the 1st energy depositions?
	geoalgo::Point_t vtx(3);
	// Calculate the distance impact parameter
	double IP =  _findRel.FindClosestApproach(thisShower,thatTrack,vtx); 
	// check if the event passes the cut on the impact parameter 
	// if not, skip this couple, if yes set the flag to true
	if (IP > IPMax) continue;
	_cIPFlag = true;
 
	_VsTrack = 1;
	_thatE   = thatTrack._energy;
	_IP = IP;
	_IPthisStart = vtxPdK.Dist(thisShower.Start());
	_IPthatStart = vtxPdK.Dist(thatTrack.front());
	_IPtrkBody = sqrt(_geoAlgo.SqDist(vtxPdK,thatTrack));
	_alg_tree->Fill();
	if (_verbose)
	std::cout << "\tImpact Parameter: " << _IP << std::endl
		  << "\tIP to Trk Start : " << _IPthatStart << std::endl
		  << "\tIP to Trk Body  : " << _IPtrkBody << std::endl
		  << "\tIP to Shr Start : " << _IPthisStart << std::endl;
	
	mode13 = true;

	///////////////////////////////////////////////////////
	/////////////////////////////////////////////////////// This is where I gave up
	///////////////////////////////////////////////////////

	std::cout<<"_maxIP "<<_maxIP<<"\n";
	if ( (IP < _maxIP)                                              // good correlation
	     && (vtx.Dist(thatTrack.front()) < _vtxToTrkStartDist)      // vertex close to track start
	     && (vtx.Dist(thisShower.Start()) < _vtxToShrStartDist) )   // vertex not unreasonably far from shower start
	  {
	    // our shower has a common origin with this track
	    // they are siblings
	    if (_verbose) { std::cout << "common origin w/ track!" << std::endl; }
	    _hassister = true;
	    siblings.push_back(t);
	    
	    if (isGammaLike(thisShower._dedx,vtx.Dist(thisShower.Start()))){
	      _dedx   = thisShower._dedx;
	      _radlen = vtx.Dist(thisShower.Start());
	      _pdg    = 22;
	      if (_verbose) { std::cout << "Shower is gamma-like. Ignore " << std::endl; }
	      mode13 = false;
	      break;
	    }
	    else{
	      _dedx   = thisShower._dedx;
	      _radlen = vtx.Dist(thisShower.Start());
	      _pdg    = 11;
	      mode13 = true;
	    }
	    _empart_tree->Fill();
	    
	  }// if common origin with primary!
      }// for all tracks


      // if still mode13 go over showers. To find potential siblings (and kill that event)
      if (!mode13) continue;

      /*
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // 1) loop over all showers in event to make sure
      //    we don't have sibilings showers
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
	// if (isGammaLike(thatShower._dedx,-1)) continue;
	

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



      // If mode13 and there are "track-candidate-vertices" formed by 2 or more tracks
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

      //If mode13 still true -> we found it! Proceed!
      // the particle with all it's info was already
      // created, simply add it to the result vector
      if (mode13){
	if (_verbose) { std::cout << "Shower is Mode13!" << std::endl; }

	// prepare holder for neutrino momentum
	//::geoalgo::Vector_t neutrinoMom(0,0,0);
	double neutrinoMom = 0;

	// fill in electron properties
	double mom = sqrt( (thisShower._energy)*(thisShower._energy) - (_e_mass*_e_mass) );
	if (mom < 0) { mom = 1; }
	if (_verbose) { std::cout << "Getting shower " << p << std::endl; }
	auto& electron = graph.GetParticle(p);
	if (_verbose) { std::cout << " and modifying properties..." << std::endl; }
	electron.SetParticleInfo(11,_e_mass,thisShower.Start(),thisShower.Dir()*mom);
	// create a new particle for the neutrino!
	if (_verbose) { std::cout << "number of partciles before: " << graph.GetNumParticles() << std::endl; }
	if (_verbose) { std::cout << "Making neutrino..." << std::endl; }
	Particle& neutrino = graph.CreateParticle();
	neutrinoMom += mom;//thisShower.Dir()*mom;
	//neutrino.SetParticleInfo(12,0.,thisShower.Start(),thisShower.Dir()*mom);
	if (_verbose) { std::cout << "made neutrino with ID " << neutrino.ID() << " and PDG: " << neutrino.PdgCode() << std::endl; }
	if (_verbose) { std::cout << "number of partciles after: " << graph.GetNumParticles() << std::endl; }
	_neutrinos += 1;
	// set relationship
	graph.SetParentage(neutrino.ID(),p);

	// Now look for all potential siblins
	// using AlgoFindRelationship
	for (auto const& t : graph.GetPrimaryNodes(RecoType_t::kTrack)){
	  
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
	      neutrinoMom += sqrt( Edep * ( Edep + 2*mass ) );
	      graph.SetParentage(neutrino.ID(),t);
	    }
	}

	::geoalgo::Vector_t momdir(0,0,1);

	neutrino.SetParticleInfo(12,0.,thisShower.Start(),momdir*neutrinoMom);

	

      }// if mode13
      // if not mode13
      else
	if (_verbose) { std::cout << "Shower is not mode13." << std::endl; }


      
    }// for all showers
    
    if (_nShower)          _cOnePlusShower++; 
    if (_nTrack )          _cOnePlusTrack++ ;
    if (_cEnDepRadiusFlag) _cEnDepRadius++  ;
    if (_cIPFlag)          _cIP++           ;
	
    return true;
  }
  
  void ERAlgoMode13::ProcessEnd(TFile* fout)
  {
    std::cout << "Events that have at least 1 track .................................... "<< _cOnePlusTrack << std::endl;
    std::cout << "Events that have at least 1 shower ................................... "<< _cOnePlusShower<< std::endl;
    std::cout << "Events that have at least 1 track, 1 shower and  pass the radius cut . "<< _cEnDepRadius  << std::endl;
    std::cout << "Events that have at least 1 track, 1 shower and  pass the IP cut ..... "<< _cIP           << std::endl;
    std::cout << "Events for which the shower is a gamma ............................... "<< _cOnePlusGamma << std::endl;
    std::cout << "Events for which the track  is a muon ................................ "<< _cOnePlusMu    << std::endl;
    std::cout << "Events for there is no other vertex activity ......................... "<< _cNoVtxAct     << std::endl;
    std::cout << "Events that pass the opening angle cut ............................... "<< _cOpeningAngle << std::endl;
    
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
    _EnDepDist                 = -1;


    return;
  }
 
}

#endif

