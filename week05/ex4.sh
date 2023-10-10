#!/bin/bash

gcc ex4.c -o ex4 -pthread -lm

n=10000000
m_values=(1 2 4 10 100)

echo "n = $n" > ex4_res.txt
for m in "${m_values[@]}"; do
    echo "m = $m" >> ex4_res.txt
    { time ./ex4 $n $m; } 2>> ex4_res.txt
    echo "" >> ex4_res.txt
done

