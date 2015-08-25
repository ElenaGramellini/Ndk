#ifndef ERTOOL_ERANAONETOONEMU_CXX
#define ERTOOL_ERANAONETOONEMU_CXX

#include "ERAnaOneToOneMu.h"

namespace ertool {

  ERAnaOneToOneMu::ERAnaOneToOneMu(const std::string& name) : AnaBase(name)  , _ana1to1_tree(nullptr)
  {
    _verbose = false;
  }

  void ERAnaOneToOneMu::Reset()
  {}

  void ERAnaOneToOneMu::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaOneToOneMu::ProcessBegin()
  {
    if (_ana1to1_tree) { delete _ana1to1_tree; }
    _ana1to1_tree = new TTree("_ana1to1_tree","Ana 1 to 1 Comparison Tree");

    _ana1to1_tree->Branch("n_tracks"  , &n_tracks, "n_tracks/I");
    _ana1to1_tree->Branch("_tracks_size"           , "vector<int>"    , &_tracks_size);
    _ana1to1_tree->Branch("_tracks_Pdg"            , "vector<int>"    , &_tracks_Pdg);                                
    _ana1to1_tree->Branch("_tracks_begEndLength"   , "vector<double>"    , &_tracks_begEndLength);          
    _ana1to1_tree->Branch("_tracks_leng"           , "vector<double>"    , &_tracks_leng);                             		  	  	  
    _ana1to1_tree->Branch("_tracks_ReducedLength"  , "vector<double>"    , &_tracks_ReducedLength);         
    _ana1to1_tree->Branch("_tracks_DepEnFromLength", "vector<double>"    , &_tracks_DepEnFromLength);       
    _ana1to1_tree->Branch("_tracks_DepEn"          , "vector<double>"    , &_tracks_DepEn);                 
    _ana1to1_tree->Branch("_tracks_x"              , "vector<double>"    , &_tracks_x);     		   	
    _ana1to1_tree->Branch("_tracks_y"              , "vector<double>"    , &_tracks_y);     		   	
    _ana1to1_tree->Branch("_tracks_z"              , "vector<double>"    , &_tracks_z);     		   	
    _ana1to1_tree->Branch("_tracks_xEnd"           , "vector<double>"    , &_tracks_xEnd);  		   	
    _ana1to1_tree->Branch("_tracks_yEnd"           , "vector<double>"    , &_tracks_yEnd);  		   	
    _ana1to1_tree->Branch("_tracks_zEnd"           , "vector<double>"    , &_tracks_zEnd);    

    _ana1to1_tree->Branch("_tracksTru_size"           , "vector<int>"    , &_tracksTru_size);
    _ana1to1_tree->Branch("_tracksTru_Pdg"            , "vector<int>"    , &_tracksTru_Pdg);                                
    _ana1to1_tree->Branch("_tracksTru_begEndLength"   , "vector<double>"    , &_tracksTru_begEndLength);          
    _ana1to1_tree->Branch("_tracksTru_leng"           , "vector<double>"    , &_tracksTru_leng);                             		  	  	  
    _ana1to1_tree->Branch("_tracksTru_ReducedLength"  , "vector<double>"    , &_tracksTru_ReducedLength);         
    _ana1to1_tree->Branch("_tracksTru_DepEnFromLength", "vector<double>"    , &_tracksTru_DepEnFromLength);       
    _ana1to1_tree->Branch("_tracksTru_DepEn"          , "vector<double>"    , &_tracksTru_DepEn);                 
    _ana1to1_tree->Branch("_tracksTru_x"              , "vector<double>"    , &_tracksTru_x);     		   	
    _ana1to1_tree->Branch("_tracksTru_y"              , "vector<double>"    , &_tracksTru_y);     		   	
    _ana1to1_tree->Branch("_tracksTru_z"              , "vector<double>"    , &_tracksTru_z);     		   	
    _ana1to1_tree->Branch("_tracksTru_xEnd"           , "vector<double>"    , &_tracksTru_xEnd);  		   	
    _ana1to1_tree->Branch("_tracksTru_yEnd"           , "vector<double>"    , &_tracksTru_yEnd);  		   	
    _ana1to1_tree->Branch("_tracksTru_zEnd"           , "vector<double>"    , &_tracksTru_zEnd);               

    _ana1to1_tree->Branch("_tracks_DeltaReducedLength"  , "vector<double>"    , &_tracks_DeltaReducedLength);  
    _ana1to1_tree->Branch("_tracks_DeltaDepEnFromLength", "vector<double>"    , &_tracks_DeltaDepEnFromLength);
    _ana1to1_tree->Branch("_tracks_DeltaDepEn"          , "vector<double>"    , &_tracks_DeltaDepEn);          
    _ana1to1_tree->Branch("_tracks_Deltax"              , "vector<double>"    , &_tracks_Deltax);     		
    _ana1to1_tree->Branch("_tracks_Deltay"              , "vector<double>"    , &_tracks_Deltay);     		
    _ana1to1_tree->Branch("_tracks_Deltaz"              , "vector<double>"    , &_tracks_Deltaz);     		
    _ana1to1_tree->Branch("_tracks_DeltaxEnd"           , "vector<double>"    , &_tracks_DeltaxEnd);  		
    _ana1to1_tree->Branch("_tracks_DeltayEnd"           , "vector<double>"    , &_tracks_DeltayEnd);  		
    _ana1to1_tree->Branch("_tracks_DeltazEnd"           , "vector<double>"    , &_tracks_DeltazEnd);           
    _ana1to1_tree->Branch("_tracks_DeltaLength"         , "vector<double>"    , &_tracks_DeltaLength);           

  }

