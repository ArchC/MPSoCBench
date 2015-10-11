#!/bin/bash

ARCHC_PREFIX=$1

echo ""

if [ ! -d "$ARCHC_PREFIX" ]; then
    echo -ne "usage:\n"
    echo -ne "\t$0  /path/where/archc/was/installed/\n \n"
    exit 1
fi

if [ ! -f "$ARCHC_PREFIX/etc/env.sh" ]; then
    echo -ne "ERROR: ArchC not found or it's not installed properly.\n\n"
    exit 1
fi

echo -ne "[Install] Copying the Environment file from ArchC to MPSoCBench... "
cp $ARCHC_PREFIX/etc/env.sh .
echo -ne "OK\n\n"

echo -ne "--------------------------------------------------------------------------------------------\n"
echo -ne "The new file called 'env.sh' will set the environment variables to MPSoCBench work properly.\n" 
echo -ne "So, whenever you are in a new terminal (bash), run:\n\n"
echo -ne "\tsource env.sh\n\n"

