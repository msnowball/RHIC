#!/bin/bash


#echo $PWD

export PHSWBASE=`basename $PWD`
export PHSWBASE_FULL=$PWD

export PATH=$PWD/bin:$PATH
export PATH=$PWD/grid/condor:$PATH
export PATH=$PWD/grid/pegasus:$PATH

alias sitenames="condor_status -autoformat GLIDEIN_ResourceName HAS_PHSW | sort | uniq"
