/* Practical 6 : FIFO full duplex communication - PROCESS 1 (sender / displayer) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FIFO1 "/tmp/fifo_one"      /* Process 1 -> Process 2 */
#define FIFO2 "/tmp/fifo_two"      /* Process 2 -> Process 1 */

int main(void) {
    char text[4096] = "", line[512], buf[1024];
    int fd, n;

    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    printf("===== PROCESS 1 =====\n");
    printf("Enter sentences (press Enter on an empty line to stop):\n");
    while (fgets(line, sizeof line, stdin) && strcmp(line, "\n") != 0)
        strcat(text, line);

    fd = open(FIFO1, O_WRONLY);
    write(fd, text, strlen(text));
    close(fd);

    printf("Process 1: sentences sent through FIFO1.\n");
    fd = open(FIFO2, O_RDONLY);
    printf("\nProcess 1: result received from Process 2 through FIFO2:\n");
    printf("--------------------------------------------\n");
    while ((n = read(fd, buf, sizeof buf - 1)) > 0) {
        buf[n] = '\0';
        printf("%s", buf);
    }
    printf("--------------------------------------------\n");
    close(fd);
    return 0;
}
