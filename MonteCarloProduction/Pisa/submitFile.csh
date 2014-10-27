#!/bin/csh -f

set NAME = ${1}
set PYTHIA_FILE = ${2}
set VERTEX_FILE = ${3}
set SUBMIT_DIR = ${4}
set OUTPUT_DIR = ${5}
set LOG_DIR = ${6}
set ERR_DIR = ${7}

set THE_LOG = ${NAME}_log.txt

source ${HOME}/.cshrc
set tempdir = $_CONDOR_SCRATCH_DIR/${NAME}
#set tempdir = tmp/${NAME}
mkdir -p ${tempdir}

cp ${SUBMIT_DIR}/* ${tempdir}
cp ${PYTHIA_FILE} ${tempdir}/phpythia.root

cd ${tempdir}

echo "-------------------------------------" |& tee -a ${THE_LOG} 
echo "Job started at `date` on $HOST" |& tee -a ${THE_LOG}
echo "-------------------------------------" |& tee -a ${THE_LOG}

echo "${VERTEX_FILE} ${PYTHIA_FILE} ${OUTPUT_DIR}/${NAME}.root" > ${NAME}.root_filetracking.txt

pisa < pisa.input |& tee -a ${THE_LOG}

ls -lh |& tee -a ${THE_LOG}

echo "-------------------------------------" |& tee -a ${THE_LOG}
echo "Job ended at `date` on $HOST" |& tee -a ${THE_LOG}
echo "-------------------------------------" |& tee -a ${THE_LOG}

cp PISAEvent.root $OUTPUT_DIR/${NAME}.root
cp ${NAME}_log.txt $LOG_DIR/
cp ${NAME}.root_filetracking.txt $OUTPUT_DIR/


exit 0;
