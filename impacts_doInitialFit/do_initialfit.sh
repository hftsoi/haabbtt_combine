#!/bin/bash
set -x
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc530
scram project CMSSW CMSSW_10_2_13
cd CMSSW_10_2_13/src
eval `scram runtime -sh`
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
git clone https://github.com/cms-analysis/CombineHarvester.git CombineHarvester
scram b

# store the output of text2workspace in the inputfiles folder
mv ../../inputfiles/* ./
combineTool.py -M Impacts -d combined.root -m 35 --doInitialFit --robustFit 1 -t -1 --expectSignal=0 --rMin=-10 --rMax=10
mv higgsCombine_initialFit*.root ../../
