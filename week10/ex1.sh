#!/bin/bash


gcc monitor.c -w -o monitor

gcc ex1.c -o ex1

./monitor /tmp &

./ex1 /tmp

sleep 2

mkdir folder1
echo > f1.txt
echo > f2.txt
ls folder1
cat f1.txt
echo "Hello" >> f2.txt
touch f1.txt
rm f1.txt
chmod u=w folder1
rm -r folder1


pkill -INT monitor

