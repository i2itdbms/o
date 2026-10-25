/* Program 2: fork() and wait() -- parent and child sort the same integers
 * Parent uses Bubble Sort, child uses Insertion Sort.
 * After fork(), parent and child have SEPARATE copies of the array.
 * Compile: gcc 02_fork_wait_sort.c -o fork_sort
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX 100

static void print_array(const char *who, int a[], int n)
{
    printf("%s: ", who);
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
    fflush(stdout);
}

static void bubble_sort(int a[], int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (a[j] > a[j + 1]) {
                int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t;
            }
}

static void insertion_sort(int a[], int n)
{
    for (int i = 1; i < n; i++) {
        int key = a[i], j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

int main(void)
{
    int a[MAX], n;

    printf("Enter number of integers (1-%d): ", MAX);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid count.\n");
        return 1;
    }
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++)
        if (scanf("%d", &a[i]) != 1) {
            printf("Invalid input.\n");
            return 1;
        }

    fflush(stdout);                 /* avoid duplicated buffered output after fork */
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {                 /* ---------- CHILD ---------- */
        printf("\n[Child  PID=%d, Parent PID=%d] Insertion sort\n", getpid(), getppid());
        insertion_sort(a, n);
        print_array("Child  sorted array", a, n);
        exit(0);                    /* exit status 0 is collected by parent's wait() */
    }

    /* ---------- PARENT ---------- */
    int status;
    /* Parent waits first so that the output is not mixed. If you want to see
     * real parallel execution, sort first and call wait() afterwards. */
    pid_t done = wait(&status);
    printf("\n[Parent PID=%d] Child %d finished (exit status %d)\n",
           getpid(), done, WEXITSTATUS(status));
    printf("[Parent] Bubble sort\n");
    bubble_sort(a, n);
    print_array("Parent sorted array", a, n);
    return 0;
}
