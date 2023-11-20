#!/bin/bash
 
# Create a file lofs.img not less than 50 MiB using dd command
dd if=/dev/zero of=lofs.img bs=1M count=50
 
# Setup a loop device on the created file using losetup
sudo losetup -fP lofs.img
 
# Create a Loop File System (LOFS) ext4 on the created file
sudo mkfs.ext4 /dev/loop0
 
# Create a new empty directory ./lofsdisk
mkdir lofsdisk
 
# Mount the created filesystem on the mount point ./lofsdisk
sudo mount /dev/loop0 lofsdisk
 
# Create files file1 and file2 with first and last name
echo "Evgeny" > lofsdisk/file1
echo "Bobkunov" > lofsdisk/file2
 
# Function to get paths of shared libraries for a binary
get_libs() {
    ldd "$1" | awk '{print $3}'
}
 
# Get shared libraries of commands and add them to the LOFS
# bash command
sudo cp $(get_libs $(which bash)) lofsdisk/
# cat command
sudo cp $(get_libs $(which cat)) lofsdisk/
# echo command
sudo cp $(get_libs $(which echo)) lofsdisk/
# ls command
sudo cp $(get_libs $(which ls)) lofsdisk/

# Compile ex1.c and create ex1 executable
gcc ex1.c -o ex1
 
# Change the root directory of the process to the mount point of the created LOFS
sudo chroot lofsdisk /bin/bash -c "./ex1 > ex1.txt"
 
# Run the program again without changing the root directory and append the output to ex1.txt
./ex1 >> ex1.txt
 
