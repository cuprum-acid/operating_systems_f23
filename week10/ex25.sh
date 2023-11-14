#!/bin/bash

touch ex5.txt

chmod a-w ex5.txt

chmod u=rwx,go=rwx ex5.txt

chmod g=u ex5.txt

# 660 means read and write permissions for the owner, and read permissions for the group.

chmod 660 ex5.txt

# 775 means read, write, and execute permissions for the owner and the group, and read and execute permissions for others.

chmod 775 ex5.txt

# 777 means read, write and execute permissions for everyone (owner, group, and others).

chmod 777 ex5.txt

