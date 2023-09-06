# Create new directories
date
sleep 3
mkdir -p ~/week01/root_folder
date
sleep 3
mkdir -p ~/week01/home_folder

date
sleep 3
find / -maxdepth 1 -type d -exec ls -lt --time=atime {} \; > ~/week01/root_folder/root.txt
date
sleep 3
find ~/ -maxdepth 1 -type d -exec ls -lt --time=atime {} \; > ~/week01/home_folder/home.txt

cat ~/week01/root_folder/root.txt
cat ~/week01/home_folder/home.txt

ls ~/week01/root_folder
ls ~/week01/home_folder
