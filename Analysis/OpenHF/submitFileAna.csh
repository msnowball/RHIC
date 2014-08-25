#!/bin/csh -f

set NAME = ${1}
set SUBMIT_DIR = ${2}
set OUTPUT_DIR = ${3}
set LOG_DIR = ${4}
set ERR_DIR = ${5}
set LIST = ${6}

source ${HOME}/.cshrc
set tempdir = $_CONDOR_SCRATCH_DIR/${NAME}
#set tempdir = tmp/
mkdir -p ${tempdir}

cp ${SUBMIT_DIR}/Analysis*.py ${tempdir}
cp ${LIST} ${tempdir}

cd ${tempdir}

source $HOME/disks/hhj/local/root/INSTALL/bin/thisroot.csh
setenv PYTHONPATH /direct/phenix+hhj/snowball/local/Python-2.7.8/INSTALL/bin:$PYTHONPATH
alias python '/direct/phenix+hhj/snowball/local/Python-2.7.8/INSTALL/bin/python'

echo "-------------------------------------"
echo "Job started at `date` on $HOST"
echo "-------------------------------------"

python Analysis.py -f ${LIST} -n 0 -N ${NAME} --time >& ${NAME}_log.out

ls -lh

cp ${NAME}.root $OUTPUT_DIR/
cp ${NAME}_log.out $LOG_DIR/

rm -r ${tempdir}

echo "-------------------------------------"
echo "Job ended at `date` on $HOST"
echo "-------------------------------------"

exit 0;
