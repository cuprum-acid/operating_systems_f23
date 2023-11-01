#!/bin/bash

#set -x

if [[ $EUID -ne 0 ]]; then
    echo "Run in sudo"
    exit 1
fi


PID=$(cat /tmp/ex1.pid)

# Check if the process is running
if ! ps -p $PID > /dev/null; then
    echo "ex1.c is not running."
    exit 1
fi

ADDRESS=$(sudo pmap -x $PID | grep "zero" | awk '{print $1}')

PASSWORD=$(sudo gdb -p $PID -batch -q -ex "x/s 0x$ADDRESS" -ex "quit"  | grep -o "pass:.*")

if [ -z "$PASSWORD" ]; then
    echo "Password not found in memory."
else
    echo "$PASSWORD" | sed 's/.$//'
    echo "Address: 0x$ADDRESS"
fi

kill -9 $PID

