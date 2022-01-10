channel=$1
year=$2
cp "cards/haabbtt${year}_${channel}.input.root" ./
cp "cards/haabbtt_${channel}_"*"_${year}_35.txt" ./

if [ $channel == em ] || [ $channel == mt ]
then
   cat=(1 2 3 4 5 6 7)
else
   cat=(1 2 3 4 5 6)
fi

for i in "${cat[@]}"
do
   echo "* autoMCStats 0.0" >> "haabbtt_${channel}_${i}_${year}_35.txt"
done

combineCards.py "haabbtt_${channel}_"*"_${year}_35.txt" > "combined_${channel}_${year}_35.txt"
combine -M FitDiagnostics "combined_${channel}_${year}_35.txt" -m 35 --robustFit=1 --rMin=-40 --rMax=40 --saveShapes --saveWithUncertainties
mv fitDiagnostics.root "fitdiag/fitdiag_${channel}_${year}.root"
rm *.root *.txt *.out
