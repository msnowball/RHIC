#!/bin/csh -f

set NEVENTS = ${1}
set NAME = ${2}
set SUBMIT_DIR = ${3}
set OUTPUT_DIR = ${4}
set LOG_DIR = ${5}
set ERR_DIR = ${6}

source ${HOME}/.cshrc
set tempdir = $_CONDOR_SCRATCH_DIR
mkdir -p ${tempdir}/${NAME}

cp ${SUBMIT_DIR}/*.C ${tempdir}
cp ${SUBMIT_DIR}/*.cfg ${tempdir}

cd ${tempdir}

echo "-------------------------------------"
echo "Job started at `date` on $HOST"
echo "-------------------------------------"

root -b -q phpythia_OpenHF.C\(${NEVENTS},\"${NAME}.root\",\"${NAME}.txt\"\);

ls -l

cp ${NAME}.root $OUTPUT_DIR/
cp ${NAME}.txt $OUTPUT_DIR/

echo "-------------------------------------"
echo "Job ended at `date` on $HOST"
echo "-------------------------------------"

exit 0;
