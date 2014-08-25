#!/bin/bash

NAME=${1}
SUBMIT_DIR=${2}
OUTPUT_DIR=${3}
LOG_DIR=${4}
ERR_DIR=${5}
LIST=${6}

#source ${HOME}/.cshrc
tempdir=$_CONDOR_SCRATCH_DIR/${NAME}
#tempdir=tmp/
mkdir -p ${tempdir}

cp ${SUBMIT_DIR}/Analysis*.py ${tempdir}
cp ${LIST} ${tempdir}

cd ${tempdir}

echo $HOME

source /phenix/u/snowball/disks/hhj/local/root/INSTALL/bin/thisroot.sh
export PYTHONPATH=/direct/phenix+hhj/snowball/local/Python-2.7.8/INSTALL/bin:$PYTHONPATH
export mypython='/direct/phenix+hhj/snowball/local/Python-2.7.8/INSTALL/bin/python'

echo $ROOTSYS 
echo $PYTHONPATH
which python

echo "-------------------------------------"
echo "Job started at `date` on $HOST"
echo "-------------------------------------"

$mypython Analysis.py -f ${LIST} -n 0 -N ${NAME} --time >& ${NAME}_log.out

ls -lh

cp ${NAME}.root $OUTPUT_DIR/
cp ${NAME}_log.out $LOG_DIR/

cd -

rm -r ${tempdir}

echo "-------------------------------------"
echo "Job ended at `date` on $HOST"
echo "-------------------------------------"

exit 0;
