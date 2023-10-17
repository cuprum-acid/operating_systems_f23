#!/bin/bash

gcc worker.c -o worker
gcc scheduler_rr.c -o scheduler_rr

if [ $? -eq 0 ]; then
    echo "Start..."
    ./scheduler_rr data.txt
else
    echo "Failed... error"
fi

