channel="mt"
year="2016"
mass=(15 20 25 30 35 40 45 50 55 60)
cat=(1 2 3 4)
for mass in "${mass[@]}"
do
   combineCards.py "haabbtt_${channel}_"*"_${year}_${mass}.txt" > "combined_${channel}_${year}_${mass}.txt"
   echo "lumiscale rateParam * * 1" >> "combined_${channel}_${year}_${mass}.txt"
   text2workspace.py "combined_${channel}_${year}_${mass}.txt" -m $mass
   combine -M AsymptoticLimits "combined_${channel}_${year}_${mass}.root" -t -1 -m $mass --setParameters lumiscale=83.6
   mkdir -p "${channel}_${year}"/"${mass}"
   mv "higgsCombineTest.AsymptoticLimits.mH${mass}.root" "${channel}_${year}"/"${mass}"/
done
combineTool.py -M CollectLimits "${channel}_${year}"/*/*Test.* --use-dirs -o "${channel}_${year}.json"
plotLimits.py "${channel}_${year}_${channel}_${year}.json" --auto-style -o "${channel}_${year}"
rm *.root *.txt *.pdf
rm -r "${channel}_${year}"
