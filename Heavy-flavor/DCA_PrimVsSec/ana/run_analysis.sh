#!/bin/bash

submit=$1

make

if [ -z $submit ]; then
    echo "[i] Set it to submit = 0 for local test"
    submit=0
fi

##################################################
### Local test ###
##################################################
if [ $submit -eq 0 ]; then
    echo "[i] Running locally"
    ./analysis
    exit
fi

###############################################################
### Interactive running for real data using file list       ###
###############################################################

if [ $submit -eq 1 ]; then
    configs=(Geo202409_Real_default)

    for config in "${configs[@]}"; do
	echo "[i] Running config = $config"

	for i in `seq 1 4`; do
	    filelist=input_files/file.${config}.run${i}.list
	    rm $filelist
	    find /eic/u/rongrong/gpfs02/D0/${config}/reco*_run${i}.*root > $filelist
	    output=DIS.D0.${config}.run${i}.root
	    echo $filelist $output
	    ./analysis $filelist $output
	done
    done
fi

###############################################################
### Batch production for real data using file list          ###
###############################################################

if [ $submit -eq 2 ]; then
    #configs=(D0_10x100_hiDiv_q2_100_24.12.0 D0_10x100_hiDiv_q2_1_24.12.0 DIS_10x100_hiDiv_q2_100_24.12.0 DIS_10x100_hiDiv_q2_1_24.12.0)
    configs=(D0_10x100_hiDiv_q2_1_25.10.3)
    #configs=(D0_10x100_hiDiv_q2_1_25.04.1 DIS_10x100_hiDiv_q2_1_25.04.1)
    #configs=(DIS_10x100_hiDiv_q2_1_25.03.1 D0_10x100_hiDiv_q2_100_24.12.0 D0_10x100_hiDiv_q2_1_24.12.0 DIS_10x100_hiDiv_q2_100_24.12.0 DIS_10x100_hiDiv_q2_1_24.12.0)
    collname="ep"

    pwd=$PWD
    for config in "${configs[@]}"; do
	echo "[i] Running config = $config"
	
	odir=/gpfs/mnt/gpfs02/eic/rongrong/D0/analysis/$config
	logdir=$odir/log
	if [ ! -d $odir ]; then
	    mkdir -pv $odir
	fi
	rm -rf $odir/*
	mkdir $logdir


	executable=job_run.sh
	cp -v ${executable} $odir/.
	cp -v analysis $odir/.
	cp -v analysis.cxx $odir/.
	
	echo $odir/$executable
	#Initialising Condor File
	condor_file=CondorFile_$config
	echo "" > ${condor_file}
	echo "Universe    = vanilla" >> ${condor_file}
	echo "Executable  = ${odir}/${executable}" >> ${condor_file}
	echo "GetEnv  =  True" >> ${condor_file}
	#echo '+SingularityImage="/cvmfs/singularity.opensciencegrid.org/eicweb/jug_xl:nightly"' >> ${condor_file}

	files=`ls /eic/u/rongrong/epic/D0/helix/input_files/$config/subList*`
	for file in $files; do
	    listNum=`basename ${file} | sed "s/.list//g" | cut -f 2 -d _`
	    LogFile=${logdir}/log_${listNum}.out
	    ErrFile=${logdir}/log_${listNum}.err
	    OutFile=${odir}/output_${listNum}.root
		
	    echo "" >> ${condor_file}
	    echo "Output    = ${LogFile}" >> ${condor_file}
	    echo "Error     = ${ErrFile}" >> ${condor_file}
	    echo "Arguments = ${odir} ${file} ${OutFile} ${collname}" >> ${condor_file}
	    echo "Queue" >> ${condor_file}
	done
	mv ${condor_file} $odir/.
	cd $odir
	condor_submit ${condor_file}
	cd $pwd
    done
fi

