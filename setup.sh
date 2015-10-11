#!/bin/bash

ARCHC_PREFIX=$1

if [ ! -d "$ARCHC_PREFIX" ]; then
    echo -ne "usage:\n"
    echo -ne "\t$0  /path/where/archc/was/installed/\n\n"
    exit 1
fi

if [ ! -f "$ARCHC_PREFIX/etc/env.sh" ]; then
    echo -ne "ERROR: ArchC not found or it's not installed properly.\n\n"
    exit 1
fi

echo -ne "--------------------------------------------------------------------------------------------\n"
echo -ne "SETUP: Initiating the submodules...\n"
git submodule update --init --recursive

echo -ne "SETUP: Copying the Environment file from ArchC to MPSoCBench... \n"
cp $ARCHC_PREFIX/etc/env.sh .
echo -ne "SETUP: SUCCESSFUL!\n"

echo -ne "--------------------------------------------------------------------------------------------\n"
echo -ne "The new file called 'env.sh' will set the environment variables to MPSoCBench work properly.\n" 
echo -ne "So, whenever you are in a new terminal (bash), run:\n\n"
echo -ne "\tsource env.sh\n\n"
echo -ne "Or put the following command in your ~/.bashrc:\n\n" 
echo -ne "\t source $PWD/env.sh\n\n"
