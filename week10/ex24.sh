#!/bin/bash

[ -d ./tmp ] && rm -r ./tmp
[ -e tmp1 ] && rm tmp1
ln -s ./tmp tmp1

ls -li

mkdir ./tmp

ls -li

./gen.sh 1 ./tmp/ex1.txt

ls -l ./tmp1

ln -s $(realpath ./tmp) ./tmp1/tmp2

./gen.sh 1 ./tmp1/tmp2/ex1.txt

ls -R ./tmp1

rm -r ./tmp
ls -R ./tmp1

rm -r tmp1
