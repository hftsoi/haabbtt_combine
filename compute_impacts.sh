channel=$1 #et/mt/em/all
year=$2 #2016/2017/2018/all
haabbtt ${channel} ${year}
combineCards.py "haabbtt_${channel}_"*"_${year}_35.txt" > "combined_${channel}_${year}_35.txt"
text2workspace.py "combined_${channel}_${year}_35.txt" -m 35
combineTool.py -M Impacts -d "combined_${channel}_${year}_35.root" -m 35 --doInitialFit --robustFit 1 -t -1 --expectSignal=0 --rMin=-40 --rMax=40
combineTool.py -M Impacts -d "combined_${channel}_${year}_35.root" -m 35 --robustFit 1 --doFits --parallel=150 -t -1 --expectSignal=0 --rMin=-40 --rMax=40
combineTool.py -M Impacts -d "combined_${channel}_${year}_35.root" -m 35 -o impacts.json
plotImpacts.py -i impacts.json -o impacts_${channel}_${year}
rm *.txt *.root *.json *.out
