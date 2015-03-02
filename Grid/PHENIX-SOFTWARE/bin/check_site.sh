#!/bin/bash
# Author: Matthew Snowball
# This script checks the OSG site to make sure our software will run

PHSWDIR=/cvmfs/oasis.opensciencegrid.org/osg/projects/phenix/snowball/phnxsw
source $PHSWDIR/bin/set_grid_env.sh

function checkPHSW()
{

    if [[ ! -d $PHSWDIR ]]; then
	EXIT_CODE=99
    else
	EXIT_CODE=0
    fi
    return $EXIT_CODE

}


function checkGlobus()
{
    globus-url-copy -version >& /dev/null
    CODE=$?
    if [[ $CODE != 0 && $CODE != 1 ]]; then
        EXIT_CODE=9
    else
        EXIT_CODE=0
    fi
    return $EXIT_CODE
}


function checkProxy()
{
    voms-proxy-init -version >& /dev/null
    CODE=$?
    if [[ $CODE == 0 ]]; then
	EXIT_CODE=0
    else
        grid-proxy-init -version >& /dev/null
        CODE=$?
        if [[ $CODE == 0 ]]; then
            EXIT_CODE=0
        fi
    fi
    
    return $EXIT_CODE
}

function main()
{

    checkPHSW
    PHSW_CODE=$?
    
    checkGlobus
    GLOBUS_CODE=$?
    
    checkProxy
    PROXY_CODE=$?

    if [[ $PROXY_CODE == 0 && $GLOBUS_CODE == 0  && $PHSW_CODE == 0 ]]; then
	EXIT_CODE=0
    else
	EXIT_CODE=99
    fi

    return $EXIT_CODE

}

main
EXIT_CODE=$?
exit $EXIT_CODE

