#!/bin/bash

gcc -o ex1 ex1.c -lpthread

./ex1 $1 $2 > ex1.txt

rm ex1

