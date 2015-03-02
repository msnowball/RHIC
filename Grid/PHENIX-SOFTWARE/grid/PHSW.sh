#!/bin/bash

#Setup the environment
source /cvmfs/oasis.opensciencegrid.org/osg/projects/phenix/snowball/phnxsw/bin/set_grid_env.sh

echo "-------------------------------------"
echo "Submission command:"
echo "PHSW.sh $@"
echo "-------------------------------------"

icounter=0
while [ $# -gt 0 ]; do

    if [[ $1 == "-cert" ]]; then CERT=$2; shift; shift; fi
    if [[ $1 == "-name" ]]; then NAME=$2; shift; shift; fi
    if [[ $1 == "-output" ]]; then OUTPUT_NAME=$2; shift; shift; fi
    if [[ $1 == "-list" ]]; then LIST=$2; shift; shift; fi
    if [[ $1 == "-job" ]]; then NJOB=$2; shift; shift; fi
    
    if [[ $icounter > 10 ]]; then break; fi

    let icounter=$icounter+1

done


function earlyexit()
{
    echo 
    echo "#################################################"
    echo "ERROR - Exiting script early: EXIT_CODE = $1"
    echo "#################################################"
    echo
    exit $1
}


function checkCode() 
{
    if [[ $1 == "" ]]; then return 0; fi
    
    CODE=$1
    if [[ $CODE != 0 ]]; then earlyexit $CODE; fi
}



function checkGlobus()
{

    echo "-------------------------------------"   
    echo "checkGlobus()"
    echo "-------------------------------------"

    which globus-url-copy
    globus-url-copy -version
    CODE=$?
    if [[ $CODE != 0 && $CODE != 1 ]]; then
        echo "ERROR - globus-url-copy check failed. Exiting..."
	EXIT_CODE=9
        earlyexit $EXIT_CODE
    else
	EXIT_CODE=0
    fi   
    return $EXIT_CODE
}


function beginjob()
{
    echo "-------------------------------------"
    echo "Job started at `date` on `hostname` at site $OSG_SITE_NAME"
    echo "-------------------------------------" 
    echo 
    echo
    echo "-------------------------------------"   
    echo "Files in directory:"
    echo "-------------------------------------"
    ls -lh
    echo
    echo
    echo "-------------------------------------"
    echo "Environment variables:"
    echo "-------------------------------------"
    printenv
    echo "-------------------------------------"
    echo "LD_LIBRARY_PATH="$LD_LIBRARY_PATH
    echo "-------------------------------------"
    echo "PATH="$PATH
    echo "-------------------------------------"
    echo 
   
    if [[ -z "$GLOBUS_LOCATION" ]]; then 
	echo "WARNING! --- GLOBUS_LOCATION is not set"
    else
	export LD_LIBRARY_PATH=$GLOBUS_LOCATION/lib:$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH=$GLOBUS_LOCATION/lib64:$LD_LIBRARY_PATH
    fi
    ATLASDIR=/atlasgrid/osg-wn-client/usr
    if [[ -d $ATLASDIR ]]; then 
	export LD_LIBRARY_PATH=$ATLASDIR/lib:$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH=$ATLASDIR/lib64:$LD_LIBRARY_PATH
    fi
    export LD_LIBRARY_PATH=/usr/lib:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=/usr/lib64:$LD_LIBRARY_PATH

    #Check for globus-url-copy
    checkGlobus

}

function endjob()
{
    echo "-------------------------------------"
    echo "Job ended at `date` on `hostname`"
    echo "-------------------------------------" 
    echo "Files in directory:"
    ls -lh

    rm -rf globus
    rm -r bin lib src include data

    echo
    echo "#################################################"
    echo "SUCCESS! - Exiting script with EXIT_CODE = $1"
    echo "#################################################"
    echo

}

function getRandSeed() 
{
    den=(0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z a b c d e f g h i j k l m n o p q r s t u v w x y z)
    nd=${#den[*]}
    randj=${den[$RANDOM % $nd]}${den[$RANDOM % $nd]}${den[$RANDOM % $nd]}${den[$RANDOM % $nd]}
    echo $randj
}




#Unpack any tars
function unpack()
{
    echo "-------------------------------------"   
    echo "unpack()"
    echo "-------------------------------------"


    for i in bin lib src include data; do
    	if [ -f $i.tar.gz ]
	then 
	    echo "Unpacking $i..."
	    tar xzf $i.tar.gz 
	    rm $i.tar.gz
            echo "Done"
	fi
    done

}

#Grid Cert for transfers
function cert()
{

    echo "-------------------------------------"   
    echo "cert()"
    echo "-------------------------------------"
    if [[ $1 == ""  ]]; then
        echo "cert() - No certificate present..."
        EXIT_CODE=12
        earlyexit $EXIT_CODE
    fi
    CERT=$1
    if [[ ! -f $CERT ]]; then
        echo "cert() - Certificate does not exist!"
        EXIT_CODE=13
        earlyexit $EXIT_CODE
    fi


    which voms-proxy-init
    voms-proxy-init -version
    CODE=$?
    if [[ $CODE == 0 ]]; then 
	cmd1="voms-proxy-init -hours 24"
	cmd2="voms-proxy-info -all"
    else
	grid-proxy-init -version
	CODE=$?
	if [[ $CODE == 0 ]]; then
	    cmd1="grid-proxy-init -hours 24"
	    cmd2="grid-proxy-info -all"
	else
	    echo "ERROR - voms-proxy-init and grid-proxy-init both failed. Exiting..."
	    EXIT_CODE=11
	    earlyexit $EXIT_CODE
	fi
    fi
	
    mkdir -p globus
    mv $CERT globus/
    cmd3="$cmd1 -cert globus/$CERT -key globus/$CERT"
    echo $cmd3
    eval $cmd3
    EXIT_CODE=$?
    if [[ $EXIT_CODE != 0 ]]; then
	EXIT_CODE=14
	earlyexit $EXIT_CODE
    fi
    eval $cmd2

}


function transfer()
{

    DIRECTION=""
    LOCAL=""
    REMOTE=""
    SERVER="phnxgftp.rcf.bnl.gov"
    OPT="-v -cd -rst -rst-retries 3 -rst-interval 60"
    COMMAND="globus"

    counter=0
    while [ $# -gt 0 ]; do
        if [[ $1 == "-to" ]]; then DIRECTION="TO"; shift; fi
	if [[ $1 == "-from" ]]; then DIRECTION="FROM"; shift; fi
        if [[ $1 == "-cmd" ]]; then COMMAND=$2; shift; shift; fi
        if [[ $1 == "-server" ]]; then SERVER=$2; shift; shift; fi
        if [[ $1 == "-local" ]]; then LOCAL=$2; shift; shift; fi
        if [[ $1 == "-remote" ]]; then REMOTE=$2; shift; shift; fi

        if [[ $counter > 6 ]]; then break; fi
        let counter=$counter+1
    done


    echo "-------------------------------------"   
    echo "transfer() $@"
    echo "-------------------------------------"   
 
    if [[ $COMMAND == "globus" ]]; then
	if [[ $DIRECTION == "FROM" ]]; then
	    cmd="globus-url-copy $OPT gsiftp://$SERVER:2811$REMOTE file://$PWD/$LOCAL"
	fi
	if [[ $DIRECTION == "TO" ]]; then
	    cmd="globus-url-copy $OPT file://$PWD/$LOCAL gsiftp://$SERVER:2811$REMOTE"
	fi
    else 
	echo "transfer() - these options are not implemented yet!"
	EXIT_CODE=21
	earlyexit $EXIT_CODE
    fi

    echo $cmd
    eval $cmd
    EXIT_CODE=$?
    #Fix globus-url-copy issue, exits with 1 sometimes
    if [[ $EXIT_CODE == 1 ]]; then EXIT_CODE=0; fi

    return $EXIT_CODE

}



#Main function
function main()
{

    echo "-------------------------------------"   
    echo "main()"
    echo "-------------------------------------"
    

    EXIT_CODE=0
    ############INSERT RUN COMMANDS HERE############





    ################################################
    return $EXIT_CODE

}

#SEQUENCE
beginjob
export NRand=`getRandSeed`
#DEFAULT EXIT_CODE
EXIT_CODE=0

unpack
cert $CERT


main
EXIT_CODE=$?

endjob $EXIT_CODE
exit $EXIT_CODE

