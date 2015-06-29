#ifndef ERTOOL_ERALGOSINGLEE_NDKMODEZERO_CXX
#define ERTOOL_ERALGOSINGLEE_NDKMODEZERO_CXX

#include "ERAlgoSingleE_NdkModeZero.h"

namespace ertool {

	size_t single_e_counter = 0;
	size_t total_e_showers = 0;
	size_t total_g_showers = 0;
	size_t nonzero_dedx_counter = 0;

	ERAlgoSingleE_NdkModeZero::ERAlgoSingleE_NdkModeZero(const std::string& name)
	: AlgoBase(name)
	, fTPC(-10.,-126.,-10.,292.,136.,1150.)
	, _empart_tree(nullptr)
	, _alg_tree(nullptr)
	{
		_e_mass     = ParticleMass(11);
		_p_mass		= ParticleMass(2212); // SHOULD WE INCLUDE THE BINDING ENERGY?
		_Ethreshold = 0;
		_verbose = true;
		_useRadLength = false;
		_hassister = false;
		_rejectLongTracks = true;
		_vtxProximityCut = 0;
		_BDtW = 0; 
		_BDtTW = 0;
		_protons = 0;

	}

	void ERAlgoSingleE_NdkModeZero::Reset()
	{
		std::cout<<__FUNCTION__<<" found "<<single_e_counter<<" events with a single electron in it."<<std::endl;
		std::cout<<"and "<<nonzero_dedx_counter<<" nonzero dedx showers"<<std::endl;
		std::cout<<"Found "<<total_e_showers<<" total e showers"<<std::endl;
		std::cout<<"Found "<<total_g_showers<<" total g showers"<<std::endl;
	}

	void ERAlgoSingleE_NdkModeZero::ProcessBegin()
	{

		_alg_emp.ProcessBegin();
		_alg_emp.SetMode(true);

		if (_alg_tree) { delete _alg_tree; }
		_alg_tree = new TTree("_alg_tree","ERAlgo SingleE Tree");
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

		if (_empart_tree) { delete _empart_tree; }
		_empart_tree = new TTree("_empart_tree","EMPart gamma/electron Tagging Tree");
		_empart_tree->Branch("_dedx",&_dedx,"dedx/D");
		_empart_tree->Branch("_radlen",&_radlen,"radlen/D");
		_empart_tree->Branch("_pdg",&_pdg,"pdg/I");

		return;
	}

	void ERAlgoSingleE_NdkModeZero::AcceptPSet(const ::fcllite::PSet& cfg) {

    // Load EMPart params
		_alg_emp.AcceptPSet(cfg);

		return;
	}

