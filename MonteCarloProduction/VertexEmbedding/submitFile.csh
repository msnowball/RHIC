#!/bin/csh -f

set NEVENTS = ${1}
set NSKIP = ${2}
set NAME = ${3}
set SUBMIT_DIR = ${4}
set OUTPUT_DIR = ${5}
set LOG_DIR = ${6}
set ERR_DIR = ${7}
set INPUT_FILE = ${8}
set RUN_NUMBER = ${9}

source ${HOME}/.cshrc
set tempdir = $_CONDOR_SCRATCH_DIR
#set tempdir = tmp/
mkdir -p ${tempdir}/${NAME}

cp ${SUBMIT_DIR}/*.C ${tempdir}
cp ${SUBMIT_DIR}/*.cfg ${tempdir}

cd ${tempdir}

echo "-------------------------------------"
echo "Job started at `date` on $HOST"
echo "-------------------------------------"

root -b -q Fun4FVTX_for_Embedding.C\(${NEVENTS},${NSKIP},${RUN_NUMBER},\"${INPUT_FILE}\",\"${NAME}_dst.root\",\"${NAME}_event.txt\"\) >& ${NAME}_log.out

ls -l

cp ${NAME}_dst.root /direct/phenix+hhj/snowball/ #$OUTPUT_DIR/
cp ${NAME}_event.txt $OUTPUT_DIR/
cp ${NAME}_log.out $LOG_DIR/

echo "-------------------------------------"
echo "Job ended at `date` on $HOST"
echo "-------------------------------------"

exit 0;
