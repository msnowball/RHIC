#!/bin/bash


DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

#echo $DIR

export MAIN=$DIR/../
export OFFLINE_MAIN=$DIR/../build/new.2
export OPT_PHENIX=$DIR/../phenix/stow
export THEROOTSYS=$OFFLINE_MAIN/root
export G4_MAIN=$OFFLINE_MAIN/geant4
export G4INSTALL=$G4_MAIN
export G4SYSTEM=Geant4-10.0.2
export G4_VMC=$OFFLINE_MAIN/geant4_vmc
export LHAPDF=$OPT_PHENIX/lhapdf-5.8.9
export LHAPATH=$LHAPDF/share/lhapdf/PDFsets
export HEPMC=$OPT_PHENIX/HepMC
export PYTHIA8=$OPT_PHENIX/pythia8180
export PHOTOS=$OPT_PHENIX/PHOTOS
export TAUOLA=$OPT_PHENIX/TAUOLA
export EVTGEN=$OPT_PHENIX/EvtGen
export VGM_INSTALL=$OFFLINE_MAIN/vgm-build
export USE_VGM=1
export CLHEP_BASE_DIR=$OPT_PHENIX/CLHEP

#G4 Data
export G4ABLADATA=$G4_MAIN/share/Geant4-10.0.2/data/G4ABLA3.0
export G4LEDATA=$G4_MAIN/share/Geant4-10.0.2/data/G4EMLOW6.35
export G4NEUTRONHPDATA=$G4_MAIN/share/Geant4-10.0.2/data/G4NDL4.4
export G4PIIDATA=$G4_MAIN/share/Geant4-10.0.2/data/G4PII1.3
export G4REALSURFACEDATA=$G4_MAIN/share/Geant4-10.0.2/data/RealSurface1.0
export G4LEVELGAMMADATA=$G4_MAIN/share/Geant4-10.0.2/data/PhotonEvaporation3.0
export G4NEUTRONXSDATA=$G4_MAIN/share/Geant4-10.0.2/data/G4NEUTRONXS1.4
export G4RADIOACTIVEDATA=$G4_MAIN/share/Geant4-10.0.2/data/RadioactiveDecay4.0
export G4SAIDXSDATA=$G4_MAIN/share/Geant4-10.0.2/data/G4SAIDDATA1.1

export LD_LIBRARY_PATH=$G4_MAIN/lib:$LD_LIBRARY_PATH #first time
export LD_LIBRARY_PATH=$LHAPDF/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$EVTGEN/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$PHOTOS/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$TAUOLA/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$PYTHIA8/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$G4_VMC/lib/tgt_linux:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$VGM_INSTALL/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$CLHEP_BASE_DIR/lib:$LD_LIBRARY_PATH

export LD_LIBRARY_PATH=$DIR/../external/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$DIR/../external/usr/lib:$LD_LIBRARY_PATH

export LD_LIBRARY_PATH=$OFFLINE_MAIN/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$OPT_PHENIX/../lib:$LD_LIBRARY_PATH

export PATH=$MAIN/../external/usr/bin:$PATH
export PATH=$OFFLINE_MAIN/bin:$PATH
export PATH=$DIR:$PATH

source $THEROOTSYS/bin/thisroot.sh



export EXTRABIN=/home/snowball/public/PHENIX-SOFTWARE/bin
export PATH=$EXTRABIN:$PATH
alias phnxgridenv="source $EXTRABIN/set_release_env.sh"

export PHSW_GRID_DIR=/home/snowball/public/PHENIX-SOFTWARE/grid
export PEG_DIR=$PHSW_GRID_DIR/pegasus
export CONDOR_DIR=$PHSW_GRID_DIR/condor

