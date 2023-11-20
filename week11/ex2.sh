#!/bin/bash
 
# Compile create_fs.c
gcc create_fs.c -w -o create_fs
# Compile ex2.c 
gcc ex2.c -w -o ex2 
 
# Create the file system
./create_fs disk0
 
# Run the file system operations using ex2.c and input.txt
./ex2 input.txt
 