	bool ERAlgoSingleE_NdkModeZero::Reconstruct(const EventData &data, ParticleGraph& graph){

		auto datacpy = data;

		if (_verbose) { 
			std::cout << "*********** BEGIN SingleE RECONSTRUCTION ************" << std::endl;
			std::cout << "Showers in event  : " << data.Shower().size() << std::endl;
		}

    // We have a list of primaries.
    // Filter them to select single e- showers

    // loop through primary showers
    // Loop through showers
		for (auto const& p : graph.GetParticleNodes(RecoType_t::kShower)){

			auto const& thisShower = datacpy.Shower(graph.GetParticle(p).RecoID());
      // keep track of whether it is single
			bool single = true;
      // if we find that this shower shares a vertex with a track -> change "_hassister" to true.
			_hassister = false;
      // Keep track of list of siblings found for the shower (will be used to reconstruct full proton interaction)
			std::vector<int> siblings;

			if (_verbose) { std::cout << "This shower: (" << p << ")" << "\tE: " << thisShower._energy << std::endl; }

      // it is primary. Make sure it satisfies SingleE conditions also
      // 1) loop over all showers in event
			for (auto const& p2 : graph.GetParticleNodes(RecoType_t::kShower)){

				auto const& thatShower = datacpy.Shower(graph.GetParticle(p2).RecoID());
				geoalgo::Point_t vtx(3);
	// make sure we don't use "thisShower" in the loop
				if (thatShower.RecoID() == thisShower.RecoID()) 
					continue;
				if (_verbose) { std::cout << "Comparing with shower (" << p2 << ")" << std::endl; }
	// is "thatshower" gamma or e-like?
	// if gamma-like maybe a nearby pi0 -> ok if close
				if (isGammaLike(thatShower._dedx,-1))
					continue;
	// "thatShower" is e-like. If the two are correlated and siblings
	// then we have two e-like showers at the vertex -> do not count
	// "thisShower" as a SingleE
	// compare the two showers
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
					single = false;
					if (_verbose) { std::cout << "NOT single" << std::endl; }
					break;
				}
      }// loop over all showers in event

      // if still single go over tracks. Find potential siblings
      if (!single)
      	continue;

      for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){

      	auto const& thatTrack = datacpy.Track(graph.GetParticle(t).RecoID());
	// make sure track has a length of at least 0.3 cm (wire spacing)
	// greater longer than 3 mm
      	if (thatTrack.Length() < 0.3)
      		continue;
      	if (_verbose) { std::cout << "Comparing with track (" << t << ")" << std::endl; }
      	geoalgo::Point_t vtx(3);
	// compare the two tracks
      	double IP =  _findRel.FindClosestApproach(thisShower,thatTrack,vtx);
      	_VsTrack = 1;
      	_thatE   = thatTrack._energy;
      	_IP = IP;
      	_IPthisStart = vtx.Dist(thisShower.Start());
      	_IPthatStart = vtx.Dist(thatTrack.front());
      	_IPtrkBody = sqrt(_geoAlgo.SqDist(vtx,thatTrack));
      	_alg_tree->Fill();
      	if (_verbose)
      		std::cout << "\tImpact Parameter: " << _IP << std::endl
      	<< "\tIP to Trk Start : " << _IPthatStart << std::endl
      	<< "\tIP to Trk Body  : " << _IPtrkBody << std::endl
      	<< "\tIP to Shr Start : " << _IPthisStart << std::endl;
      	single = true;
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
			single = false;
			break;
		}
		else{
			_dedx   = thisShower._dedx;
			_radlen = vtx.Dist(thisShower.Start());
			_pdg    = 11;
			single = true;
		}
		_empart_tree->Fill();

	  }// if common origin with primary!
      }// for all tracks

      // If single and there are "track-candidate-vertices" formed by 2 or more tracks
      // compare shower start distance to these vertices.
      // if more than some threshold not single
      double distmin = 1036;
      // get list of potential vertices that come from 2 or more objects
      // sharing a start point
      auto const& candidateVertices = _findRel.GetVertices(graph,2);
      for (auto const& vertex : candidateVertices){
      	double thisdist = thisShower.Start().Dist(vertex);
      	if ( thisdist < distmin)
      		distmin = thisdist;
      }

      if ( single and !_hassister and (_vtxProximityCut != 0) ){
      	if ( (distmin != 1036) and (distmin > _vtxProximityCut) ){
      		if(_verbose) { std::cout << "Trk-Vtx found @ distance of " << distmin << ". Shower not single!" << std::endl; }
      		single = false;
      	}
      }

      // if still single (and no sister track) look at
      // dEdx to determine if e-like
      if (single && !_hassister){
      	if ( isGammaLike(thisShower._dedx,-1) || (thisShower._dedx <= 0) || (thisShower._dedx > 10.) ){
      		if (_verbose) { std::cout << "Shower is single but gamma-like -> reject" << std::endl; }
      		single = false;
      	}
      }

      //If single still true -> we found it! Proceed!
      // the particle with all it's info was already
      // created, simply add it to the result vector
      if (single){
      	if (_verbose) { std::cout << "Shower is Single!" << std::endl; }

	// prepare holder for proton momentum
	//::geoERAlgo::Vector_t protonMom(0,0,0);
      	double protonMom = 0;

	// fill in electron properties
      	double mom = sqrt( (thisShower._energy)*(thisShower._energy) - (_e_mass*_e_mass) );
      	if (mom < 0) { mom = 1; }
      	if (_verbose) { std::cout << "Getting shower " << p << std::endl; }
      	auto& electron = graph.GetParticle(p);
      	if (_verbose) { std::cout << " and modifying properties..." << std::endl; }
      	electron.SetParticleInfo(11,_e_mass,thisShower.Start(),thisShower.Dir()*mom);
	// create a new particle for the proton!
      	if (_verbose) { std::cout << "number of particles before: " << graph.GetNumParticles() << std::endl; }
      	if (_verbose) { std::cout << "Making proton..." << std::endl; }
      	Particle& proton = graph.CreateParticle();
	protonMom += mom;//thisShower.Dir()*mom;

	if (_verbose) { std::cout << "made proton with ID " << proton.ID() << " and PDG: " << proton.PdgCode() << std::endl; }
	if (_verbose) { std::cout << "number of partciles after: " << graph.GetNumParticles() << std::endl; }
	_protons += 1;
	// set relationship
	// THIS DOESN'T WORK NEEDS FIXING!!!!
	// graph.SetParentage(proton.ID(),p);


// // NOT SURE IF THIS IS THE TIME TO DO THIS YET!!!
// 	// Now look for all potential siblins
// 	// using ERAlgoFindRelationship
// 	for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){

// 		auto const& track = datacpy.Track(graph.GetParticle(t).RecoID());
// 	  // make sure track has a length of at least 0.3 cm (wire spacing)
// 	  // greater longer than 3 mm
// 		if (track.Length() < 0.3)
// 			continue;

// 		::geoalgo::Point_t vtx(3);
// 		double score = -1;
// 		auto const& rel = _findRel.FindRelation(thisShower,track,vtx,score);
// 		if (rel == kSibling)
// 	    { // add this track to PaticleTree
// 	    	auto &trackParticle = graph.GetParticle(t);
// 	      // if not primary according to primary finder -> don't add
// 	    	if (!trackParticle.Primary())
// 	    		continue;
// 	      // track deposited energy
// 	    	double Edep = track._energy;
// 	      // track direction
// 	    	geoalgo::Vector_t Dir = (track[1]-track[0]);
// 	    	Dir.Normalize();
// 	    	double mass = _findRel.GetMass(track);
// 	    	geoalgo::Vector_t Mom = Dir * ( sqrt( Edep * (Edep+2*mass) ) );
// 	    	trackParticle.SetParticleInfo(_findRel.GetPDG(track),mass,track[0],Mom);
// 	    	protonMom += sqrt( Edep * ( Edep + 2*mass ) );
// 	    	graph.SetParentage(proton.ID(),t);
// 	    }
// 	}
// 	::geoalgo::Vector_t momdir(0,0,1);

// 	proton.SetParticleInfo(2212,_p_mass,thisShower.Start(),momdir*protonMom);
	

      }// if single
      // if not single
      else
      	if (_verbose) { std::cout << "Shower is not single." << std::endl; }


      
    }// for all primaries found / for all showers
    
    return true;
}

void ERAlgoSingleE_NdkModeZero::ProcessEnd(TFile* fout){

	std::cout << "Num. of protons found: " << _protons << std::endl;

	if(fout){
		fout->cd();

		if (_alg_tree)
			_alg_tree->Write();
		if (_empart_tree)
			_empart_tree->Write();
	}

	return;
}


bool ERAlgoSingleE_NdkModeZero::isGammaLike(const double dedx, double radlen, bool forceRadLen)
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



void ERAlgoSingleE_NdkModeZero::ClearTree(){

	_E = -1;
	_PDG = -1;
	_VsTrack = -1;
	_thatE = -1;
	_dEdx = -1;
	_IP = -1;
	_IPthisStart = -1;
	_IPthatStart = -1;
	_IPtrkBody = -1;
	_distBackAlongTraj = -1;
	_distToTopWall  = -999999;

	return;
}


}

#endif