  bool ERAnaOneToOneMu::Analyze(const EventData &data, const ParticleGraph &graph)
  { 
    ClearTree();
    Particle trackMCParticle;            Particle trackParticle;
    Track    MCTrack;        	         Track    Track;       
 
    int    MCPdg            = 0;         int    Pdg            = 0;	 
    int    MCsize           = 0;         int    size           = 0;	 
    double MCbegEndLength   = 0;         double begEndLength   = 0;	 
    double MCleng           = 0;         double leng           = 0;	 
    double MCReducedLength   = 0;        double ReducedLength   = 0;	 
    double MCDepEnFromLength = 0;        double DepEnFromLength = 0;	 
    double MCDepEnMCTrack    = 0;        double DepEnTrack    = 0;	 
    double MCxTrack= 0;	      	         double xTrack= 0;	      	 
    double MCyTrack= 0;	      	         double yTrack= 0;	      	 
    double MCzTrack= 0;	      	         double zTrack= 0;	      	 
    double MCxEndTrack  = 0;	         double xEndTrack  = 0;	  
    double MCyEndTrack  = 0;	         double yEndTrack  = 0;	  
    double MCzEndTrack  = 0;             double zEndTrack  = 0;            
	
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MCParticleGraph checks
    auto const& mc_graph = MCParticleGraph();
    auto const& mc_graph_arr = mc_graph.GetParticleArray();
    auto const& mc_data = MCEventData();
    if (!mc_data.Shower().size()) std::cout<<"No MCShowers"<<"\n";
    if (!mc_data.Track().size()) std::cout<<"No MCTracks"<<"\n";

    if (_verbose){
      std::cout<<"########## In MCParticleGraph ########## \n";
      std::cout << "MC Particle Diagram: " << std::endl
		<< mc_graph.Diagram() << std::endl;
      std::cout<<" Graph "<<"\n";
      std::cout<<" Size In ERAnaMCTruth "<<mc_graph_arr.size()<<"\n";
      std::cout<<"Number of Tracks: "<<mc_data.Track().size()<<"\n";
    }
    
    for (auto const& t : mc_graph.GetParticleNodes(RecoType_t::kTrack)){
      if (mc_graph.GetParticle(t).PdgCode() == -13){
	MCTrack = mc_data.Track(mc_graph.GetParticle(t).RecoID());
	trackMCParticle = mc_graph.GetParticle(t);

	MCPdg                = trackMCParticle.PdgCode();	  
	MCDepEnMCTrack       = trackMCParticle.Energy() - 105;	  
	MCxTrack     = trackMCParticle.Vertex()[0];	      		  
	MCyTrack     = trackMCParticle.Vertex()[1];	      		  
	MCzTrack     = trackMCParticle.Vertex()[2];	      		  

	MCsize               = MCTrack.size();	  
	MCbegEndLength       = (MCTrack.front() - MCTrack.back()).Length();
	MCleng               = MCTrack.Length();	  
	MCReducedLength      = MCTrack.Length();	  
	MCDepEnFromLength    = 2.3*MCTrack.Length();	  
	MCxEndTrack  = MCTrack.back()[0] ;	      	  
	MCyEndTrack  = MCTrack.back()[1] ;	      	  
	MCzEndTrack  = MCTrack.back()[2] ;    
          
      }
      if (_verbose) { 
	std::cout << "Track:              (" << t << ")" << "\tE: " << MCTrack._energy << std::endl; 
	std::cout << "Track particle:     (" << t << ")" << "\tE: " << trackMCParticle.Energy() << std::endl; }      
    }
    
    if (mc_data.Track().size())
      {
	_tracksTru_Pdg.push_back(MCPdg)                         ;
	_tracksTru_size.push_back(MCsize)                       ;
	_tracksTru_begEndLength.push_back(MCbegEndLength)       ;
	_tracksTru_leng.push_back(MCleng)                       ;
	_tracksTru_ReducedLength.push_back(MCReducedLength)     ;
	_tracksTru_DepEnFromLength.push_back(MCDepEnFromLength) ;
	_tracksTru_DepEn.push_back(MCDepEnMCTrack)              ;

	_tracksTru_x.push_back   (MCxTrack     )  ;	      
	_tracksTru_y.push_back   (MCyTrack     )  ;	      
	_tracksTru_z.push_back   (MCzTrack     )  ;	      
	_tracksTru_xEnd.push_back(MCxEndTrack  )  ;	      
	_tracksTru_yEnd.push_back(MCyEndTrack  )  ;	      
	_tracksTru_zEnd.push_back(MCzEndTrack  )  ;         
      }

    // End of checks on MCParticleGraph
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Loop through Particles associated with a track
    //LOOP ON TRACKS
    auto const track_nodes = graph.GetParticleNodes(RecoType_t::kTrack);
    for (auto const& t : track_nodes){
      auto const& trackParticle = graph.GetParticle(t);
      auto const& Track = data.Track(graph.GetParticle(t).RecoID());
      //Assign particle identification
      if ((Track._pid_score[Track::kProton]<Track._pid_score[Track::kPion])&&
	  (Track._pid_score[Track::kProton]<Track._pid_score[Track::kKaon])&&
	  (Track._pid_score[Track::kProton]<Track._pid_score[Track::kMuon]))      Pdg = 2212;
      
      if ((Track._pid_score[Track::kPion]<Track._pid_score[Track::kProton])&&
	  (Track._pid_score[Track::kPion]<Track._pid_score[Track::kKaon])&&
	  (Track._pid_score[Track::kPion]<Track._pid_score[Track::kMuon]))        Pdg = 211; 
      
      if ((Track._pid_score[Track::kKaon]<Track._pid_score[Track::kProton])&&
	  (Track._pid_score[Track::kKaon]<Track._pid_score[Track::kPion])&&
	  (Track._pid_score[Track::kKaon]<Track._pid_score[Track::kMuon]))        Pdg = 321;
      
      if ((Track._pid_score[Track::kMuon]<Track._pid_score[Track::kProton])&&
	  (Track._pid_score[Track::kMuon]<Track._pid_score[Track::kPion])&&
	  (Track._pid_score[Track::kMuon]<Track._pid_score[Track::kKaon]))        Pdg = 13;      
      
      if (_verbose) std::cout<<"Track Size "<<Track.size()<<"\n";      
      // Before doing anything else, let's fix the lenght of the track

      if (Track.size() < 100) continue;   // I keep only "long" tracks //10 for MC, 100 for Reco
      double fakeLength    = 0; 
      auto edge1  = Track.front();
      auto edge2  = Track.back();
      auto trBeg  = Track.front();
      auto trEnd  = Track.back();
      auto trBeg2 = Track.front();

      ///////////////////////////////////////////////////////////
      ///In this block I refine my track
      double chunck = (double) Track.size()/5.;
      int holeIni  = 0 ;  int holeFin  = 0 ;

      // In this block I fix the real begin and end of the track
      for (int i = 0; i < Track.size()-1; i++){
	auto trajBit = Track[i] - Track[i+1];
	auto stepLength = trajBit.Length();	
	if (i < chunck) {
	  if (stepLength > 5 ) {holeIni = i+1; fakeLength  += stepLength ;} //100 for MC, 5 for Reco
	}else if (i > chunck*4){ 
	  if (stepLength > 5 ) {fakeLength  += stepLength ; if (!holeFin ) holeFin = i; }
	}
      }// end of points loop
      if (!holeFin) holeFin = Track.size()-1;
      ReducedLength = 0;
      for (int i = holeIni; i < Track.size()-1; i++){
	if ((i >= holeFin )&&(holeFin)) break; 
	auto trajBit = Track[i] - Track[i+1];
	auto stepLength = trajBit.Length();	
	ReducedLength+= stepLength ;
      }
      edge1 = Track[holeIni];
      edge2 = Track[holeFin];

      size          = holeFin-holeIni;	 
      leng          = Track.Length();      // track trajectory length from reco obj
      begEndLength  = (edge1-edge2).Length();  // track trajectory length new start to new end
      
      DepEnTrack     = Track._energy;  // track deposited energy from calorimetry obj
      DepEnFromLength   = ReducedLength*2.3;  // track deposited energy from length obj
      
      if (DepEnTrack>2000) continue;
      
     
      if((edge1-trackMCParticle.Vertex()).Length() < (edge2-trackMCParticle.Vertex()).Length()){
	  trBeg  = edge1;
	  trEnd  = edge2;
	  trBeg2 = Track[holeIni+1];
	}
	else  {
	  trBeg = edge2;
	  trEnd = edge1;
	  trBeg2 = Track[holeFin-1];
	}
	
	if ((trBeg[0]<3.   ) && (trEnd[0]> 250.)) continue;
	if ((trBeg[1]<-113.) && (trEnd[1]> 113.)) continue;
	if ((trBeg[2]<3.   ) && (trEnd[2]>1050.)) continue; 
	if ((trEnd[0]<3.   ) && (trBeg[0]> 250.)) continue;
	if ((trEnd[1]<-113.) && (trBeg[1]> 113.)) continue;
	if ((trEnd[2]<3.   ) && (trBeg[2]>1050.)) continue; 

	xTrack     = trBeg[0];	      	 
	yTrack     = trBeg[1];	      	 
	zTrack     = trBeg[2];	      	 
	xEndTrack  = trEnd[0];	  	 
	yEndTrack  = trEnd[1];	  	 
	zEndTrack  = trEnd[2];          

      
      if (data.Track().size())
	{
	  _tracks_Pdg.push_back(Pdg)                         ;
	  _tracks_size.push_back(size)                       ;
	  _tracks_begEndLength.push_back(begEndLength)       ;
	  _tracks_leng.push_back(leng)                       ;
	  _tracks_ReducedLength.push_back(ReducedLength)     ;
	  _tracks_DepEnFromLength.push_back(DepEnFromLength) ;
	  _tracks_DepEn.push_back(DepEnTrack)              ;
	  
	  _tracks_x.push_back   (xTrack     )  ;	      
	  _tracks_y.push_back   (yTrack     )  ;	      
	  _tracks_z.push_back   (zTrack     )  ;	      
	  _tracks_xEnd.push_back(xEndTrack  )  ;	      
	  _tracks_yEnd.push_back(yEndTrack  )  ;	      
	  _tracks_zEnd.push_back(zEndTrack  )  ;                
	  

	  if (mc_data.Track().size())
	    {/*
	      if ((xEndTrack > 10)     && (xEndTrack < 246.35)&&
		  (yEndTrack > -106.5) && (yEndTrack < 106.5 )&&
		  (zEndTrack > 10)     && (zEndTrack < 1026  ) )
		{
	     */
	      _tracks_DeltaReducedLength.push_back(ReducedLength - MCReducedLength)     ;
	      _tracks_DeltaLength.push_back(leng - MCReducedLength)     ;	
	      _tracks_DeltaDepEnFromLength.push_back(DepEnFromLength - MCDepEnMCTrack) ;	
	      _tracks_DeltaDepEn.push_back(DepEnTrack - MCDepEnMCTrack)              ;	
	      
	      _tracks_Deltax.push_back   (xTrack    - MCxTrack    )  ;	      	
	      _tracks_Deltay.push_back   (yTrack    - MCyTrack    )  ;	      	
	      _tracks_Deltaz.push_back   (zTrack    - MCzTrack    )  ;	      	
	      _tracks_DeltaxEnd.push_back(xEndTrack - MCxEndTrack )  ;	      	
	      _tracks_DeltayEnd.push_back(yEndTrack - MCyEndTrack )  ;	      	
	      _tracks_DeltazEnd.push_back(zEndTrack - MCzEndTrack )  ;               
	      
	    }
	}
     }
    


    _ana1to1_tree->Fill();
    return true; 
  }

