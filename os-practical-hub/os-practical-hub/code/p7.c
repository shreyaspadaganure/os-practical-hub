/* Practical 7 : Pipes - full duplex communication between parent and child */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int p1[2], p2[2];                   /* p1: parent -> child, p2: child -> parent */
    char path[256], buf[1024];
    ssize_t n;

    if (pipe(p1) < 0 || pipe(p2) < 0) { perror("pipe"); exit(1); }
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }

    if (pid > 0) {                      /* PARENT */
        close(p1[0]); close(p2[1]);
        printf("PARENT: Enter pathname of the file: ");
        fflush(stdout);
        scanf("%255s", path);
        write(p1[1], path, strlen(path) + 1);
        close(p1[1]);
        printf("PARENT: pathname sent to child through pipe 1.\n");
        printf("PARENT: contents received from child through pipe 2:\n");
        printf("--------------------------------------------\n");
        fflush(stdout);
        while ((n = read(p2[0], buf, sizeof buf)) > 0)
            write(STDOUT_FILENO, buf, n);
        printf("--------------------------------------------\n");
        close(p2[0]);
        wait(NULL);
    } else {                            /* CHILD */
        close(p1[1]); close(p2[0]);
        read(p1[0], path, sizeof path);
        close(p1[0]);
        int fd = open(path, O_RDONLY);
        if (fd < 0) {
            const char *msg = "CHILD: Error - cannot open the file\n";
            write(p2[1], msg, strlen(msg));
        } else {
            while ((n = read(fd, buf, sizeof buf)) > 0)
                write(p2[1], buf, n);
            close(fd);
        }
        close(p2[1]);
        exit(0);
    }
    return 0;
}
