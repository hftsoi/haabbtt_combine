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

# store the output root file from doInitialFit (higgsCombine_initialFit*.root) and the workspace combined.root in the inputfiles folder
mv ../../inputfiles/* ./
combineTool.py -M Impacts -d combined.root -m 35 --robustFit 1 --doFits -t -1 --expectSignal=0 --rMin=-10 --rMax=10 --job-mode condor --sub-opts='+JobFlavour="workday"\ntransfer_input_files = inputfiles\nrequestmemory = 8G' --merge 1 --task-name rundoFits --dry-run

### JobFlavour sets the maximum runtime of a job. Default is "expresso" if no argument is passed.
# espresso     = 20 minutes
# microcentury = 1 hour
# longlunch    = 2 hours
# workday      = 8 hours
# tomorrow     = 1 day
# testmatch    = 3 days
# nextweek     = 1 week

mv condor_* ../../

# after that, remember to edit the output executable condor_rundoFits.sh
# 1) at the top the environment and directory path, same as above
# 2) at the end move the output higgsCombine_paramFit_*.root back to submit machine
