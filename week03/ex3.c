#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 100
#define MAX_PATH_LENGTH 2048
#define MAX_DIR 1024

struct File {
    int id;
    char name[MAX_FILES];
    int size;
    char *data;
    struct Directory *directory;
};

struct Directory {
    char name[MAX_FILES];
    struct File **files;
    struct Directory **directories;
    unsigned char nf;  // Number of files in the directory
    unsigned char nd;  // Number of subdirectories in the current directory
    char path[MAX_PATH_LENGTH];  // Absolute path of this directory
    struct Directory *parent;
    struct Directory *sub_dirs[MAX_DIR];
};

void overwrite_to_file(struct File *file, const char *str) {
    if (file->data) {
        free(file->data);
    }
    file->data = strdup(str);
    file->size = strlen(file->data);
}

void append_to_file(struct File *file, const char *str) {
    if (file->data) {
        size_t old_size = file->size;
        size_t str_len = strlen(str);
        file->data = (char *) realloc(file->data, old_size + str_len + 1);
        strcat(file->data, str);
        file->size = old_size + str_len;
    } else {
        file->data = strdup(str);
        file->size = strlen(file->data);
    }
}

void printp_file(struct File *file) {
    printf("%s/%s \n", file->directory->path, file->name);
}

void add_file(struct File *file, struct Directory *dir) {
    dir->nf++;
    dir->files = (struct File **) realloc(dir->files, dir->nf * sizeof(struct File *));
    dir->files[dir->nf - 1] = file;
    file->directory = dir;
}

void add_dir(struct Directory *dir1, struct Directory *dir2) {
    if (dir1 && dir2) {
        dir2->sub_dirs[dir2->nd] = dir1;
        dir2->nd++;
        char temp_path[MAX_PATH_LENGTH];
        if (strcmp(dir2->path, "/")) {
            strcpy(temp_path, dir2->path);
            strcat(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        } else {
            strcpy(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        }
    }
}


int main() {

    struct Directory root = {"", {NULL}, {NULL}, 0, 0, "/", NULL};

    struct Directory home = {"home", {NULL}, {NULL}, 0, 0, "/home", &root};
    struct Directory bin = {"bin", {NULL}, {NULL}, 0, 0, "/bin", &root};

    add_dir(&home, &root);
    add_dir(&bin, &root);

    struct File bash = {1, "bash", 0, NULL, &bin};
    add_file(&bash, &bin);

    struct File ex3_1 = {2, "ex3_1.c", 0, NULL, &home};
    struct File ex3_2 = {3, "ex3_2.c", 0, NULL, &home};
    add_file(&ex3_1, &home);
    add_file(&ex3_2, &home);

    overwrite_to_file(&ex3_1, "int printf(const char * format, ...);");
    overwrite_to_file(&ex3_2, "//This is a comment in C language");

    overwrite_to_file(&bash, "Bourne Again Shell!!");

    append_to_file(&ex3_1, "int main(){printf(\"Hello World!\");}");

    printp_file(&bash);
    printp_file(&ex3_1);
    printp_file(&ex3_2);

    return 0;
}
