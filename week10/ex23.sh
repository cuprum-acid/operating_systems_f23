#!/bin/bash

./gen.sh 10 ex1.txt

ln ex1.txt ex11.txt
ln ex1.txt ex12.txt

diff ex1.txt ex11.txt
diff ex1.txt ex12.txt

ls -i ex1.txt ex11.txt ex12.txt > output.txt

du ex1.txt

ln ex1.txt ex13.txt
mv ex13.txt /tmp

find . -inum $(ls -i ex1.txt | awk '{print $1}') -exec ls -l {} \;
find / -inum $(ls -i ex1.txt | awk '{print $1}') -exec ls -l {} \;

# The difference between searching in the current path and the root path (/):
# Searching in the current path only finds hard links within the current directory.
# Searching in the root path finds hard links anywhere on the system.

ls -l ex1.txt

find . -inum $(ls -i ex1.txt | awk '{print $1}') -exec rm {} \;

