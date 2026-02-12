#!/bin/bash
#script to run the machine learning differential in pT and y
# Shyam Kumar; INFN Bari, Italy
rm *.png
prepare_sample=true  

if $prepare_sample; then
find . -name "*.root" -type f -delete # delete all root files in directory
find . -name "*.png" -type f -delete # delete all png files in directory
find . -name "*.txt" -type f -delete # delete all png files in directory
cd Data_Preparation/Filtered_D0Sample/
root -b -l -q Create_Signal_with_Cuts.C
root -b -l -q Create_bkg_with_Cuts.C
cd ../Filtered_DISSample
root -b -l -q Create_Signal_with_Cuts.C
root -b -l -q Create_bkg_with_Cuts.C
cd ../../ 
# add the two signals
hadd Data_Preparation/SignalD0.root  Data_Preparation/Filtered_D0Sample/SignalD0.root Data_Preparation/Filtered_DISSample/SignalD0.root 
hadd Data_Preparation/BkgD0.root Data_Preparation/Filtered_D0Sample/BkgD0.root Data_Preparation/Filtered_DISSample/BkgD0.root 
fi
cd Data_Preparation/Merge_Data/
source Merged_Data.sh
cd ../../
y_arr=(-2.0 -1.0 1.0 2.0 3.0) 
pt_arr=(1.0 20.0)

for ((i=0; i<${#y_arr[@]}-1; i++)); do
for ((j=0; j<${#pt_arr[@]}-1; j++)); do
rm -rf ML_Output_Optuna_${y_arr[i]}_${y_arr[i+1]}_${pt_arr[j]}_${pt_arr[j+1]} && mkdir ML_Output_Optuna_${y_arr[i]}_${y_arr[i+1]}_${pt_arr[j]}_${pt_arr[j+1]}
python3 machine_learning_Final.py --ymin ${y_arr[i]} --ymax ${y_arr[i+1]} --ptmin ${pt_arr[j]} --ptmax ${pt_arr[j+1]}
root -b -l -q draw_soverb_significance.C'('${y_arr[i]}', '${y_arr[i+1]}','${pt_arr[j]}','${pt_arr[j+1]}')'
done
done

