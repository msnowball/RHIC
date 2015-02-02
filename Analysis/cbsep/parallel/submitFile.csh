#!/bin/tcsh

set NAME = ${1}
set SUBMIT_DIR = ${2}
set LIST = ${3}
set OUTPUT_DIR = ${SUBMIT_DIR}/output
set LOG_DIR = ${SUBMIT_DIR}/log
set ERR_DIR = ${SUBMIT_DIR}/err
set LOG_FILE = ${NAME}_log.out
set OUT_FILE = ${NAME}_hists.root

source $HOME/.cshrc
#source /opt/phenix/bin/phenix_setup.csh
cd $HOME/disks/hhj/local/root-32bit/INSTALL/bin
source thisroot.csh
cd -
# set tempdir = $_CONDOR_SCRATCH_DIR/${NAME}
set tempdir = tmp/${NAME}
mkdir -p ${tempdir}

cp ${SUBMIT_DIR}/*.py ${tempdir}
cp ${LIST} ${tempdir}
set NEWLIST = `basename ${LIST}`
bash copyFromDcache.sh $NEWLIST ${tempdir} |& tee -a $LOG_FILE

cd ${tempdir}

echo "-------------------------------------" |& tee -a $LOG_FILE
echo "Job started at `date` on $HOST" |&tee -a $LOG_FILE
echo "-------------------------------------" |&tee -a $LOG_FILE

#$MYPYTHON makeAna.py -i ${NEWLIST} -o $OUT_FILE |&tee -a $LOG_FILE
python makeAna.py -i ${NEWLIST} -o $OUT_FILE |&tee -a $LOG_FILE

ls -lh |&tee -a $LOG_FILE

echo "-------------------------------------" |&tee -a $LOG_FILE
echo "Job ended at `date` on $HOST" |&tee -a $LOG_FILE
echo "-------------------------------------" |&tee -a $LOG_FILE

cp $OUT_FILE $OUTPUT_DIR/
cp $LOG_FILE $LOG_DIR/


exit 0;
