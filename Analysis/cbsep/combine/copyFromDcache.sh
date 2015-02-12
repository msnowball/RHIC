#!/bin/bash


function usage()
{
cat << EOF

copyFromDcache.sh <txt file list>

EOF
    
}

if [[ $# < 1 ]]; then usage; exit 1; fi
LIST=$1
if [[ $2 == "" ]]; then
    DIR=./
else
    DIR=$2
fi




function copy()
{
    if [[ $1 == "" ]]; then return 2; fi

    dccp $1 $DIR
    return $?
    
}


function check()
{
    if [[ $1 == "" ]]; then return 2; fi

    env DCACHE_DOOR=phnxdoor1.rcf.bnl.gov:22133 dc_check $1
    return $?
    
}




for i in $(cat $LIST); do
    
    
    check $i
    CODE=$?
    if [[ $CODE == 0 ]]; then
	echo "Copying $i..."
	copy $i
    else
	echo "Check for $i failed, skipping..."
    fi

done

exit 0