  void ERAnaOneToOneMu::ProcessEnd(TFile* fout)
  {
    if(fout){
      fout->cd();
      if (_ana1to1_tree)
	_ana1to1_tree->Write();
    }
  }

  void ERAnaOneToOneMu::ClearTree(){

    n_tracks = 0;
    _tracks_Pdg.clear()            ;       _tracksTru_Pdg.clear()            ;
    _tracks_size.clear()           ;       _tracksTru_size.clear()           ;
           		  	  			
    _tracks_begEndLength.clear()   ;       _tracksTru_begEndLength.clear()   ;
    _tracks_leng.clear()           ;       _tracksTru_leng.clear()           ;
           		  	  			
    _tracks_ReducedLength.clear()  ;       _tracksTru_ReducedLength.clear()  ;
    _tracks_DepEnFromLength.clear();       _tracksTru_DepEnFromLength.clear();
    _tracks_DepEn.clear()          ;       _tracksTru_DepEn.clear()          ;
    _tracks_x.clear()     ;		   _tracksTru_x.clear()     ;		
    _tracks_y.clear()     ;		   _tracksTru_y.clear()     ;		
    _tracks_z.clear()     ;		   _tracksTru_z.clear()     ;		
    _tracks_xEnd.clear()  ;		   _tracksTru_xEnd.clear()  ;		
    _tracks_yEnd.clear()  ;		   _tracksTru_yEnd.clear()  ;		
    _tracks_zEnd.clear()  ;                _tracksTru_zEnd.clear()  ;         

    _tracks_DeltaReducedLength.clear()  ;
    _tracks_DeltaDepEnFromLength.clear();
    _tracks_DeltaDepEn.clear()          ;
    _tracks_Deltax.clear()     ;		
    _tracks_Deltay.clear()     ;		
    _tracks_Deltaz.clear()     ;		
    _tracks_DeltaxEnd.clear()  ;		
    _tracks_DeltayEnd.clear()  ;		
    _tracks_DeltazEnd.clear()  ;     
    _tracks_DeltaLength.clear()  ;         
  }

}

#endif
