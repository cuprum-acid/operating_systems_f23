#!/bin/bash

gcc ex3.c -o ex3 -pthread -lm

n=10000000
m_values=(1 2 4 10 100)

echo "n = $n" > ex3_res.txt
for m in "${m_values[@]}"; do
    echo "m = $m" >> ex3_res.txt
    { time ./ex3 $n $m; } 2>> ex3_res.txt
    echo "" >> ex3_res.txt
done

