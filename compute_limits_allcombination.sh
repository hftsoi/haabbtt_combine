channel=(mt et em)
year=(2018 2017 2016)
#mass=(12 15 20 25 30 35 40 45 50 55 60)
mass=$1
for mass in "${mass[@]}"
do
   for year in "${year[@]}"
   do
      for channel in "${channel[@]}"
      do
         if [ $channel == em ] || [ $channel == mt ]
         then
            cat=(1 2 3 4 5 6 7)
         else
            cat=(1 2 3 4 5 6)
         fi
         for i in "${cat[@]}"
         do
            echo "* autoMCStats 0.0" >> "haabbtt_${channel}_${i}_${year}_${mass}.txt"
         done
      done
   done
   combineCards.py "haabbtt_"*"_${mass}.txt" > "combined_allchannels_allyears_${mass}.txt"
   text2workspace.py "combined_allchannels_allyears_${mass}.txt" -m $mass
   combine -M AsymptoticLimits "combined_allchannels_allyears_${mass}.root" -t -1 -m $mass
   mkdir -p "allchannels_allyears"/"${mass}"
   mv "higgsCombineTest.AsymptoticLimits.mH${mass}.root" "allchannels_allyears"/"${mass}"/
done
#combineTool.py -M CollectLimits "allchannels_allyears"/*/*Test.* --use-dirs -o "allchannels_allyears.json"
#plotLimits.py "allchannels_allyears_allchannels_allyears.json" --auto-style -o "allchannels_allyears"
