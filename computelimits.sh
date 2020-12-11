channel="em"
year="2017"
mass=(12 20 30 40 50 60)
cat=(1 2 3 4 5)
for mass in "${mass[@]}"
do
   for i in "${cat[@]}"
   do
      echo "* autoMCStats 0.0" >> "haabbtt_${channel}_${i}_${year}_${mass}.txt"
   done
   combineCards.py "haabbtt_${channel}_"*"_${year}_${mass}.txt" > "combined_${channel}_${year}_${mass}.txt"
   text2workspace.py "combined_${channel}_${year}_${mass}.txt" -m $mass
   combine -M AsymptoticLimits "combined_${channel}_${year}_${mass}.root" -t -1 -m $mass
   mkdir -p "${channel}_${year}"/"${mass}"
   mv "higgsCombineTest.AsymptoticLimits.mH${mass}.root" "${channel}_${year}"/"${mass}"/
done
combineTool.py -M CollectLimits "${channel}_${year}"/*/*Test.* --use-dirs -o "${channel}_${year}.json"
plotLimits.py "${channel}_${year}_${channel}_${year}.json" --auto-style -o "${channel}_${year}"
