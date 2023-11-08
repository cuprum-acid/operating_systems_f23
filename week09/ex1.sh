#!/bin/bash

# Compile the programs
gcc mmu.c -o mmu
gcc pager.c -o pager

# Run the pager process with the specified algorithm
./pager <num_pages> <algorithm> <disk_accesses> &

# Store the PID of the pager process
PAGER_PID=$!

# Run the MMU process with the pager process PID
./mmu <num_pages> <reference_string> $PAGER_PID

# Cleanup
rm -f mmu pager
