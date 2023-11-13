#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/inotify.h>
#include <signal.h>
#include <sys/stat.h>
 
#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))
 
void print_stat_info(const char *path) {
    struct stat file_stat;
    if (stat(path, &file_stat) == 0) {
        printf("Path: %s, Size: %ld bytes, Inode: %lu\n", path, file_stat.st_size, (unsigned long)file_stat.st_ino);
    } else {
        printf("Error getting stat info for %s: %s\n", path, strerror(errno));
    }
}
 
void handle_sigint(int signum) {
    printf("\nTerminating monitor.c\n");
    exit(0);
}
 
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <directory_path>\n", argv[0]);
        return 1;
    }
 
    char *path = argv[1];
    int inotify_fd, watch_fd;
    char buffer[EVENT_BUF_LEN];
 
    inotify_fd = inotify_init();
    if (inotify_fd < 0) {
        perror("inotify_init");
        return 1;
    }
 
    watch_fd = inotify_add_watch(inotify_fd, path, IN_ACCESS | IN_CREATE | IN_DELETE | IN_MODIFY | IN_OPEN | IN_ATTRIB);
    if (watch_fd < 0) {
        perror("inotify_add_watch");
        return 1;
    }
 
    signal(SIGINT, handle_sigint);
 
    printf("Monitoring directory: %s\n", path);
    print_stat_info(path);
 
    while (1) {
        int length = read(inotify_fd, buffer, EVENT_BUF_LEN);
        if (length < 0) {
            perror("read");
            break;
        }
 
        int i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    printf("%s is created\n", event->name);
                }
                if (event->mask & IN_DELETE) {
                    printf("%s is deleted\n", event->name);
                }
                if (event->mask & IN_MODIFY) {
                    printf("%s is modified\n", event->name);
                }
                if (event->mask & IN_ACCESS) {
                    printf("%s is accessed\n", event->name);
                }
                if (event->mask & IN_OPEN) {
                    printf("%s is opened\n", event->name);
                }
                if (event->mask & IN_ATTRIB) {
                    printf("%s attributes are changed\n", event->name);
                }
                char full_path[1024];
                snprintf(full_path, sizeof(full_path), "%s/%s", path, event->name);
                print_stat_info(full_path);
            }
            i += EVENT_SIZE + event->len;
        }
    }
 
    inotify_rm_watch(inotify_fd, watch_fd);
    close(inotify_fd);
    return 0;
}

