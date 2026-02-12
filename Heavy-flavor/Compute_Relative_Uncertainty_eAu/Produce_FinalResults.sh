#!/bin/bash
#script to run the machine learning differential in pT and y
# Shyam Kumar; INFN Bari, Italy
rm *.txt *.png
source Results_wo_ML.sh
source Results_w_ML.sh
root -b -l -q Compare_relative_uncertainty_ePIC.C


