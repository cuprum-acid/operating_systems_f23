#!/bin/bash

gcc worker.c -o worker
gcc scheduler_sjf.c -o scheduler_sjf

if [ $? -eq 0 ]; then
    echo "Start..."
    ./scheduler_sjf data.txt
else
    echo "Failed... error"
fi

