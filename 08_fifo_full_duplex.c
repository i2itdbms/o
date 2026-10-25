/* Program 8: Full-duplex communication between two INDEPENDENT processes
 *            using two FIFOs (named pipes).
 *
 *  FIFO1 (/tmp/os_fifo1): Process 1 --> Process 2   (sentences)
 *  FIFO2 (/tmp/os_fifo2): Process 2 --> Process 1   (result file contents)
 *
 *  Process 1 : reads sentences from keyboard (type END on a line to finish),
 *              sends them, then displays what Process 2 sends back.
 *  Process 2 : counts characters, words, lines; saves them in result.txt;
 *              sends the file contents back.
 *
 * Compile: gcc 08_fifo_full_duplex.c -o fifo_duplex
 * Run in TWO terminals:
 *      Terminal A: ./fifo_duplex 2      (receiver / counter)
 *      Terminal B: ./fifo_duplex 1      (sender / display)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FIFO1    "/tmp/os_fifo1"
#define FIFO2    "/tmp/os_fifo2"
#define RESULT   "result.txt"
#define BUF_SIZE 512

static void make_fifos(void)
{
    if (mkfifo(FIFO1, 0666) == -1 && errno != EEXIST) { perror("mkfifo FIFO1"); exit(1); }
    if (mkfifo(FIFO2, 0666) == -1 && errno != EEXIST) { perror("mkfifo FIFO2"); exit(1); }
}

/* ------------------------- PROCESS 1 ------------------------- */
static void process1(void)
{
    char line[BUF_SIZE], buf[BUF_SIZE];
    ssize_t n;

    make_fifos();

    /* open() blocks until the other side opens the FIFO too */
    int wfd = open(FIFO1, O_WRONLY);
    if (wfd < 0) { perror("open FIFO1"); exit(1); }

    printf("[P1] Enter sentences. Type END on a new line to finish.\n");
    while (fgets(line, sizeof line, stdin) != NULL) {
        if (strcmp(line, "END\n") == 0 || strcmp(line, "END") == 0)
            break;
        write(wfd, line, strlen(line));
    }
    close(wfd);                         /* closing = EOF for Process 2 */

    int rfd = open(FIFO2, O_RDONLY);
    if (rfd < 0) { perror("open FIFO2"); exit(1); }

    printf("\n[P1] Result received from Process 2:\n");
    printf("-------------------------------------\n");
    fflush(stdout);                     /* printf is buffered, write() is not */
    while ((n = read(rfd, buf, sizeof buf)) > 0)
        write(STDOUT_FILENO, buf, (size_t)n);
    printf("-------------------------------------\n");
    close(rfd);
}

/* ------------------------- PROCESS 2 ------------------------- */
static void process2(void)
{
    char buf[BUF_SIZE];
    ssize_t n;
    long chars = 0, words = 0, lines = 0;
    int in_word = 0;

    make_fifos();

    int rfd = open(FIFO1, O_RDONLY);
    if (rfd < 0) { perror("open FIFO1"); exit(1); }

    printf("[P2] Waiting for sentences...\n");
    while ((n = read(rfd, buf, sizeof buf)) > 0) {
        for (ssize_t i = 0; i < n; i++) {
            chars++;
            if (buf[i] == '\n') lines++;
            if (isspace((unsigned char)buf[i])) {
                in_word = 0;
            } else if (!in_word) {      /* start of a new word */
                in_word = 1;
                words++;
            }
        }
    }
    close(rfd);

    /* store results in a text file */
    FILE *fp = fopen(RESULT, "w");
    if (fp == NULL) { perror("fopen"); exit(1); }
    fprintf(fp, "Characters : %ld\nWords      : %ld\nLines      : %ld\n", chars, words, lines);
    fclose(fp);
    printf("[P2] Counts saved in %s. Sending contents back...\n", RESULT);

    /* send file contents back through FIFO2 */
    int wfd = open(FIFO2, O_WRONLY);
    if (wfd < 0) { perror("open FIFO2"); exit(1); }
    int ffd = open(RESULT, O_RDONLY);
    if (ffd < 0) { perror("open result"); exit(1); }
    while ((n = read(ffd, buf, sizeof buf)) > 0)
        write(wfd, buf, (size_t)n);
    close(ffd);
    close(wfd);

    unlink(FIFO1);                      /* cleanup */
    unlink(FIFO2);
    printf("[P2] Done.\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2 || (argv[1][0] != '1' && argv[1][0] != '2')) {
        fprintf(stderr, "Usage: %s 1   (sender)  OR  %s 2   (counter)\n", argv[0], argv[0]);
        return 1;
    }
    if (argv[1][0] == '1') process1();
    else                   process2();
    return 0;
}
