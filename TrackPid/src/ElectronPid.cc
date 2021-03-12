#include "ElectronPid.h"

#include <cstdlib>
#include <cstdio>
#include <iomanip>
#include <fstream>

#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>                         // for PHIODataNode
#include <phool/PHNode.h>                               // for PHNode
#include <phool/PHNodeIterator.h>
#include <phool/PHObject.h>                             // for PHObject
#include <phool/getClass.h>
#include <phool/phool.h>  // for PHWHERE
#include <phool/PHRandomSeed.h>
#include <phool/getClass.h>

#include <fun4all/Fun4AllReturnCodes.h>

#include <trackbase_historic/SvtxTrackMap.h>
#include <trackbase_historic/SvtxTrack.h>

// gsl
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

#include <TVector3.h>
#include <TMatrixFfwd.h>    // for TMatrixF

#include <iostream>                            // for operator<<, basic_ostream
#include <set>                                 // for _Rb_tree_iterator, set
#include <utility>                             // for pair

class PHCompositeNode;

using namespace std;

ElectronPid::ElectronPid(const std::string& name, const std::string &filename) : SubsysReco(name)
{
  OutputNtupleFile=nullptr;
  OutputFileName=filename;
  EventNumber=0;
  output_ntuple = true;

  /// default limits 
  EMOP_lowerlimit = 0.0;
  EMOP_higherlimit = 100.0;
  HOP_lowerlimit = 0.0;
  HinOEM_higherlimit = 100.0;
  Pt_lowerlimit = 0.0;
  Pt_higherlimit = 100.0;
}

ElectronPid::~ElectronPid() 
{
}

int ElectronPid::Init(PHCompositeNode *topNode)
{

  if(output_ntuple) {

	OutputNtupleFile = new TFile(OutputFileName.c_str(),"RECREATE");
  	std::cout << "PairMaker::Init: output file " << OutputFileName.c_str() << " opened." << endl;

	ntpbeforecut = new TNtuple("ntpbeforecut","","p:pt:cemce3x3:hcaline3x3:hcaloute3x3:cemce3x3overp:hcaline3x3overcemce3x3:hcale3x3overp:charge:pid:quality:e_cluster:EventNumber:z:vtxid");
        ntpcutEMOP = new TNtuple("ntpcutEMOP","","p:pt:cemce3x3:hcaline3x3:hcaloute3x3:charge:pid:quality:e_cluster:EventNumber:z:vtxid");
	ntpcutHOP = new TNtuple("ntpcutHOP","","p:pt:cemce3x3:hcaline3x3:hcaloute3x3:charge:pid:quality:e_cluster:EventNumber:z:vtxid");
	ntpcutEMOP_HinOEM = new TNtuple("ntpcutEMOP_HinOEM","","p:pt:cemce3x3:hcaline3x3:hcaloute3x3:charge:pid:quality:e_cluster:EventNumber:z:vtxid");
	ntpcutEMOP_HinOEM_Pt = new TNtuple("ntpcutEMOP_HinOEM_Pt","","p:pt:cemce3x3:hcaline3x3:hcaloute3x3:charge:pid:quality:e_cluster:EventNumber:z:vtxid");
  }
  else {
	PHNodeIterator iter(topNode);
	PHCompositeNode *dstNode = dynamic_cast<PHCompositeNode *>(iter.findFirst("PHCompositeNode", "DST"));
	if (!dstNode)
	{
  	  cerr << PHWHERE << " ERROR: Can not find DST node." << endl;
    	  return Fun4AllReturnCodes::ABORTEVENT;
	}
  }
  
  return Fun4AllReturnCodes::EVENT_OK;
  
}

int ElectronPid::InitRun(PHCompositeNode* topNode)
{
  int ret = GetNodes(topNode);
  if (ret != Fun4AllReturnCodes::EVENT_OK) return ret;

  return Fun4AllReturnCodes::EVENT_OK;
}

