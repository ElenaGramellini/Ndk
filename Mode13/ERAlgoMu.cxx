#ifndef ERTOOL_ERALGOMU_CXX
#define ERTOOL_ERALGOMU_CXX

#include "ERAlgoMu.h"

namespace ertool {
  int crazy = 0;
  int zero  = 0;

  ERAlgoMu::ERAlgoMu(const std::string& name) 
  : AlgoBase(name)
  , _algoMu_tree(nullptr)
  {
    // histogram to hold the energy of each reconstructed michel electron
    
    // set verbosity to be off by default
    _verbose = false;
  }

  void ERAlgoMu::Reset()
  {   }

  void ERAlgoMu::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    return;
  }

  void ERAlgoMu::ProcessBegin()
  {

    if (_algoMu_tree) { delete _algoMu_tree; }
    _algoMu_tree = new TTree("_algoMu_tree","algoMu Tree");

    _algoMu_tree->Branch("n_tracks"  , &n_tracks, "n_tracks/I");

    _algoMu_tree->Branch("_tracks_size"        , "vector<int>"    , &_tracks_size        );
    _algoMu_tree->Branch("_tracks_steps"       , "vector<double>" , &_tracks_steps       ); 
    _algoMu_tree->Branch("_tracks_stepsDensity", "vector<double>" , &_tracks_stepsDensity);

    _algoMu_tree->Branch("_tracks_den1"   , "vector<double>" , &_tracks_den1 );
    _algoMu_tree->Branch("_tracks_den2"   , "vector<double>" , &_tracks_den2 );
    _algoMu_tree->Branch("_tracks_den3"   , "vector<double>" , &_tracks_den3 );

    _algoMu_tree->Branch("_mu_begEndLength" , "vector<double>" , &_mu_begEndLength );
    _algoMu_tree->Branch("_mu_lengthRatio"  , "vector<double>" , &_mu_lengthRatio  );

    _algoMu_tree->Branch("_mu_ReducedLength20", "vector<double>" , &_mu_ReducedLength20);
    _algoMu_tree->Branch("_mu_ReducedLength10", "vector<double>" , &_mu_ReducedLength10);
    _algoMu_tree->Branch("_mu_ReducedLength5" , "vector<double>" , &_mu_ReducedLength5 );
    _algoMu_tree->Branch("_mu_ReducedLength2" , "vector<double>" , &_mu_ReducedLength2 );
    _algoMu_tree->Branch("_mu_ReducedLength1" , "vector<double>" , &_mu_ReducedLength1 );

    _algoMu_tree->Branch("_mu_FakeLength20"   , "vector<double>" , &_mu_FakeLength20   );
    _algoMu_tree->Branch("_mu_FakeLength10"   , "vector<double>" , &_mu_FakeLength10   );
    _algoMu_tree->Branch("_mu_FakeLength5"    , "vector<double>" , &_mu_FakeLength5    );
    _algoMu_tree->Branch("_mu_FakeLength2"    , "vector<double>" , &_mu_FakeLength2    );
    _algoMu_tree->Branch("_mu_FakeLength1"    , "vector<double>" , &_mu_FakeLength1    );
    

    _algoMu_tree->Branch("_mu_ReducedLengthThirds20", "vector<double>" , &_mu_ReducedLengthThirds20);
    _algoMu_tree->Branch("_mu_ReducedLengthThirds10", "vector<double>" , &_mu_ReducedLengthThirds10);
    _algoMu_tree->Branch("_mu_ReducedLengthThirds5" , "vector<double>" , &_mu_ReducedLengthThirds5 );
    _algoMu_tree->Branch("_mu_ReducedLengthThirds2" , "vector<double>" , &_mu_ReducedLengthThirds2 );
    _algoMu_tree->Branch("_mu_ReducedLengthThirds1" , "vector<double>" , &_mu_ReducedLengthThirds1 );

    _algoMu_tree->Branch("_mu_FakeLengthThirds20"   , "vector<double>" , &_mu_FakeLengthThirds20   );
    _algoMu_tree->Branch("_mu_FakeLengthThirds10"   , "vector<double>" , &_mu_FakeLengthThirds10   );
    _algoMu_tree->Branch("_mu_FakeLengthThirds5"    , "vector<double>" , &_mu_FakeLengthThirds5    );
    _algoMu_tree->Branch("_mu_FakeLengthThirds2"    , "vector<double>" , &_mu_FakeLengthThirds2    );
    _algoMu_tree->Branch("_mu_FakeLengthThirds1"    , "vector<double>" , &_mu_FakeLengthThirds1    );



    _algoMu_tree->Branch("_mu_ReducedLengthHoles20", "vector<double>" , &_mu_ReducedLengthHoles20);
    _algoMu_tree->Branch("_mu_ReducedLengthHoles10", "vector<double>" , &_mu_ReducedLengthHoles10);
    _algoMu_tree->Branch("_mu_ReducedLengthHoles5" , "vector<double>" , &_mu_ReducedLengthHoles5 );
    _algoMu_tree->Branch("_mu_ReducedLengthHoles2" , "vector<double>" , &_mu_ReducedLengthHoles2 );
    _algoMu_tree->Branch("_mu_ReducedLengthHoles1" , "vector<double>" , &_mu_ReducedLengthHoles1 );

    _algoMu_tree->Branch("_mu_FakeLengthHoles20"   , "vector<double>" , &_mu_FakeLengthHoles20   );
    _algoMu_tree->Branch("_mu_FakeLengthHoles10"   , "vector<double>" , &_mu_FakeLengthHoles10   );
    _algoMu_tree->Branch("_mu_FakeLengthHoles5"    , "vector<double>" , &_mu_FakeLengthHoles5    );
    _algoMu_tree->Branch("_mu_FakeLengthHoles2"    , "vector<double>" , &_mu_FakeLengthHoles2    );
    _algoMu_tree->Branch("_mu_FakeLengthHoles1"    , "vector<double>" , &_mu_FakeLengthHoles1    );


    _algoMu_tree->Branch("_mu_En"    , "vector<double>" , &_mu_En    );
    _algoMu_tree->Branch("_mu_DepEn" , "vector<double>" , &_mu_DepEn );
    _algoMu_tree->Branch("_mu_Mom"   , "vector<double>" , &_mu_Mom   );
    _algoMu_tree->Branch("_mu_px"    , "vector<double>" , &_mu_px    );
    _algoMu_tree->Branch("_mu_py"    , "vector<double>" , &_mu_py    );
    _algoMu_tree->Branch("_mu_pz"    , "vector<double>" , &_mu_pz    );

    _algoMu_tree->Branch("_mu_Pdg"   , "vector<int>"    , &_mu_Pdg   );
						          
    _algoMu_tree->Branch("_mu_x"     , "vector<double>" , &_mu_x     );
    _algoMu_tree->Branch("_mu_y"     , "vector<double>" , &_mu_y     );
    _algoMu_tree->Branch("_mu_z"     , "vector<double>" , &_mu_z     );
    _algoMu_tree->Branch("_mu_xEnd"  , "vector<double>" , &_mu_xEnd  );
    _algoMu_tree->Branch("_mu_yEnd"  , "vector<double>" , &_mu_yEnd  );
    _algoMu_tree->Branch("_mu_zEnd"  , "vector<double>" , &_mu_zEnd  );
						          

    _algoMu_tree->Branch("_mu_Refx"   , "vector<double>" , &_mu_Refx   );
    _algoMu_tree->Branch("_mu_Refy"   , "vector<double>" , &_mu_Refy   );
    _algoMu_tree->Branch("_mu_Refz"   , "vector<double>" , &_mu_Refz   );
    _algoMu_tree->Branch("_mu_RefxEnd", "vector<double>" , &_mu_RefxEnd);
    _algoMu_tree->Branch("_mu_RefyEnd", "vector<double>" , &_mu_RefyEnd);
    _algoMu_tree->Branch("_mu_RefzEnd", "vector<double>" , &_mu_RefzEnd);


    _algoMu_tree->Branch("_mu_EnMPS" , "vector<double>" , &_mu_EnMPS );
    _algoMu_tree->Branch("_mu_MomMPS", "vector<double>" , &_mu_MomMPS);
    _algoMu_tree->Branch("_mu_pxMPS" , "vector<double>" , &_mu_pxMPS );
    _algoMu_tree->Branch("_mu_pyMPS" , "vector<double>" , &_mu_pyMPS );
    _algoMu_tree->Branch("_mu_pzMPS" , "vector<double>" , &_mu_pzMPS );
    _algoMu_tree->Branch("_mu_leng"  , "vector<double>" , &_mu_leng  );
    
    return;
  }

  bool ERAlgoMu::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
    ClearTree(); // before filling the info for new event, clean the vectors

    auto datacpy = data;
    int Pdg = -1; 
    
    //Count number of tracks in the event
    n_tracks = graph.GetParticleNodes(RecoType_t::kTrack).size();

    // Loop through Particles associated with a track
    for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){
      auto const& particleFromDataP = graph.GetParticle(t);
      auto const& track = datacpy.Track(particleFromDataP.RecoID());
     
      //Assign particle identification
      if ((track._pid_score[Track::kProton]<track._pid_score[Track::kPion])&&
	  (track._pid_score[Track::kProton]<track._pid_score[Track::kKaon])&&
	  (track._pid_score[Track::kProton]<track._pid_score[Track::kMuon]))      Pdg = 2212;
      
      if ((track._pid_score[Track::kPion]<track._pid_score[Track::kProton])&&
	  (track._pid_score[Track::kPion]<track._pid_score[Track::kKaon])&&
	  (track._pid_score[Track::kPion]<track._pid_score[Track::kMuon]))        Pdg = 211; 
      
      if ((track._pid_score[Track::kKaon]<track._pid_score[Track::kProton])&&
	  (track._pid_score[Track::kKaon]<track._pid_score[Track::kPion])&&
	  (track._pid_score[Track::kKaon]<track._pid_score[Track::kMuon]))        Pdg = 321;

      if ((track._pid_score[Track::kMuon]<track._pid_score[Track::kProton])&&
	  (track._pid_score[Track::kMuon]<track._pid_score[Track::kPion])&&
	  (track._pid_score[Track::kMuon]<track._pid_score[Track::kKaon]))        Pdg = 13;      
      
      
      // track deposited energy from calorimetry obj
      double Edep = track._energy;
      // track trajectory length
      double length = track.Length();
      // length between first and last energy deposition
      auto begEnd = track.front() - track.back();
      double begEndLength = begEnd.Length();
      //ratio between the 2 lengths
      double lengthRatio = begEndLength/length;
      


      // track direction, this is screwed up if beginning of track is WRONG
      geoalgo::Vector_t Dir = (track[1]-track[0]);
      Dir.Normalize();
      
      double mass = ParticleMass(Pdg);
      double Energy  = Edep + mass;
      double Mom_Mag = sqrt( Energy*Energy - mass*mass );
      geoalgo::Vector_t Mom = Dir *  Mom_Mag; 

      double fakeLength20 = 0 ; // trying to subtrack length of the shower
      double fakeLength10 = 0 ; // trying to subtrack length of the shower
      double fakeLength5  = 0 ; // trying to subtrack length of the shower
      double fakeLength2  = 0 ; // trying to subtrack length of the shower
      double fakeLength1  = 0 ; // trying to subtrack length of the shower

      double fakeLengthThirds20 = 0 ; // trying to subtrack length of the shower
      double fakeLengthThirds10 = 0 ; // trying to subtrack length of the shower
      double fakeLengthThirds5  = 0 ; // trying to subtrack length of the shower
      double fakeLengthThirds2  = 0 ; // trying to subtrack length of the shower
      double fakeLengthThirds1  = 0 ; // trying to subtrack length of the shower

      double fakeLengthHoles20 = 0 ; // trying to subtrack length of the shower
      double fakeLengthHoles10 = 0 ; // trying to subtrack length of the shower
      double fakeLengthHoles5  = 0 ; // trying to subtrack length of the shower
      double fakeLengthHoles2  = 0 ; // trying to subtrack length of the shower
      double fakeLengthHoles1  = 0 ; // trying to subtrack length of the shower

      double len1 = 0;	
      double len2 = 0;	
      double len3 = 0;	

      double chunck = (double) track.size()/5.;
      
      int hole20Ini = 0 ;  int hole20Fin = 0 ;
      int hole10Ini = 0 ;  int hole10Fin = 0 ;
      int hole5Ini  = 0 ;  int hole5Fin  = 0 ;
      int hole2Ini  = 0 ;  int hole2Fin  = 0 ;
      int hole1Ini  = 0 ;  int hole1Fin  = 0 ;

      _tracks_steps.clear();
      for (int i = 0; i < track.size(); i++){
	if (track[i] == track.back()) continue;
	if (track.size() < 100) continue;
	auto trajBit = track[i] - track[i+1];
	auto stepLength = trajBit.Length();

	if (stepLength > 20)  fakeLength20 += stepLength ;
	if (stepLength > 10)  fakeLength10 += stepLength ;
	if (stepLength > 5 )  fakeLength5  += stepLength ;
	if (stepLength > 2 )  fakeLength2  += stepLength ;
	if (stepLength > 1 )  fakeLength1  += stepLength ;

	
	if (i < chunck) {
	  len1+=trajBit.Length();
	  if (stepLength > 20) {hole20Ini= i; fakeLengthThirds20 += stepLength ;}
	  if (stepLength > 10) {hole10Ini= i; fakeLengthThirds10 += stepLength ;}
	  if (stepLength > 5 ) {hole5Ini = i; fakeLengthThirds5  += stepLength ;}
	  if (stepLength > 2 ) {hole2Ini = i; fakeLengthThirds2  += stepLength ;}
	  if (stepLength > 1 ) {hole1Ini = i; fakeLengthThirds1  += stepLength ;}

	}else if ((i >= chunck)&&(i < chunck*4)){
	  len2+=trajBit.Length();
	} else{ 
	  len3+=trajBit.Length();
	  if (stepLength > 20) {fakeLengthThirds20 += stepLength ; if (!hole20Fin) hole20Fin= i; }
	  if (stepLength > 10) {fakeLengthThirds10 += stepLength ; if (!hole10Fin) hole10Fin= i; }
	  if (stepLength > 5 ) {fakeLengthThirds5  += stepLength ; if (!hole5Fin ) hole5Fin = i; }
	  if (stepLength > 2 ) {fakeLengthThirds2  += stepLength ; if (!hole2Fin ) hole2Fin = i; }
	  if (stepLength > 1 ) {fakeLengthThirds1  += stepLength ; if (!hole1Fin ) hole1Fin = i; }
	}
	
	/*
	_tracks_steps.push_back(trajBit.Length());
	yetAnOtherLeng+=trajBit.Length();
	if (stepLength > 20) {  _mu_leng.push_back( yetAnOtherLeng); yetAnOtherLeng=0;}
	*/
      }// end of points loop


      if (_verbose){
	std::cout<<"track.size() "<<track.size()<<"\n";
	std::cout<<"hole20Ini "<<hole20Ini<<"\n";
	std::cout<<"hole10Ini "<<hole10Ini<<"\n";
	std::cout<<"hole5Ini  "<<hole5Ini <<"\n";
	std::cout<<"hole2Ini  "<<hole2Ini <<"\n";
	std::cout<<"hole1Ini  "<<hole1Ini <<"\n";
	
	std::cout<<"hole20Fin "<<hole20Fin<<"\n";
	std::cout<<"hole10Fin "<<hole10Fin<<"\n";
	std::cout<<"hole5Fin  "<<hole5Fin <<"\n";
	std::cout<<"hole2Fin  "<<hole2Fin <<"\n";
	std::cout<<"hole1Fin  "<<hole1Fin <<"\n";
      }	

      for (int i = 0; i < track.size(); i++){
	if (track[i] == track.back()) continue;
	if (track.size() < 100) continue;
	auto trajBit = track[i] - track[i+1];
	auto stepLength = trajBit.Length();
       
	if ((i <= hole20Ini)&&(hole20Ini)) {fakeLengthHoles20 += stepLength ;}
	if ((i <= hole10Ini)&&(hole10Ini)) {fakeLengthHoles10 += stepLength ;}
	if ((i <= hole5Ini )&&(hole5Ini )) {fakeLengthHoles5  += stepLength ;}
	if ((i <= hole2Ini )&&(hole2Ini )) {fakeLengthHoles2  += stepLength ;}
	if ((i <= hole1Ini )&&(hole1Ini )) {fakeLengthHoles1  += stepLength ;}
	
	if ((i >= hole20Fin)&&(hole20Fin)) {fakeLengthHoles20 += stepLength ;}
	if ((i >= hole10Fin)&&(hole10Fin)) {fakeLengthHoles10 += stepLength ;}
	if ((i >= hole5Fin )&&(hole5Fin )) {fakeLengthHoles5  += stepLength ;}
	if ((i >= hole2Fin )&&(hole2Fin )) {fakeLengthHoles2  += stepLength ;}
	if ((i >= hole1Fin )&&(hole1Fin )) {fakeLengthHoles1  += stepLength ;}
	
	}

      if(hole5Ini) {
	if (abs(track[hole5Ini][0]-128) < abs(track[hole5Fin][0]-128)) 
	  {_mu_Refx.push_back(track[hole5Ini][0]);}
	else{_mu_Refx.push_back(track[hole5Fin][0]);}         
	
	if (abs(track[hole5Ini][1]) < abs(track[hole5Fin][1])) 
	  {_mu_Refy.push_back(track[hole5Ini][1]);}
	else{_mu_Refy.push_back(track[hole5Fin][1]);}         
	
	if (abs(track[hole5Ini][2]-518) < abs(track[hole5Fin][2]-518)) 
	  {_mu_Refz.push_back(track[hole5Ini][2]);}
	else{_mu_Refz.push_back(track[hole5Fin][2]);}
      }else if(hole5Fin) {
	if (abs(track[hole5Ini][0]-128) < abs(track[hole5Fin][0]-128)) 
	  {_mu_Refx.push_back(track[hole5Ini][0]);}
	else{_mu_Refx.push_back(track[hole5Fin][0]);}         
	
	if (abs(track[hole5Ini][1]) < abs(track[hole5Fin][1])) 
	  {_mu_Refy.push_back(track[hole5Ini][1]);}
	else{_mu_Refy.push_back(track[hole5Fin][1]);}         
	
	if (abs(track[hole5Ini][2]-518) < abs(track[hole5Fin][2]-518)) 
	  {_mu_Refz.push_back(track[hole5Ini][2]);}
	else{_mu_Refz.push_back(track[hole5Fin][2]);}
      }else
	{
	  _mu_Refx.push_back((track.front())[0]);
	  _mu_Refy.push_back((track.front())[1]);
	  _mu_Refz.push_back((track.front())[2]);      
	}
 
      /*     
      if(hole5Fin) {
	_mu_RefxEnd.push_back(track[hole5Fin][0]);
	_mu_RefyEnd.push_back(track[hole5Fin][1]);
	_mu_RefzEnd.push_back(track[hole5Fin][2]);      
      }else
	{
	  _mu_RefxEnd.push_back((track.back())[0]);
	  _mu_RefyEnd.push_back((track.back())[1]);
	  _mu_RefzEnd.push_back((track.back())[2]);      
	}
      //      _mu_leng.push_back( yetAnOtherLeng);
*/
      double den1 = len1/chunck;	
      double den2 = len2/chunck;	
      double den3 = len3/chunck;	
      _tracks_den1.push_back(den1);
      _tracks_den2.push_back(den2);
      _tracks_den3.push_back(den3);
      
      /*
      for (int i = 0; i < track.size(); i++){
	if (track[i] == track.back()) continue;
	if (track.size() < 2) continue;
	auto trajBit = track[i] - track[i+1];
	if(_verbose)std::cout<<"trajBit.Length() "<<trajBit.Length()<<"\n";

	_tracks_steps.push_back(trajBit.Length());

	if (trajBit.Length()>5) fakeLength += trajBit.Length();
	}
      */
      if(_verbose)std::cout<<"fakeLength() "<<fakeLength2<<"\n";      

      double StepDensity = track.size()/length;

      _tracks_size.push_back(track.size());
      _tracks_stepsDensity.push_back(StepDensity);
      _mu_FakeLength20.push_back(fakeLength20);
      _mu_FakeLength10.push_back(fakeLength10);
      _mu_FakeLength5 .push_back(fakeLength5 );    
      _mu_FakeLength2 .push_back(fakeLength2 );
      _mu_FakeLength1 .push_back(fakeLength1 );

      _mu_ReducedLength20.push_back(length-fakeLength20);
      _mu_ReducedLength10.push_back(length-fakeLength10);
      _mu_ReducedLength5 .push_back(length-fakeLength5 );
      _mu_ReducedLength2 .push_back(length-fakeLength2 );
      _mu_ReducedLength1 .push_back(length-fakeLength1 );


      _mu_FakeLengthThirds20.push_back(fakeLengthThirds20);
      _mu_FakeLengthThirds10.push_back(fakeLengthThirds10);
      _mu_FakeLengthThirds5 .push_back(fakeLengthThirds5 );    
      _mu_FakeLengthThirds2 .push_back(fakeLengthThirds2 );
      _mu_FakeLengthThirds1 .push_back(fakeLengthThirds1 );

      _mu_ReducedLengthThirds20.push_back(length-fakeLengthThirds20);
      _mu_ReducedLengthThirds10.push_back(length-fakeLengthThirds10);
      _mu_ReducedLengthThirds5 .push_back(length-fakeLengthThirds5 );
      _mu_ReducedLengthThirds2 .push_back(length-fakeLengthThirds2 );
      _mu_ReducedLengthThirds1 .push_back(length-fakeLengthThirds1 );



      _mu_FakeLengthHoles20.push_back(fakeLengthHoles20);
      _mu_FakeLengthHoles10.push_back(fakeLengthHoles10);
      _mu_FakeLengthHoles5 .push_back(fakeLengthHoles5 );    
      _mu_FakeLengthHoles2 .push_back(fakeLengthHoles2 );
      _mu_FakeLengthHoles1 .push_back(fakeLengthHoles1 );

      _mu_ReducedLengthHoles20.push_back(length-fakeLengthHoles20);
      _mu_ReducedLengthHoles10.push_back(length-fakeLengthHoles10);
      _mu_ReducedLengthHoles5 .push_back(length-fakeLengthHoles5 );
      _mu_ReducedLengthHoles2 .push_back(length-fakeLengthHoles2 );
      _mu_ReducedLengthHoles1 .push_back(length-fakeLengthHoles1 );

      _mu_Pdg.push_back(Pdg);
      _mu_En.push_back( Energy  );
      _mu_DepEn.push_back( Edep    );
      _mu_Mom.push_back( Mom_Mag );
      _mu_px.push_back( Mom[0]  );
      _mu_py.push_back( Mom[1]  );
      _mu_pz.push_back( Mom[2]  );

	
      _mu_x.push_back( (track.front())[0]   );
      _mu_y.push_back( (track.front())[1]   );
      _mu_z.push_back( (track.front())[2]   );

      _mu_xEnd.push_back( (track.back())[0]   );
      _mu_yEnd.push_back( (track.back())[1]   );
      _mu_zEnd.push_back( (track.back())[2]   );
      _mu_leng.push_back( length);

      _mu_begEndLength.push_back(begEndLength);
      _mu_lengthRatio.push_back(lengthRatio);

    

    //std::cout<<"Number of crazy found is "<<crazy<<std::endl;
    //std::cout<<"Number of < 0   found is "<<zero<<std::endl;
      //      std::cout<<"\n";

    }
    
    _algoMu_tree->Fill();         
    return true;
  }

  void ERAlgoMu::ProcessEnd(TFile* fout)
  {
    if(fout){
      fout->cd();
      if (_algoMu_tree)
	_algoMu_tree->Write();
    }
    
  }

  void ERAlgoMu::InitializeHistos(){

  }


  void ERAlgoMu::Finalize()
  {
    std::cout<<"Number of crazy found is "<<crazy<<std::endl;
    std::cout<<"Number of < 0   found is "<<zero<<std::endl;
   
  }

  void ERAlgoMu::ClearTree(){

    n_tracks = 0;
    _tracks_size.clear();
    _mu_begEndLength.clear();
    _mu_lengthRatio.clear();
    _tracks_steps.clear()   ;
    _tracks_stepsDensity.clear()   ;

    _mu_ReducedLength20.clear(); _mu_FakeLength20.clear();
    _mu_ReducedLength10.clear(); _mu_FakeLength10.clear();
    _mu_ReducedLength5.clear() ; _mu_FakeLength5.clear() ;
    _mu_ReducedLength2.clear() ; _mu_FakeLength2.clear() ;
    _mu_ReducedLength1.clear() ; _mu_FakeLength1.clear() ;

    _mu_ReducedLengthThirds20.clear(); _mu_FakeLengthThirds20.clear();
    _mu_ReducedLengthThirds10.clear(); _mu_FakeLengthThirds10.clear();
    _mu_ReducedLengthThirds5.clear() ; _mu_FakeLengthThirds5.clear() ;
    _mu_ReducedLengthThirds2.clear() ; _mu_FakeLengthThirds2.clear() ;
    _mu_ReducedLengthThirds1.clear() ; _mu_FakeLengthThirds1.clear() ;

    _mu_ReducedLengthHoles20.clear(); _mu_FakeLengthHoles20.clear();
    _mu_ReducedLengthHoles10.clear(); _mu_FakeLengthHoles10.clear();
    _mu_ReducedLengthHoles5.clear() ; _mu_FakeLengthHoles5.clear() ;
    _mu_ReducedLengthHoles2.clear() ; _mu_FakeLengthHoles2.clear() ;
    _mu_ReducedLengthHoles1.clear() ; _mu_FakeLengthHoles1.clear() ;

    _tracks_den1.clear();
    _tracks_den2.clear();
    _tracks_den3.clear();
    _mu_En.clear()   ;  _mu_EnMPS .clear();
    _mu_DepEn.clear();	_mu_MomMPS.clear();
    _mu_Mom.clear()  ;	_mu_leng  .clear();

    _mu_px.clear()   ;  _mu_pxMPS.clear();
    _mu_py.clear()   ;  _mu_pyMPS.clear();
    _mu_pz.clear()   ;  _mu_pzMPS.clear();
    
    _mu_Pdg.clear()  ;
    
    _mu_x.clear()    ;  _mu_xEnd.clear();       
    _mu_y.clear()    ;  _mu_yEnd.clear();	    
    _mu_z.clear()    ;  _mu_zEnd.clear();

    _mu_Refx.clear() ;  _mu_RefxEnd.clear();       
    _mu_Refy.clear() ;  _mu_RefyEnd.clear();	    
    _mu_Refz.clear() ;  _mu_RefzEnd.clear();

    return;
  }

}

#endif

      /*
      //Momentum and Energy Calculation with Multiple Coulomb Scattering
      _mu_MomMPS = 1000*momCalc.GetMomentumMultiScatterLLHD(track);
      _mu_EnMPS  = sqrt(_mu_MomMPS*_mu_MomMPS + mass*mass); 
      
      geoalgo::Vector_t MomMPS = Dir*_mu_MomMPS;
      _mu_pxMPS = MomMPS[0];
      _mu_pyMPS = MomMPS[1];
      _mu_pzMPS = MomMPS[2];
      */
      
      /*
      graph.GetParticle(t).SetParticleInfo(Pdg,
					   mass,
					   track.front(),
					   Mom);
      
      auto muon =graph.GetParticle(t);
      */
      
