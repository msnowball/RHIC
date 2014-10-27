#!/bin/csh -f

set NEVENTS = ${1}
set PISA_FILE = ${2}
set NAME = ${3}
set SUBMIT_DIR = ${4}
set OUTPUT_DIR = ${5}
set LOG_DIR = ${6}
set ERR_DIR = ${7}

set THE_LOG = ${NAME}_log.out
set SIGNAL_ONLY_NAME = ${NAME}_sigOnly
set EMBEDDED_NAME = ${NAME}_embedded

source ${HOME}/.cshrc
set tempdir = $_CONDOR_SCRATCH_DIR/${NAME}
#set tempdir = tmp/${NAME}
mkdir -p ${tempdir}

cp ${SUBMIT_DIR}/*.C ${tempdir}

cd ${tempdir}

echo "-------------------------------------" |& tee -a ${THE_LOG}
echo "Job started at `date` on $HOST" |& tee -a ${THE_LOG}
echo "-------------------------------------" |& tee -a ${THE_LOG}


root -b -q Fun4All_SignalOnly.C\(${NEVENTS},\"${PISA_FILE}\",\"\",\"\",\"${SIGNAL_ONLY_NAME}\"\) |& tee -a ${THE_LOG}
root -b -q Fun4All_Embed.C\(${NEVENTS},\"${SIGNAL_ONLY_NAME}_dst.root\",\"\",\"${EMBEDDED_NAME}\"\) |& tee -a ${THE_LOG}

ls -lh |& tee -a ${THE_LOG}

mv ${SIGNAL_ONLY_NAME}_dst.root_filetracking.txt ${SIGNAL_ONLY_NAME}_pdst.root_filetracking.txt


unset OUTPUT_DIR
set OUTPUT_DIR = "/direct/phenix+hhj2/snowball/MonteCarlo/RECO/Run12pp510/KPi_FastSIM/"

mkdir -p $OUTPUT_DIR/Embedded
mkdir -p $OUTPUT_DIR/SignalOnly


echo "-------------------------------------" |& tee -a ${THE_LOG}
echo "Job ended at `date` on $HOST" |& tee -a ${THE_LOG}
echo "-------------------------------------" |& tee -a ${THE_LOG}

cp ${SIGNAL_ONLY_NAME}_pdst.root $OUTPUT_DIR/SignalOnly
cp ${SIGNAL_ONLY_NAME}_pdst.root_filetracking.txt $OUTPUT_DIR/SignalOnly

cp ${EMBEDDED_NAME}_pdst.root $OUTPUT_DIR/Embedded
cp ${EMBEDDED_NAME}_pdst.root_filetracking.txt $OUTPUT_DIR/Embedded

cp ${THE_LOG} $LOG_DIR/

exit 0;
