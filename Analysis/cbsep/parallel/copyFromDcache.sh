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



for i in $(cat $LIST); do
    
    echo "Copying $i..."
    copy $i
    
done

exit 0

