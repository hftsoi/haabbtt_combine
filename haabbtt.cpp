#include <string>
#include <map>
#include <set>
#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"
#include "CombineHarvester/CombineTools/interface/Observation.h"
#include "CombineHarvester/CombineTools/interface/Process.h"
#include "CombineHarvester/CombineTools/interface/Utilities.h"
#include "CombineHarvester/CombineTools/interface/Systematics.h"
#include "CombineHarvester/CombineTools/interface/BinByBin.h"

using namespace std;

int main(int argc, char** argv) {
    
    std::string channel = *(argv + 1);
    std::string year = *(argv + 2);
    
    // Where the datacard is stored
    string aux_shapes = string(getenv("CMSSW_BASE")) + "/src/auxiliaries/shapes/";
    
    ch::CombineHarvester cb;
    // Uncomment this next line to see a *lot* of debug information
    // cb.SetVerbosity(3);
    
    // List of categories (also called 'bins' in Combine)
    ch::Categories cats = {
        {1, "m_tt_1b"},
        {2, "m_tt_2b"}
    };
    
    // List of mass points for the signal, the masses are added at the end of the signal string names
    vector<string> masses = {"12","15","20","25","30","35","40","45","50","55","60"};
    
    // Observed data (name must be "data_obs" in datacards)
    cb.AddObservations({"*"}, {"haabbtt"}, {year}, {channel}, cats);
    
    // List of backgrounds in the datacards
    vector<string> bkg_procs = {"ttbar","ZJ","ST","VV","ggh_htt","ggh_hww","qqh_htt","qqh_hww","Zh_htt","Zh_hww","Wh_htt","Wh_hww","tth"};
    if (channel=="em") bkg_procs.push_back("WJ");
    
    vector<string> bkg_procs_noEMB_nofake = bkg_procs;
    
    bkg_procs.push_back("embedded");
    bkg_procs.push_back("fake");
    
    cb.AddProcesses({"*"}, {"haabbtt"}, {year}, {channel}, bkg_procs, cats, false);
    
    // List of signals in the datacards. The mass will be concatenated at the end by the code
    vector<string> sig_procs = {"gghbbtt","vbfbbtt"};
    cb.AddProcesses(masses, {"haabbtt"}, {year}, {channel}, sig_procs, cats, true);
    
    vector<string> sig_ggh = {"gghbbtt12","gghbbtt15","gghbbtt20","gghbbtt25","gghbbtt30","gghbbtt35","gghbbtt40","gghbbtt45","gghbbtt50","gghbbtt55","gghbbtt60"};
    vector<string> sig_vbf = {"vbfbbtt12","vbfbbtt15","vbfbbtt20","vbfbbtt25","vbfbbtt30","vbfbbtt35","vbfbbtt40","vbfbbtt45","vbfbbtt50","vbfbbtt55","vbfbbtt60"};
    
    using ch::syst::SystMap;
    using ch::syst::era;
    using ch::syst::bin_id;
    using ch::syst::process;
    using ch::JoinStr;
    
    // =========================== Normalization uncertainties ===========================
    // The AddSyst method supports {$BIN, $PROCESS, $MASS, $ERA, $CHANNEL, $ANALYSIS}
    cb.cp().process({"ggh_htt","qqh_htt","Zh_htt","Wh_htt"}).AddSyst(cb, "CMS_htt_BR", "lnN", SystMap<>::init(1.018));
    cb.cp().process({"ggh_hww","qqh_hww","Zh_hww","Wh_hww"}).AddSyst(cb, "CMS_hww_BR", "lnN", SystMap<>::init(1.015));
    cb.cp().process(JoinStr({{"ggh_htt","ggh_hww"},sig_ggh})).AddSyst(cb, "pdf_Higgs_gg", "lnN", SystMap<>::init(1.032));
    cb.cp().process(JoinStr({{"qqh_htt","qqh_hww"},sig_vbf})).AddSyst(cb, "pdf_Higgs_qq", "lnN", SystMap<>::init(1.021));
    cb.cp().process({"ZJ"}).AddSyst(cb, "CMS_normalization_Z_13TeV", "lnN", SystMap<>::init(1.02));
    cb.cp().process({"ttbar"}).AddSyst(cb, "CMS_ttbar_xs", "lnN", SystMap<>::init(1.042));
    cb.cp().process({"VV"}).AddSyst(cb, "CMS_VV_xs", "lnN", SystMap<>::init(1.05));
    cb.cp().process({"ST"}).AddSyst(cb, "CMS_ST_xs", "lnN", SystMap<>::init(1.05));
    cb.cp().process({"embedded"}).AddSyst(cb, "CMS_normalization_embedded_13TeV", "lnN", SystMap<>::init(1.04));
    // Luminosity uncertainties, no embedded
    if (year=="2016"){
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "lumi_13TeV_2016", "lnN", SystMap<>::init(1.01));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "lumi_13TeV_correlated16-18", "lnN", SystMap<>::init(1.006));
    }
    if (year=="2017"){
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "lumi_13TeV_2017", "lnN", SystMap<>::init(1.02));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "lumi_13TeV_correlated16-18", "lnN", SystMap<>::init(1.009));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "lumi_13TeV_correlated17-18", "lnN", SystMap<>::init(1.006));
    }
    if (year=="2018"){
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "lumi_13TeV_2018", "lnN", SystMap<>::init(1.015));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "lumi_13TeV_correlated16-18", "lnN", SystMap<>::init(1.02));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "lumi_13TeV_correlated17-18", "lnN", SystMap<>::init(1.002));
    }
    
    if (channel=="em"){
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "CMS_eleID_13TeV", "lnN", SystMap<>::init(1.02));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "CMS_muID_13TeV", "lnN", SystMap<>::init(1.02));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "CMS_emutrg_13TeV", "lnN", SystMap<>::init(1.02));
        // eleID 50% correlated with MC
        cb.cp().process({"embedded"}).AddSyst(cb, "CMS_eleID_13TeV", "lnN", SystMap<>::init(1.01));// 2% * 50%
        cb.cp().process({"embedded"}).AddSyst(cb, "CMS_EMB_eleID_13TeV", "lnN", SystMap<>::init(1.01732));// 2% * sqrt(1-50%^2)
        // muID 50% correlated with MC
        cb.cp().process({"embedded"}).AddSyst(cb, "CMS_muID_13TeV", "lnN", SystMap<>::init(1.01));
        cb.cp().process({"embedded"}).AddSyst(cb, "CMS_EMB_muID_13TeV", "lnN", SystMap<>::init(1.01732));
        
        cb.cp().process({"embedded"}).AddSyst(cb, "CMS_EMB_emutrg_13TeV", "lnN", SystMap<>::init(1.02));
        cb.cp().process({"fake"}).AddSyst(cb, "CMS_normalization_qcd_13TeV", "lnN", SystMap<>::init(1.20));
    }
    
    if (channel=="et"){
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "CMS_eleID_13TeV", "lnN", SystMap<>::init(1.02));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "CMS_etrg_13TeV", "lnN", SystMap<>::init(1.02));
        // eleID 50% correlated with MC
        cb.cp().process({"embedded"}).AddSyst(cb, "CMS_eleID_13TeV", "lnN", SystMap<>::init(1.01));
        cb.cp().process({"embedded"}).AddSyst(cb, "CMS_EMB_eleID_13TeV", "lnN", SystMap<>::init(1.01732));
        
        cb.cp().process({"embedded"}).AddSyst(cb, "CMS_EMB_etrg_13TeV", "lnN", SystMap<>::init(1.02));
        cb.cp().process({"ZJ"}).AddSyst(cb, "CMS_Z_misID_13TeV", "lnN", SystMap<>::init(1.20));
        cb.cp().process({"fake"}).AddSyst(cb, "CMS_normalization_fake_13TeV", "lnN", SystMap<>::init(1.20));
    }
    
    if (channel=="mt"){
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "CMS_muID_13TeV", "lnN", SystMap<>::init(1.02));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb, "CMS_mutrg_13TeV", "lnN", SystMap<>::init(1.02));
        // muID 50% correlated with MC
        cb.cp().process({"embedded"}).AddSyst(cb, "CMS_muID_13TeV", "lnN", SystMap<>::init(1.01));
        cb.cp().process({"embedded"}).AddSyst(cb, "CMS_EMB_muID_13TeV", "lnN", SystMap<>::init(1.01732));
        
        cb.cp().process({"embedded"}).AddSyst(cb, "CMS_EMB_mutrg_13TeV", "lnN", SystMap<>::init(1.02));
        cb.cp().process({"ZJ"}).AddSyst(cb, "CMS_Z_misID_13TeV", "lnN", SystMap<>::init(1.20));
        cb.cp().process({"fake"}).AddSyst(cb, "CMS_normalization_fake_13TeV", "lnN", SystMap<>::init(1.20));
    }
    
    // =========================== Shape uncertainties ===========================
    // The AddSyst method supports {$BIN, $PROCESS, $MASS, $ERA, $CHANNEL, $ANALYSIS}
    
    // btagging efficiency, no embedded
    cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_btagsf_heavy_$ERA", "shape", SystMap<>::init(1.00));
    cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_btagsf_light_$ERA", "shape", SystMap<>::init(1.00));
    
    // Trigger efficiency
    if (channel=="et" or channel=="mt"){
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_trgeff_single_$CHANNEL_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_trgeff_cross_$CHANNEL_$ERA", "shape", SystMap<>::init(1.00));
        // 50% correlated with MC
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_trgeff_single_$CHANNEL_$ERA", "shape", SystMap<>::init(0.50));// 1.00 * 50%
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_trgeff_cross_$CHANNEL_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_trgeff_single_$CHANNEL_$ERA", "shape", SystMap<>::init(0.866));// 1.00 * sqrt(1-50%^2)
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_trgeff_cross_$CHANNEL_$ERA", "shape", SystMap<>::init(0.866));
    }
    if (channel=="em"){
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_trgeff_Mu8E23_$CHANNEL_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_trgeff_Mu23E12_$CHANNEL_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_trgeff_both_$CHANNEL_$ERA", "shape", SystMap<>::init(1.00));
        // 50% correlated with MC
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_trgeff_Mu8E23_$CHANNEL_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_trgeff_Mu23E12_$CHANNEL_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_trgeff_both_$CHANNEL_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_trgeff_Mu8E23_$CHANNEL_$ERA", "shape", SystMap<>::init(0.866));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_trgeff_Mu23E12_$CHANNEL_$ERA", "shape", SystMap<>::init(0.866));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_trgeff_both_$CHANNEL_$ERA", "shape", SystMap<>::init(0.866));
    }
    
    // tau related corrections, no fake bkg
    if (channel=="et" or channel=="mt"){
        // tau ID efficiency (VSjet)
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_tauideff_pt20to25_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_tauideff_pt25to30_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_tauideff_pt30to35_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_tauideff_pt35to40_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_tauideff_pt40to500_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_tauideff_pt500to1000_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_tauideff_ptgt1000_$ERA", "shape", SystMap<>::init(1.00));
        // 50% correlated with MC
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_tauideff_pt20to25_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_tauideff_pt25to30_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_tauideff_pt30to35_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_tauideff_pt35to40_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_tauideff_pt40to500_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_tauideff_pt500to1000_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_tauideff_ptgt1000_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_tauideff_pt20to25_$ERA", "shape", SystMap<>::init(0.866));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_tauideff_pt25to30_$ERA", "shape", SystMap<>::init(0.866));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_tauideff_pt30to35_$ERA", "shape", SystMap<>::init(0.866));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_tauideff_pt35to40_$ERA", "shape", SystMap<>::init(0.866));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_tauideff_pt40to500_$ERA", "shape", SystMap<>::init(0.866));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_tauideff_pt500to1000_$ERA", "shape", SystMap<>::init(0.866));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_tauideff_ptgt1000_$ERA", "shape", SystMap<>::init(0.866));
        
        // tau ID efficiency (VSe), no anti-lepton in embedded
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_tauideff_VSe_bar_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_tauideff_VSe_end_$ERA", "shape", SystMap<>::init(1.00));
        
        // tau ID efficiency (VSmu), no anti-lepton in embedded
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_tauideff_VSmu_eta0to0p4_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_tauideff_VSmu_eta0p4to0p8_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_tauideff_VSmu_eta0p8to1p2_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_tauideff_VSmu_eta1p2to1p7_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_tauideff_VSmu_eta1p7to2p3_$ERA", "shape", SystMap<>::init(1.00));
        
        // tau ES
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_TES_dm0_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_TES_dm1_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_TES_dm10_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_TES_dm11_$ERA", "shape", SystMap<>::init(1.00));
        // 50% correlated with MC
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_TES_dm0_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_TES_dm1_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_TES_dm10_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_TES_dm11_$ERA", "shape", SystMap<>::init(0.50));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_TES_dm0_$ERA", "shape", SystMap<>::init(0.866));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_TES_dm1_$ERA", "shape", SystMap<>::init(0.866));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_TES_dm10_$ERA", "shape", SystMap<>::init(0.866));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_TES_dm11_$ERA", "shape", SystMap<>::init(0.866));
        
        // tau ES (ele fake), no embedded
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_eleTES_dm0_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_eleTES_dm1_$ERA", "shape", SystMap<>::init(1.00));
        
        // tau ES (mu fake), no embedded
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_muTES_dm0_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_muTES_dm1_$ERA", "shape", SystMap<>::init(1.00));
    }
    
    // Leptons ES
    if (channel=="et" or channel=="em"){
        // ele ES
        cb.cp().process(JoinStr({bkg_procs,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_eleES_bar_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_eleES_end_$ERA", "shape", SystMap<>::init(1.00));
    }
    if (channel=="mt" or channel=="em"){
        // mu ES
        cb.cp().process(JoinStr({bkg_procs,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_muES_eta0to1p2_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_muES_eta1p2to2p1_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({bkg_procs,sig_ggh,sig_vbf})).AddSyst(cb,"CMS_muES_eta2p1to2p4_$ERA", "shape", SystMap<>::init(1.00));
    }
    
    // JES, no embedded
    cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_JetAbsolute", "shape", SystMap<>::init(1.00));
    cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_JetAbsolute_$ERA", "shape", SystMap<>::init(1.00));
    cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_JetBBEC1", "shape", SystMap<>::init(1.00));
    cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_JetBBEC1_$ERA", "shape", SystMap<>::init(1.00));
    cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_JetEC2", "shape", SystMap<>::init(1.00));
    cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_JetEC2_$ERA", "shape", SystMap<>::init(1.00));
    cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_JetFlavorQCD", "shape", SystMap<>::init(1.00));
    cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_JetHF", "shape", SystMap<>::init(1.00));
    cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_JetHF_$ERA", "shape", SystMap<>::init(1.00));
    cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_JetRelativeBal", "shape", SystMap<>::init(1.00));
    cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_JetRelativeSample", "shape", SystMap<>::init(1.00));
    cb.cp().process(JoinStr({bkg_procs_noEMB_nofake,{"fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_JER", "shape", SystMap<>::init(1.00));
    
    // recoil correction, for Z+jets, W+jets, ggh and qqh (no W+jets in e+tau and mu+tau)
    // UES uncertainties, for MC without recoil correction
    if (channel=="et" or channel=="mt"){
        cb.cp().process(JoinStr({{"ZJ","ggh_htt","ggh_hww","qqh_htt","qqh_hww","fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_0j_resolution_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({{"ZJ","ggh_htt","ggh_hww","qqh_htt","qqh_hww","fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_0j_response_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({{"ZJ","ggh_htt","ggh_hww","qqh_htt","qqh_hww","fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_1j_resolution_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({{"ZJ","ggh_htt","ggh_hww","qqh_htt","qqh_hww","fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_1j_response_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({{"ZJ","ggh_htt","ggh_hww","qqh_htt","qqh_hww","fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_gt1j_resolution_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({{"ZJ","ggh_htt","ggh_hww","qqh_htt","qqh_hww","fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_gt1j_response_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process({"ttbar","ST","VV","Zh_htt","Zh_hww","Wh_htt","Wh_hww","tth","fake"}).AddSyst(cb,"CMS_UES_$ERA", "shape", SystMap<>::init(1.00));
    }
    if (channel=="em"){
        cb.cp().process(JoinStr({{"ZJ","WJ","ggh_htt","ggh_hww","qqh_htt","qqh_hww","fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_0j_resolution_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({{"ZJ","WJ","ggh_htt","ggh_hww","qqh_htt","qqh_hww","fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_0j_response_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({{"ZJ","WJ","ggh_htt","ggh_hww","qqh_htt","qqh_hww","fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_1j_resolution_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({{"ZJ","WJ","ggh_htt","ggh_hww","qqh_htt","qqh_hww","fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_1j_response_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({{"ZJ","WJ","ggh_htt","ggh_hww","qqh_htt","qqh_hww","fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_gt1j_resolution_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({{"ZJ","WJ","ggh_htt","ggh_hww","qqh_htt","qqh_hww","fake"},sig_ggh,sig_vbf})).AddSyst(cb,"CMS_gt1j_response_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process({"ttbar","ST","VV","Zh_htt","Zh_hww","Wh_htt","Wh_hww","tth","fake"}).AddSyst(cb,"CMS_UES_$ERA", "shape", SystMap<>::init(1.00));
    }
    
    // Z pt reweighting
    cb.cp().process({"ZJ","fake"}).AddSyst(cb,"CMS_Zpt_$ERA", "shape", SystMap<>::init(1.00));
    
    // top pt reweighting
    cb.cp().process({"ttbar","fake"}).AddSyst(cb,"CMS_toppt_$ERA", "shape", SystMap<>::init(1.00));
    
    // tau tracking efficiency in embedded
    if (channel=="et" or channel=="mt"){
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_tautrack_dm0dm10_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_tautrack_dm1_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_EMB_tautrack_dm11_$ERA", "shape", SystMap<>::init(1.00));
    }
    
    // Non-DY MC contamination to embedded
    cb.cp().process({"embedded"}).AddSyst(cb,"CMS_nonDY_$ERA", "shape", SystMap<>::init(1.00));
    
    // Reducible background estimation
    if (channel=="et" or channel=="mt"){
        // fake factor for cross triggered events
        cb.cp().process({"fake"}).AddSyst(cb,"CMS_crosstrg_fakefactor_$ERA", "shape", SystMap<>::init(1.00));
        // fake rate measurement
        cb.cp().process({"fake"}).AddSyst(cb,"CMS_jetFR_pt0to25_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process({"fake"}).AddSyst(cb,"CMS_jetFR_pt25to30_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process({"fake"}).AddSyst(cb,"CMS_jetFR_pt30to35_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process({"fake"}).AddSyst(cb,"CMS_jetFR_pt35to40_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process({"fake"}).AddSyst(cb,"CMS_jetFR_pt40to50_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process({"fake"}).AddSyst(cb,"CMS_jetFR_pt50to60_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process({"fake"}).AddSyst(cb,"CMS_jetFR_ptgt60_$ERA", "shape", SystMap<>::init(1.00));
    }
    if (channel=="em"){
        // SS correction and closure
        cb.cp().process({"fake"}).AddSyst(cb,"CMS_SScorrection_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process({"fake"}).AddSyst(cb,"CMS_SSclosure_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process({"fake"}).AddSyst(cb,"CMS_SSboth2D_$ERA", "shape", SystMap<>::init(1.00));
    }
    
    
    // Name of the input datacard
    // The ExtractShapes method supports {$BIN, $PROCESS, $MASS, $SYSTEMATIC}
    cb.cp().backgrounds().ExtractShapes(
                                        aux_shapes + "final_"+channel+"_"+year+".root",
                                        "$BIN/$PROCESS",
                                        "$BIN/$PROCESS_$SYSTEMATIC");
    cb.cp().signals().ExtractShapes(
                                    aux_shapes + "final_"+channel+"_"+year+".root",
                                    "$BIN/$PROCESS$MASS",
                                    "$BIN/$PROCESS$MASS_$SYSTEMATIC");
    
    ch::SetStandardBinNames(cb);
    
    set<string> bins = cb.bin_set();
    
    TFile output(("haabbtt"+year+"_"+channel+".input.root").c_str(), "RECREATE");
    
    for (auto b : bins) {
        for (auto m : masses) {
            cout << ">> Writing datacard for bin: " << b << " and mass: " << m
            << "\n";
            cb.cp().bin({b}).mass({m, "*"}).WriteDatacard(b + "_" + m + ".txt", output);
        }
    }
    
}

