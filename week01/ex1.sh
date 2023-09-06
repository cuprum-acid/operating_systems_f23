mkdir ~/week01

cd ~/week01

find /usr/bin -type f -name '*gcc*' | sort -r | head -n 5 > ex1.txt
