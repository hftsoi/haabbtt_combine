#!/bin/bash
set -x
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc530
scram project CMSSW CMSSW_10_2_13
cd CMSSW_10_2_13/src
eval `scram runtime -sh`
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
scram b

# store *.input.root and the combined txt datacard in the inputfiles folder
mv ../../inputfiles/* ./
text2workspace.py combined.txt -m 35
mv combined.root ../../
