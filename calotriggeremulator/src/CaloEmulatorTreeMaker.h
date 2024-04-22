#ifndef CALOEMULATORTREEMAKER_H
#define CALOEMULATORTREEMAKER_H

#include <fun4all/SubsysReco.h>

#include <string>
#include <vector>

#include <calotrigger/LL1Out.h>
#include <calotrigger/LL1Outv1.h>
#include <calotrigger/TriggerPrimitive.h>
#include <calotrigger/TriggerPrimitivev1.h>
#include <calotrigger/TriggerPrimitiveContainer.h>
#include <calotrigger/TriggerPrimitiveContainerv1.h>
#include <calotrigger/TriggerDefs.h>
#include <calobase/TowerInfoContainer.h>
#include <calobase/TowerInfoContainerv1.h>
#include <calobase/TowerInfoContainerv2.h>
#include <calobase/TowerInfoContainerv3.h>
#include <calobase/TowerInfo.h>
#include <calobase/TowerInfov1.h>
#include <calobase/TowerInfov2.h>
#include <calobase/TowerInfov3.h>

#include "TTree.h"
#include "TFile.h"


class PHCompositeNode;
class TriggerPrimitive;
class TriggerPrimitiveContainer;
class LL1Out;
class TowerInfoContainer;
class CaloEmulatorTreeMaker : public SubsysReco
{
 public:

  CaloEmulatorTreeMaker(const std::string &name = "CaloEmulatorTreeMaker", const std::string &outfilename = "trees_caloemulator.root");

  virtual ~CaloEmulatorTreeMaker();

  int Init(PHCompositeNode *topNode) override;

  int InitRun(PHCompositeNode *topNode) override;

  int process_event(PHCompositeNode *topNode) override;
  
  void GetNodes (PHCompositeNode *topNode);

  int ResetEvent(PHCompositeNode *topNode) override;

  int EndRun(const int runnumber) override;

  int End(PHCompositeNode *topNode) override;

  int Reset(PHCompositeNode * /*topNode*/) override;

  void SetVerbosity(int verbo) ;
  
  void UseCaloTowerBuilder(bool use) {useCaloTowerBuilder = use;}
 private:
  void reset_tree_vars();

  int _verbosity;

  TFile *_f;
  TTree *_tree;
  std::string _foutname;
  std::string _nodename;
  int _i_event;
  bool useCaloTowerBuilder;

  LL1Out *_ll1out_trigger;
  LL1Out *_ll1out_raw_trigger;

  TriggerPrimitive *_trigger_primitive;
  TriggerPrimitiveContainer *_trigger_primitives_raw_emcal;
  TriggerPrimitiveContainer *_trigger_primitives_raw_emcal_ll1;
  TriggerPrimitiveContainer *_trigger_primitives_raw_trigger;
  TriggerPrimitiveContainer *_trigger_primitives_emcal;
  TriggerPrimitiveContainer *_trigger_primitives_emcal_ll1;
  TriggerPrimitiveContainer *_trigger_primitives_hcalin;
  TriggerPrimitiveContainer *_trigger_primitives_hcalout;
  TriggerPrimitiveContainer *_trigger_primitives_hcal_ll1;
  TriggerPrimitiveContainer *_trigger_primitives_trigger;

  TowerInfo *_tower;
  TowerInfoContainer* _towers;

  /* const int emcal_nsums = 6144; // number of sums in all of emcal 4*12*4*32; */
  /* const int hcal_nsums = 384; // number of sums in all of emcal 12*32; */
  /* const int emcal_ll1_nsums = 384; // number of 8x8 sums in all of emcal 4*12*4*32 /16; */
  /* const int jet_nsums = 288; //32*9; */

  unsigned int b_trigger_sum_smpl_emcal[6144];
  unsigned int b_trigger_sumkey_emcal[6144];
  unsigned int b_trigger_sum_emcal[6144];  
  unsigned int b_trigger_sum_smpl_hcalin[384];
  unsigned int b_trigger_sumkey_hcalin[384];
  unsigned int b_trigger_sum_hcalin[384];
  unsigned int b_trigger_sum_smpl_hcalout[384];
  unsigned int b_trigger_sumkey_hcalout[384];
  unsigned int b_trigger_sum_hcalout[384];
  
  unsigned int b_trigger_sum_smpl_emcal_ll1[384];
  unsigned int b_trigger_sumkey_emcal_ll1[384];
  unsigned int b_trigger_sum_emcal_ll1[384];
  unsigned int b_trigger_sum_smpl_hcal_ll1[384];
  unsigned int b_trigger_sumkey_hcal_ll1[384];
  unsigned int b_trigger_sum_hcal_ll1[384];

  unsigned int b_trigger_sum_smpl_jet[288];
  unsigned int b_trigger_sumkey_jet[288];
  unsigned int b_trigger_sum_jet[288];

  unsigned int b_trigger_sum_smpl_jet_input[384];
  unsigned int b_trigger_sumkey_jet_input[384];
  unsigned int b_trigger_sum_jet_input[384];

  unsigned int b_trigger_raw_sum_smpl_emcal[6144];
  unsigned int b_trigger_raw_sumkey_emcal[6144];
  unsigned int b_trigger_raw_sum_emcal[6144];

  unsigned int b_trigger_raw_sum_smpl_emcal_ll1[384];
  unsigned int b_trigger_raw_sumkey_emcal_ll1[384];
  unsigned int b_trigger_raw_sum_emcal_ll1[384];

  unsigned int b_trigger_raw_sum_smpl_jet[288];
  unsigned int b_trigger_raw_sumkey_jet[288];
  unsigned int b_trigger_raw_sum_jet[288];

  unsigned int b_trigger_raw_sum_smpl_jet_input[384];
  unsigned int b_trigger_raw_sumkey_jet_input[384];
  unsigned int b_trigger_raw_sum_jet_input[384];

  std::vector<unsigned int> b_trigger_bits;
  std::vector<unsigned int> b_trigger_raw_bits;

  std::vector<float> b_emcal_energy;
  std::vector<float> b_emcal_time;
  std::vector<float> b_emcal_etabin;
  std::vector<float> b_emcal_phibin;

  std::vector<float> b_hcalin_energy;
  std::vector<float> b_hcalin_time;
  std::vector<float> b_hcalin_etabin;
  std::vector<float> b_hcalin_phibin;

  std::vector<float> b_hcalout_energy;
  std::vector<float> b_hcalout_time;
  std::vector<float> b_hcalout_etabin;
  std::vector<float> b_hcalout_phibin;

};

#endif 
