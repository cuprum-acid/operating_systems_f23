#!/bin/bash

file /dev/*
file /etc/*

ls -l /etc | grep ^d |wc -l

gcc -o ex1 ex1.c

#ex1.c: C source, ASCII text
file ex2.c
file ex2

sed -i 's/Hello, world!/Привет, мир!/' ex1.c
#ex1.c: C source, Unicode text, UTF-8 text
file ex1.c

#File type changed because of non-ASCII characters appeared (Russian language is considered to be UTF-8)

rm ex2

