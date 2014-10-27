#!/bin/bash

NAME=${1}
SUBMIT_DIR=${2}
LIST=${3}
OUTPUT_DIR=${SUBMIT_DIR}/output
LOG_DIR=${SUBMIT_DIR}/log
ERR_DIR=${SUBMIT_DIR}/err

source $HOME/disks/hhj/local/root/INSTALL/bin/thisroot.sh
tempdir=$_CONDOR_SCRATCH_DIR/${NAME}
#tempdir=tmp/${NAME}
mkdir -p ${tempdir}

cp ${SUBMIT_DIR}/*.py ${tempdir}
cp ${LIST} ${tempdir}
NEWLIST=`basename ${LIST}`

cd ${tempdir}

echo "-------------------------------------"
echo "Job started at `date` on $HOST"
echo "-------------------------------------"

$MYPYTHON makeAna.py -i ${NEWLIST} -o ${NAME}_hists.root >& ${NAME}_log.out

ls -lh

cp ${NAME}_hists.root $OUTPUT_DIR/
cp ${NAME}_log.out $LOG_DIR/

echo "-------------------------------------"
echo "Job ended at `date` on $HOST"
echo "-------------------------------------"

exit 0;