int ElectronPid::process_event(PHCompositeNode* topNode)
{
  EventNumber++;
  float ntp[30];

  cout<<"EventNumber ===================== " << EventNumber-1 << endl;
  if(EventNumber==1) topNode->print();

  // get the tracks
  for(SvtxTrackMap::Iter it = _track_map->begin(); it != _track_map->end(); ++it)
    {

      SvtxTrack *track = it->second;

      double px = track->get_px();
      double py = track->get_py();

      double z = track->get_z();

      unsigned int vtxid = track->get_vertex_id();

      double mom = track->get_p();
      double pt = sqrt(px*px + py*py);
      int charge = track->get_charge();
      int pid = it->first;
      int quality = track->get_quality();

      double e_cluster = track->get_cal_cluster_e(SvtxTrack::CAL_LAYER::CEMC);

      double e_cemc_3x3 = track->get_cal_energy_3x3(SvtxTrack::CAL_LAYER::CEMC);
      double e_hcal_in_3x3 = track->get_cal_energy_3x3(SvtxTrack::CAL_LAYER::HCALIN);
      double e_hcal_out_3x3 = track->get_cal_energy_3x3(SvtxTrack::CAL_LAYER::HCALOUT);

      // CEMC E/p cut
      double cemceoverp = e_cemc_3x3 / mom;
      // HCaline/CEMCe cut
      double hcalineovercemce = e_hcal_in_3x3 / e_cemc_3x3;
      // HCal E/p cut
      double hcaleoverp = (e_hcal_in_3x3 + e_hcal_out_3x3) / mom;

      ntp[0] = mom;
      ntp[1] = pt;
      ntp[2] = e_cemc_3x3;
      ntp[3] = e_hcal_in_3x3;
      ntp[4] = e_hcal_out_3x3;
      ntp[5] = cemceoverp;
      ntp[6] = hcalineovercemce;
      ntp[7] = hcaleoverp;
      ntp[8] = charge;
      ntp[9] = pid;
      ntp[10] = quality;
      ntp[11] = e_cluster;
      ntp[12] = EventNumber;
      ntp[13] = z;
      ntp[14] = vtxid;
      if(output_ntuple) { ntpbeforecut -> Fill(ntp); }

	//std::cout << " Pt_lowerlimit " << Pt_lowerlimit << " Pt_higherlimit " << Pt_higherlimit << " HOP_lowerlimit " << HOP_lowerlimit <<std::endl;
        //std::cout << " EMOP_lowerlimit " << EMOP_lowerlimit << " EMOP_higherlimit " << EMOP_higherlimit << " HinOEM_higherlimit " << HinOEM_higherlimit <<std::endl;

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////electrons
     // if(cemceoverp > EMOP_lowerlimit && cemceoverp < EMOP_higherlimit && quality < 10)
     if(cemceoverp > EMOP_lowerlimit && cemceoverp < EMOP_higherlimit && quality < 5)
	{
	
	  ntp[0] = mom;
    	  ntp[1] = pt;
    	  ntp[2] = e_cemc_3x3;
    	  ntp[3] = e_hcal_in_3x3;
    	  ntp[4] = e_hcal_out_3x3;
    	  ntp[5] = charge;
    	  ntp[6] = pid;
	  ntp[7] = quality;
	  ntp[8] = e_cluster;
	  ntp[9] = EventNumber;
	  ntp[10] = z;
  	  ntp[11] = vtxid;
  	  if(output_ntuple) { ntpcutEMOP -> Fill(ntp); }

	  if(hcalineovercemce < HinOEM_higherlimit)
	     {

		  ntp[0] = mom;
    		  ntp[1] = pt;
    		  ntp[2] = e_cemc_3x3;
    		  ntp[3] = e_hcal_in_3x3;
    		  ntp[4] = e_hcal_out_3x3;
    		  ntp[5] = charge;
    		  ntp[6] = pid;
		  ntp[7] = quality;
		  ntp[8] = e_cluster;
		  ntp[9] = EventNumber;
	  	  ntp[10] = z;
  		  ntp[11] = vtxid;
  		  if(output_ntuple) { ntpcutEMOP_HinOEM -> Fill(ntp); }

		  if( pt > Pt_lowerlimit && pt < Pt_higherlimit)
	  	    {

			  ntp[0] = mom;
    			  ntp[1] = pt;
    			  ntp[2] = e_cemc_3x3;
    			  ntp[3] = e_hcal_in_3x3;
    			  ntp[4] = e_hcal_out_3x3;
    			  ntp[5] = charge;
    			  ntp[6] = pid;
			  ntp[7] = quality;
			  ntp[8] = e_cluster;
			  ntp[9] = EventNumber;
			  ntp[10] = z;
  			  ntp[11] = vtxid;
  			  if(output_ntuple) { ntpcutEMOP_HinOEM_Pt -> Fill(ntp); }
   	 	
	 		  if(Verbosity() > 0) {
	 	 	   	std::cout << " Track " << it->first  << " identified as electron " << "    mom " << mom << " e_cemc_3x3 " << e_cemc_3x3  << " cemceoverp " << cemceoverp << " e_hcal_in_3x3 " << e_hcal_in_3x3 << " e_hcal_out_3x3 " << e_hcal_out_3x3 << std::endl; 
			   }
		  
			  // add to the association map
	 		  _track_pid_assoc->addAssoc(TrackPidAssoc::electron, it->second->get_id());
		   }
   	    }
	}
      
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////hadrons
     
     // if(hcaleoverp > HOP_lowerlimit && quality < 10)
      if(hcaleoverp > HOP_lowerlimit && quality < 5)
	{
      
	  ntp[0] = mom;
    	  ntp[1] = pt;
    	  ntp[2] = e_cemc_3x3;
    	  ntp[3] = e_hcal_in_3x3;
    	  ntp[4] = e_hcal_out_3x3;
    	  ntp[5] = charge;
    	  ntp[6] = pid;
	  ntp[7] = quality;
    	  ntp[8] = e_cluster;
	  ntp[9] = EventNumber;
	  ntp[10] = z;
  	  ntp[11] = vtxid;
  	  if(output_ntuple) { ntpcutHOP -> Fill(ntp); }

	  if(Verbosity() > 0) {
		std::cout << " Track " << it->first  << " identified as hadron " << "    mom " << mom << " e_cemc_3x3 " << e_cemc_3x3  << " hcaleoverp " << hcaleoverp << " e_hcal_in_3x3 " << e_hcal_in_3x3 << " e_hcal_out_3x3 " << e_hcal_out_3x3 << std::endl; 
	    }

	  // add to the association map
	  _track_pid_assoc->addAssoc(TrackPidAssoc::hadron, it->second->get_id());
     	}
      

    }
  
  // Read back the association map

  if(Verbosity() > 1)
    std::cout << "Read back the association map electron entries" << std::endl;
  auto electrons = _track_pid_assoc->getTracks(TrackPidAssoc::electron);
  for(auto it = electrons.first; it != electrons.second; ++it)
    {
      SvtxTrack *tr = _track_map->get(it->second);
      double p = tr->get_p();

      if(Verbosity() > 1)
	std::cout << " pid " << it->first << " track ID " << it->second << " mom " << p << std::endl;
    }

  if(Verbosity() > 1)
    std::cout << "Read back the association map hadron entries" << std::endl;
  auto hadrons = _track_pid_assoc->getTracks(TrackPidAssoc::hadron);
  for(auto it = hadrons.first; it != hadrons.second; ++it)
    {
      SvtxTrack *tr = _track_map->get(it->second);
      double p = tr->get_p();

      if(Verbosity() > 1)
	std::cout << " pid " << it->first << " track ID " << it->second << " mom " << p << std::endl;
    }

  
  return Fun4AllReturnCodes::EVENT_OK;
}


