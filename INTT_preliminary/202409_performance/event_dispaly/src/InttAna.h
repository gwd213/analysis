#ifndef INTTANA_H__
#define INTTANA_H__

#include <fun4all/SubsysReco.h>
#include <string>

#include <cmath>
/// Cluster/Calorimeter includes

/// Fun4All includes
#include <fun4all/Fun4AllHistoManager.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>

// #include <trackbase/TrkrClusterv4.h>
// #include <trackbase/TrkrClusterContainerv3.h>
#include <trackbase/TrkrCluster.h>
#include <trackbase/TrkrClusterContainer.h>
#include <trackbase/TrkrHit.h>
#include <trackbase/TrkrHitSet.h>
#include <trackbase/TrkrHitSetContainer.h>
#include <trackbase/ActsGeometry.h>
#include <trackbase/InttDefs.h>

#include <trackbase/InttEventInfo.h>

#include <globalvertex/GlobalVertex.h>
#include <globalvertex/GlobalVertexMap.h>

// #include <globalvertex/MbdVertex.h>
// #include <globalvertex/MbdVertexMap.h>
#include <mbd/MbdOut.h>
#include <ffarawobjects/Gl1RawHit.h>

#include <ffarawobjects/InttRawHit.h>
#include <ffarawobjects/InttRawHitContainer.h>

#include "inttxyvertexfinder/InttVertexMapv1.h"
#include "inttxyvertexfinder/InttVertex3D.h"
#include "inttxyvertexfinder/InttVertex3DMap.h"

#include <globalvertex/SvtxVertex.h>
#include <globalvertex/SvtxVertexMap.h>
#include <trackbase_historic/SvtxTrack.h>
#include <trackbase_historic/SvtxTrackMap.h>

#include <calobase/RawCluster.h>
#include <calobase/RawClusterContainer.h>

/// ROOT includes
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TVector3.h>

/// C++ includes
#include <cassert>
#include <cmath>
#include <sstream>
#include <string>

#include "InttEvent.h"
#include "InttRawData.h"
// truth
//  To get vertex
#include <globalvertex/GlobalVertexMap.h>
#include <globalvertex/GlobalVertex.h>

// TODO:
/// HEPMC truth includes
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <HepMC/GenEvent.h>
#include <HepMC/GenVertex.h>
#pragma GCC diagnostic pop

#include <phhepmc/PHHepMCGenEvent.h>
#include <phhepmc/PHHepMCGenEventMap.h>

#include <g4main/PHG4InEvent.h>
#include <g4main/PHG4VtxPoint.h> // for PHG4Particle
#include <g4main/PHG4Particle.h> // for PHG4Particle
// truth end

/// Class declarations for use in the analysis module
class PHCompositeNode;
class TFile;
class TH1;
class TH2;
class TNtuple;
class TTree;
class InttRawData;

/// Definition of this analysis module class
class InttAna : public SubsysReco
{
 public:
  struct evtbco_info{
    int      evt_gl1 {-1};
    uint     evt_intt{ 0};
    int      evt_mbd {-1};
    uint64_t bco_gl1 {0};
    uint64_t bco_intt{0};
    uint64_t bco_mbd {0};

    void clear() {
      evt_gl1  = -1;
      evt_intt =  0;
      evt_mbd  = -1;
      bco_gl1  =  0;
      bco_intt =  0;
      bco_mbd  =  0;
    };

    void copy(const evtbco_info& info) {
      evt_gl1  = info.evt_gl1 ;
      evt_intt = info.evt_intt;
      evt_mbd  = info.evt_mbd ;
      bco_gl1  = info.bco_gl1 ;
      bco_intt = info.bco_intt;
      bco_mbd  = info.bco_mbd ;
    };
  };
  
 public:
  /// Constructor
  InttAna(const std::string &name = "InttAna",
          const std::string &fname = "AnaTutorial.root");

  // Destructor
  virtual ~InttAna();

  /// SubsysReco initialize processing method
  int Init(PHCompositeNode *);
 
  /// SubsysReco initialize processing method
  int InitRun(PHCompositeNode *);

  /// SubsysReco event processing method
  int process_event(PHCompositeNode *);

  /// SubsysReco end processing method
  int End(PHCompositeNode *);

  //truth information
  // void getHEPMCTruth(PHCompositeNode *);
  // void getPHG4Particle(PHCompositeNode *);
  void getHEPMCTruth(PHCompositeNode *topNode);
  void getPHG4Particle(PHCompositeNode *topNode);


  void setInttRawData(InttRawData *rawModule) { _rawModule = rawModule; }
  void readRawHit(PHCompositeNode *);

  void setBeamCenter(float x=0, float y=0) {xbeam_=x; ybeam_=y;}

 private:
  InttRawData* _rawModule;

  std::string fname_;
  TFile* anafile_;
  TH1*   h_dca2d_zero; 
  TH2*   h2_dca2d_zero; 
  TH2*   h2_dca2d_len; 
  TH1*   h_zvtx;
  TH1*   h_eta;
  TH1*   h_phi;
  TH1*   h_theta;
  TNtuple*  h_ntp_clus; 
  TNtuple*  h_ntp_emcclus; 
  //TTree*  h_t_clus; 
  TNtuple*  h_ntp_cluspair; 
  TNtuple*  h_ntp_emccluspair{nullptr}; 
  TNtuple*  h_ntp_evt; 
  TTree*  h_t_evt_bco; 

  float xbeam_;
  float ybeam_;
  double xvtx_sim = -9999;
  double yvtx_sim = -9999;
  double zvtx_sim = -9999;

  evtbco_info m_evtbcoinfo;
  evtbco_info m_evtbcoinfo_prev;

  TH1* h_zvtxseed_;
  TTree *m_hepmctree;

    /// HEPMC Tree variables
  int m_evt=0;
  double m_xvtx;
  double m_yvtx;
  double m_zvtx;
  int m_partid1;
  int m_partid2;
  double m_x1;
  double m_x2;
  int m_mpi;
  int m_process_id;
  double m_truthenergy;
  double m_trutheta;
  double m_truththeta;
  double m_truthphi;
  int m_status;
  double m_truthpx;
  double m_truthpy;
  double m_truthpz;
  double m_truthpt;
  double m_truthp;
  int m_numparticlesinevent;
  int m_truthpid;
  double m_vertex;


  void InitHists();
  void InitTrees();
  void InitTuples();

  int GetNodes( PHCompositeNode *topNode );
  ActsGeometry *m_tGeometry;
  PHHepMCGenEventMap *hepmceventmap;
  PHG4InEvent *phg4inevent;
  TrkrClusterContainer *m_clusterMap;
  GlobalVertexMap *vertexs;
  InttEventInfo *inttevthead;
  SvtxVertexMap *svtxvertexmap;
  MbdOut *mbdout;
  SvtxVertex *svtxvertex;

  
};

#endif
