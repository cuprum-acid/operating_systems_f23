#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024

int main() {
    char input[MAX_INPUT_SIZE];
    char current_dir[MAX_INPUT_SIZE];

    while (1) {
        printf("%s> ", getcwd(current_dir, sizeof(current_dir)));

        fgets(input, MAX_INPUT_SIZE, stdin);

        input[strlen(input) - 1] = '\0';

        if (strcmp(input, "exit") == 0) {
            printf("Exiting Shell...\n");
            break;
        } else if (strncmp(input, "cd ", 3) == 0) {
        
            char *path = input + 3;

            if (strcmp(path, "..") == 0) {
            
                if (chdir("..") != 0) {
                    perror("chdir");
                }
            } else {
               
                if (chdir(path) != 0) {
                    perror("chdir");
                }
            }

            continue;
        }

        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("fork");
            return 1;
        } else if (child_pid == 0) {
     
            char *args[MAX_INPUT_SIZE / 2];
            int arg_count = 0;

            char *token = strtok(input, " ");
            while (token != NULL) {
                args[arg_count] = token;
                arg_count++;
                token = strtok(NULL, " ");
            }

            args[arg_count] = NULL;

            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(1);
            }
        }
    }

    return 0;
}

