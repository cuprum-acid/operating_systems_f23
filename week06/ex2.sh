#!/bin/bash

gcc worker.c -o worker
gcc scheduler.c -o scheduler

if [ $? -eq 0 ]; then
    echo "Start..."
    ./scheduler data.txt
else
    echo "Failed... error"
fi

