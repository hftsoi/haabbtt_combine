# haabbtt_combine
Follow instructions to set up (see http://cms-analysis.github.io/CombineHarvester/):
```
export SCRAM_ARCH=slc6_amd64_gcc530
scram project CMSSW CMSSW_10_2_13
cd CMSSW_10_2_13/src
cmsenv
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
# IMPORTANT: Checkout the recommended tag on the link above
git clone https://github.com/cms-analysis/CombineHarvester.git CombineHarvester
scram b
```

mkdir auxiliaries/shapes and store final root files there

Copy the main code haabbtt.cpp to CombineHarvester/CombineTools/bin/ and add to BuildFile.xml

Compile: scram b -j 8
