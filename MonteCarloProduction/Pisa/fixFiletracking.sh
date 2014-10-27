#!/bin/bash



cat tmp.txt | while read line
do

    #echo $line
    NAME=$(echo $line | awk '{print $3}')
    PYTHIA=$(echo $line | awk '{print $4}')
    VERTEX=$(echo $line | awk '{print $5}')
    OUTPUT=$(echo $line | awk '{print $7}')

    #echo "${VERTEX} ${PYTHIA} ${OUTPUT}/${NAME}.root"
    echo "${VERTEX} ${PYTHIA} ${OUTPUT}/${NAME}.root" > ${OUTPUT}/${NAME}.root_filetracking.txt

done