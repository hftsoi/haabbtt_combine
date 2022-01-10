channel=$1
year=(2018 2017 2016)
mass=(12 15 20 25 30 35 40 45 50 55 60)
if [ $channel == em ] || [ $channel == mt ]
then
   cat=(1 2 3 4 5 6 7)
else
   cat=(1 2 3 4 5 6)
fi
haabbtt $channel 2018
haabbtt $channel 2017
haabbtt $channel 2016
for mass in "${mass[@]}"
do
   for year in "${year[@]}"
   do
      for i in "${cat[@]}"
      do
         echo "* autoMCStats 0.0" >> "haabbtt_${channel}_${i}_${year}_${mass}.txt"
      done
   done
   combineCards.py "haabbtt_${channel}_"*"_${mass}.txt" > "combined_${channel}_allyears_${mass}.txt"
   text2workspace.py "combined_${channel}_allyears_${mass}.txt" -m $mass
   combine -M AsymptoticLimits "combined_${channel}_allyears_${mass}.root" -t -1 -m $mass
   mkdir -p "${channel}_allyears"/"${mass}"
   mv "higgsCombineTest.AsymptoticLimits.mH${mass}.root" "${channel}_allyears"/"${mass}"/
done
combineTool.py -M CollectLimits "${channel}_allyears"/*/*Test.* --use-dirs -o "${channel}_allyears.json"
plotLimits.py "${channel}_allyears_${channel}_allyears.json" --auto-style -o "${channel}_allyears"