int ElectronPid::GetNodes(PHCompositeNode* topNode)
{
  _track_map = findNode::getClass<SvtxTrackMap>(topNode, "SvtxTrackMap");

  // if the TrackPidAssoc node is not present, create it
  _track_pid_assoc =  findNode::getClass<TrackPidAssoc>(topNode, "TrackPidAssoc");
  if(!_track_pid_assoc)
    {
      PHNodeIterator iter(topNode);      
      PHCompositeNode* dstNode = static_cast<PHCompositeNode*>(iter.findFirst(
									      "PHCompositeNode", "DST"));
      if (!dstNode)
	{
	  cerr << PHWHERE << "DST Node missing, quit!" << endl;
	  return Fun4AllReturnCodes::ABORTEVENT;
	}

     // Get the SVTX node
      PHNodeIterator iter_dst(dstNode);
      PHCompositeNode* tb_node =
	dynamic_cast<PHCompositeNode*>(iter_dst.findFirst("PHCompositeNode",
							  "SVTX"));
      if (!tb_node)
	{
	  cout << PHWHERE << "SVTX node missing, quit!" << endl;
	  return Fun4AllReturnCodes::ABORTEVENT;
	}

      // now add the new node  
      _track_pid_assoc = new TrackPidAssoc;
      PHIODataNode<PHObject>* assoc_node = new PHIODataNode<PHObject>(
								      _track_pid_assoc, "TrackPidAssoc", "PHObject");
      tb_node->addNode(assoc_node);
      if (Verbosity() > 0)
	cout << PHWHERE << "Svtx/TrackPidAssoc node added" << endl;
    }
  
  return Fun4AllReturnCodes::EVENT_OK;
}

int ElectronPid::End(PHCompositeNode * /*topNode*/)
{
if(output_ntuple) {
  OutputNtupleFile -> cd();
  OutputNtupleFile -> Write();
  OutputNtupleFile -> Close();
}

  cout << "************END************" << endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

