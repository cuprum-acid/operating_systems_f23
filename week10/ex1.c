#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <libgen.h>
#include <errno.h>
 
void find_all_hlinks(const char *source) {
    struct stat source_stat;
    if (lstat(source, &source_stat) == -1) {
        perror("lstat");
        return;
    }
 
    DIR *dir;
    struct dirent *entry;
 
    if ((dir = opendir(".")) == NULL) {
        perror("opendir");
        return;
    }
 
    printf("Hard links to %s (Inode: %lu):\n", source, (unsigned long)source_stat.st_ino);
 
    while ((entry = readdir(dir)) != NULL) {
        struct stat entry_stat;
        char entry_path[PATH_MAX];
        if (lstat(entry->d_name, &entry_stat) == -1) {
            perror("lstat");
            continue;
        }
 
        if (entry_stat.st_ino == source_stat.st_ino && S_ISREG(entry_stat.st_mode)) {
            if (realpath(entry->d_name, entry_path) != NULL) {
                printf("Inode: %lu, Path: %s\n", (unsigned long)entry_stat.st_ino, entry_path);
            }
        }
    }
 
    closedir(dir);
}
 
void unlink_all(const char *source) {
    struct stat source_stat;
    if (lstat(source, &source_stat) == -1) {
        perror("lstat");
        return;
    }
 
    char last_hard_link[PATH_MAX];
    struct stat last_stat;
 
    DIR *dir;
    struct dirent *entry;
 
    if ((dir = opendir(".")) == NULL) {
        perror("opendir");
        return;
    }
 
    while ((entry = readdir(dir)) != NULL) {
        struct stat entry_stat;
        if (lstat(entry->d_name, &entry_stat) == -1) {
            perror("lstat");
            continue;
        }
 
        if (entry_stat.st_ino == source_stat.st_ino && S_ISREG(entry_stat.st_mode)) {
            strncpy(last_hard_link, entry->d_name, sizeof(last_hard_link));
            last_stat = entry_stat;
        }
    }
 
    closedir(dir);
 
    if (unlink(source) == -1) {
        perror("unlink");
        return;
    }
 
    printf("Removed duplicates of hard link %s (Inode: %lu)\n", last_hard_link, (unsigned long)last_stat.st_ino);
}
 
void create_sym_link(const char *source, const char *link) {
    if (symlink(source, link) == -1) {
        perror("symlink");
    }
}
 
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <directory_path>\n", argv[0]);
        return 1;
    }
 
    char *path = argv[1];


    FILE *myfile1 = fopen("myfile1.txt", "w");
    if (myfile1) {
        fprintf(myfile1, "Hello world.");
        fclose(myfile1);
    } else {
        perror("fopen");
        return 1;
    }
 

    link("myfile1.txt", "myfile11.txt");
    link("myfile1.txt", "myfile12.txt");
 

    find_all_hlinks("myfile1.txt");
 

    if (rename("myfile1.txt", "/tmp/myfile1.txt") != 0) {
        perror("rename");
        return 1;
    }
 
  
    FILE *myfile11 = fopen("myfile11.txt", "a");
    if (myfile11) {
        fprintf(myfile11, " Modified.");
        fclose(myfile11);
    } else {
        perror("fopen");
        return 1;
    }
 

    create_sym_link("/tmp/myfile1.txt", "myfile13.txt");
 

    FILE *tmpmyfile1 = fopen("/tmp/myfile1.txt", "a");
    if (tmpmyfile1) {
        fprintf(tmpmyfile1, " Modified in /tmp.");
        fclose(tmpmyfile1);
    } else {
        perror("fopen");
        return 1;
    }
 
    unlink_all("myfile11.txt");
 
    return 0;
}

