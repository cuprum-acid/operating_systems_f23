#!/bin/bash

gcc ex3.c -o ex3

./ex3 3 &
parent_pid=$!
echo "Running with n = 3 (Parent PID: $parent_pid)"
sleep 1
pstree -p -h -l -u -A $parent_pid
echo "Waiting for 5 seconds..."
sleep 5
pstree -p -h -l -u -A $parent_pid
echo "Waiting for 5 seconds..."
sleep 5
pstree -p -h -l -u -A $parent_pid


./ex3 5 &
parent_pid=$!
echo "Running with n = 5 (Parent PID: $parent_pid)"
pstree -p -h -l -u -A $parent_pid
echo "Waiting for 5 seconds..."
sleep 5
pstree -p -h -l -u -A $parent_pid
echo "Waiting for 5 seconds..."
sleep 5
pstree -p -h -l -u -A $parent_pid
echo "Waiting for 5 seconds..."
sleep 5
pstree -p -h -l -u -A $parent_pid
echo "Waiting for 5 seconds..."
sleep 5
pstree -p -h -l -u -A $parent_pid
pkill ex3
