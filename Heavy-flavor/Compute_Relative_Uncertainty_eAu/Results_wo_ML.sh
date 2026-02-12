#!/bin/bash
#script to run the machine learning differential in pT and y
# Shyam Kumar; INFN Bari, Italy
y_arr=(-2.0 -1.0) 
pt_arr=(0.0 1.0 2.0 5.0)

for ((i=0; i<${#y_arr[@]}-1; i++)); do
for ((j=0; j<${#pt_arr[@]}-1; j++)); do
root -b -l -q RelUncertainty_D0.C'('${y_arr[i]}', '${y_arr[i+1]}', '${pt_arr[j]}', '${pt_arr[j+1]}')'
done
done

y_arr=(-1.0 1.0 3.0) 
pt_arr=(0.0 1.0 2.0 5.0 10.0)

for ((i=0; i<${#y_arr[@]}-1; i++)); do
for ((j=0; j<${#pt_arr[@]}-1; j++)); do
root -b -l -q RelUncertainty_D0.C'('${y_arr[i]}', '${y_arr[i+1]}', '${pt_arr[j]}', '${pt_arr[j+1]}')'
done
done


