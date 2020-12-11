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

int main() {

  // Where the datacard is stored
  string aux_shapes = string(getenv("CMSSW_BASE")) + "/src/auxiliaries/shapes/";

  ch::CombineHarvester cb;

  // List of categories
  ch::Categories cats = {
      {1, "m_tt_1"},
      {2, "m_tt_2"},
      {3, "m_tt_3"},
      {4, "m_tt_4"},
      {5, "m_tt_5"}
    };

  // List of mass points for the signal, the masses are added at the end of the signal string names
  vector<string> masses = {"12","15","20","25","30","35","40","45","50","55","60"}; // can be a vector with several mass points

  cb.AddObservations({"*"}, {"haabbtt"}, {"2018"}, {"mt"}, cats); // keep haabbtt for this analysis, change tt to the final state (eg em for emu). Same in the rest of the code

  // List of backgrounds in the datacards
  vector<string> bkg_procs = {"HTT","HWW","ZTT","TT","VV","Z","ST","ttHnonbb","QCD"};
  cb.AddProcesses({"*"}, {"haabbtt"}, {"2018"}, {"mt"}, bkg_procs, cats, false);

  // List of signals in the datacards. The mass will be concatenated at the end by the code
  vector<string> sig_procs = {"gghbbtt","vbfbbtt"};
  cb.AddProcesses(masses, {"haabbtt"}, {"2018"}, {"mt"}, sig_procs, cats, true);


  using ch::syst::SystMap;
  using ch::syst::era;
  using ch::syst::bin_id;
  using ch::syst::process;

  // Normalization uncertainties, to be improved. Adapt depending on your background list

  cb.cp().process(ch::JoinStr({{"HTT","HWW","TT","VV","Z","ST","ttHnonbb"},sig_procs})).AddSyst(cb, "CMS_normalization_muID_13TeV", "lnN", SystMap<>::init(1.02));
  cb.cp().process(ch::JoinStr({{"HTT","HWW","TT","VV","Z","ST","ttHnonbb"},sig_procs})).AddSyst(cb, "CMS_normalization_tauID_13TeV", "lnN", SystMap<>::init(1.05));
  cb.cp().process(ch::JoinStr({{"HTT","HWW","TT","VV","Z","ST","ttHnonbb"},sig_procs})).AddSyst(cb, "CMS_normalization_mutrg_13TeV", "lnN", SystMap<>::init(1.02));
  cb.cp().process({"Z"}).AddSyst(cb, "CMS_normalization_Z_13TeV", "lnN", SystMap<>::init(1.02));
  cb.cp().process({"HTT"}).AddSyst(cb, "CMS_normalization_HTT_13TeV", "lnN", SystMap<>::init(1.018));
  cb.cp().process({"HWW"}).AddSyst(cb, "CMS_normalization_HWW_13TeV", "lnN", SystMap<>::init(1.015));
  cb.cp().process({"gghbbtt12","gghbbtt15","gghbbtt20","gghbbtt25","gghbbtt30","gghbbtt35","gghbbtt40","gghbbtt45","gghbbtt50","gghbbtt55","gghbbtt60"}).AddSyst(cb, "CMS_normaliztion_pdf_Higgs_gg_13TeV", "lnN", SystMap<>::init(1.032));
  cb.cp().process({"vbfbbtt12","vbfbbtt15","vbfbbtt20","vbfbbtt25","vbfbbtt30","vbfbbtt35","vbfbbtt40","vbfbbtt45","vbfbbtt50","vbfbbtt55","vbfbbtt60"}).AddSyst(cb, "CMS_normaliztion_pdf_Higgs_qq_13TeV", "lnN", SystMap<>::init(1.021));
  cb.cp().process(ch::JoinStr({{"TT","ST"},sig_procs})).AddSyst(cb, "CMS_normaliztion_btagging_13TeV", "lnN", SystMap<>::init(1.05));
  cb.cp().process({"TT"}).AddSyst(cb, "CMS_normalization_ttbar_13TeV", "lnN", SystMap<>::init(1.042));
  cb.cp().process({"VV"}).AddSyst(cb, "CMS_normalization_diboson_13TeV", "lnN", SystMap<>::init(1.05));
  cb.cp().process({"ST"}).AddSyst(cb, "CMS_normalization_singletop_13TeV", "lnN", SystMap<>::init(1.05));
  cb.cp().process({"Z"}).AddSyst(cb, "CMS_normalization_Z_misID_mutotauh_13TeV", "lnN", SystMap<>::init(1.2));
  cb.cp().process(ch::JoinStr({{"HTT","HWW","TT","VV","Z","ST","ttHnonbb"},sig_procs})).AddSyst(cb, "CMS_normalization_lumi_13TeV", "lnN", SystMap<>::init(1.025));
  cb.cp().process({"QCD"}).AddSyst(cb, "CMS_normalization_qcd_13TeV", "lnN", SystMap<>::init(1.20));
  cb.cp().process({"ZTT"}).AddSyst(cb, "CMS_normalization_embedded_muID_13TeV", "lnN", SystMap<>::init(1.02));
  cb.cp().process({"ZTT"}).AddSyst(cb, "CMS_normalization_embedded_tauID_13TeV", "lnN", SystMap<>::init(1.05));
  cb.cp().process({"ZTT"}).AddSyst(cb, "CMS_normalization_embedded_mutrg_13TeV", "lnN", SystMap<>::init(1.02));
  cb.cp().process({"ZTT"}).AddSyst(cb, "CMS_normalization_embedded_13TeV", "lnN", SystMap<>::init(1.04));

  // Shape uncertainties to be added later


  // Name of the input datacard
  cb.cp().backgrounds().ExtractShapes(
      aux_shapes + "final_mt_2018.root",
      "$BIN/$PROCESS",
      "$BIN/$PROCESS_$SYSTEMATIC");
  cb.cp().signals().ExtractShapes(
      aux_shapes + "final_mt_2018.root",
      "$BIN/$PROCESS$MASS",
      "$BIN/$PROCESS$MASS_$SYSTEMATIC");

  ch::SetStandardBinNames(cb);

  set<string> bins = cb.bin_set();

  TFile output("haabbtt2018_mt.input.root", "RECREATE");

  for (auto b : bins) {
    for (auto m : masses) {
      cout << ">> Writing datacard for bin: " << b << " and mass: " << m
           << "\n";
      cb.cp().bin({b}).mass({m, "*"}).WriteDatacard(
          b + "_" + m + ".txt", output);
    }
  }

}

