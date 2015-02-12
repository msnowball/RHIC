#!/bin/tcsh

set NAME = ${1}
set SUBMIT_DIR = ${2}
set LIST = ${3}
set OUTPUT_DIR = ${SUBMIT_DIR}/output
set LOG_DIR = ${SUBMIT_DIR}/log
set ERR_DIR = ${SUBMIT_DIR}/err
set LOG_FILE = ${NAME}_log.out
set OUT_FILE = ${NAME}_hists.root

set COMBINE_FILE = combine.root

source $HOME/.cshrc
set tempdir = $_CONDOR_SCRATCH_DIR/${NAME}
#set tempdir = tmp/${NAME}
mkdir -p ${tempdir}

cp ${LIST} ${tempdir}
cp copyFromDcache.sh ${tempdir}

cd ${tempdir}
set NEWLIST = `basename ${LIST}`
bash copyFromDcache.sh $NEWLIST |& tee -a $LOG_FILE


echo "-------------------------------------" |& tee -a $LOG_FILE
echo "Job started at `date` on $HOST" |&tee -a $LOG_FILE
echo "-------------------------------------" |&tee -a $LOG_FILE

hadd $COMBINE_FILE *.root |& tee -a $LOG_FILE

ls -lh |&tee -a $LOG_FILE

echo "-------------------------------------" |&tee -a $LOG_FILE
echo "Job ended at `date` on $HOST" |&tee -a $LOG_FILE
echo "-------------------------------------" |&tee -a $LOG_FILE

cp $COMBINE_FILE $OUTPUT_DIR/${NAME}_pdst.root
cp $LOG_FILE $LOG_DIR/


exit 0;
