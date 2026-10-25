/* Program 9: Full-duplex communication between parent and child using TWO pipes
 *
 *  pipe1 : parent --> child   (pathname of the file)
 *  pipe2 : child  --> parent  (contents of the file)
 *
 * Parent writes the pathname, child reads it, opens the file, and sends the
 * contents back; parent prints them on standard output.
 * Rule: every process must close the pipe ends it does not use, otherwise
 * read() never sees EOF and the program hangs.
 *
 * Compile: gcc 09_pipe_parent_child_file.c -o pipe_file
 * Run    : ./pipe_file            (asks for path)
 *          ./pipe_file notes.txt
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PATH_SIZE 512
#define BUF_SIZE  1024

int main(int argc, char *argv[])
{
    int p2c[2], c2p[2];            /* parent->child, child->parent */
    char path[PATH_SIZE];

    if (argc > 1) {
        strncpy(path, argv[1], PATH_SIZE - 1);
        path[PATH_SIZE - 1] = '\0';
    } else {
        printf("Enter file pathname: ");
        if (fgets(path, sizeof path, stdin) == NULL) return 1;
        path[strcspn(path, "\n")] = '\0';
    }

    if (pipe(p2c) == -1 || pipe(c2p) == -1) { perror("pipe"); return 1; }

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {                              /* ------- CHILD ------- */
        char name[PATH_SIZE], buf[BUF_SIZE];
        ssize_t n;

        close(p2c[1]);                           /* child only reads pipe1  */
        close(c2p[0]);                           /* child only writes pipe2 */

        n = read(p2c[0], name, sizeof name - 1); /* get pathname */
        if (n <= 0) exit(1);
        name[n] = '\0';
        close(p2c[0]);

        int fd = open(name, O_RDONLY);
        if (fd < 0) {
            const char *err = "[child] Error: cannot open the file.\n";
            write(c2p[1], err, strlen(err));
        } else {
            while ((n = read(fd, buf, sizeof buf)) > 0)
                write(c2p[1], buf, (size_t)n);
            close(fd);
        }
        close(c2p[1]);                           /* EOF for parent */
        exit(0);
    }

    /* ------------------- PARENT ------------------- */
    char buf[BUF_SIZE];
    ssize_t n;

    close(p2c[0]);                               /* parent only writes pipe1 */
    close(c2p[1]);                               /* parent only reads pipe2  */

    write(p2c[1], path, strlen(path));           /* send pathname */
    close(p2c[1]);

    printf("\n----- Contents of %s -----\n", path);
    fflush(stdout);                              /* printf is buffered, write() is not */
    while ((n = read(c2p[0], buf, sizeof buf)) > 0)
        write(STDOUT_FILENO, buf, (size_t)n);
    printf("\n----- End of file -----\n");
    close(c2p[0]);

    wait(NULL);
    return 0;
}
