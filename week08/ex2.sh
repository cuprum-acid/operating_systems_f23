#!/bin/bash

mkdir -p /tmp/ex2

# Compile the pager.c and mmu.c programs
gcc pager.c -o pager
gcc mmu.c -o mmu

# Test case 1
echo "Test case 1"
./pager 4 2 &
PID_PAGER=$!
./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $PID_PAGER pager
wait $PID_PAGER

# Test case 2
echo "Test case 2"
./pager 5 3 &
PID_PAGER=$!
./mmu 5 R0 R1 R0 W1 R0 R1 R0 W1 R0 R2 R0 W2 R0 R2 R0 W2 R0 R3 R0 W3 R0 R3 R0 W3 R0 R4 R0 W4 R0 R4 R0 W4 $PID_PAGER pager
wait $PID_PAGER

# Clean up
rm pager mmu /tmp/ex2/pagetable

echo "All test cases completed."

