/* Program 11: SCAN and C-LOOK Disk Scheduling
 *
 * Disk has tracks 0 .. disk_size-1. The spindle (centre) is at track 0, so
 * "initial movement AWAY from the spindle" = towards HIGHER track numbers.
 *
 * SCAN   : head moves up serving requests, goes all the way to the LAST track
 *          (disk_size-1), reverses, and serves remaining requests downwards.
 * C-LOOK : head moves up serving requests only up to the HIGHEST request
 *          (does not go to the disk end), then JUMPS to the LOWEST request
 *          and again moves upward. (Circular + LOOK)
 *          The return jump is counted in the total head movement
 *          (some books exclude it; both values are printed here).
 *
 * Compile: gcc 11_scan_clook_disk_scheduling.c -o scan_clook
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX 100

static int cmp(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}

static void scan(int req[], int n, int head, int disk_size)
{
    int total = 0, pos = head;

    printf("\n===== SCAN (moving away from spindle first) =====\n");
    printf("Path: %d", head);

    /* 1. upward: requests >= head, in ascending order */
    for (int i = 0; i < n; i++)
        if (req[i] >= head) {
            total += abs(req[i] - pos);
            pos = req[i];
            printf(" -> %d", pos);
        }

    /* 2. go to the last track (end of disk) */
    if (pos != disk_size - 1) {
        total += (disk_size - 1) - pos;
        pos = disk_size - 1;
        printf(" -> %d", pos);
    }

    /* 3. reverse: requests < head, in descending order */
    for (int i = n - 1; i >= 0; i--)
        if (req[i] < head) {
            total += abs(req[i] - pos);
            pos = req[i];
            printf(" -> %d", pos);
        }

    printf("\nTotal head movement (SCAN): %d cylinders\n", total);
}

static void c_look(int req[], int n, int head)
{
    int total = 0, jump = 0, pos = head;
    int has_lower = 0;

    printf("\n===== C-LOOK (moving away from spindle first) =====\n");
    printf("Path: %d", head);

    /* 1. upward: requests >= head, ascending (stop at highest request) */
    for (int i = 0; i < n; i++)
        if (req[i] >= head) {
            total += abs(req[i] - pos);
            pos = req[i];
            printf(" -> %d", pos);
        } else {
            has_lower = 1;
        }

    /* 2. jump to the lowest request, then continue upward through the rest */
    if (has_lower) {
        jump = abs(req[0] - pos);          /* req[0] is the smallest request */
        total += jump;
        pos = req[0];
        printf(" => %d", pos);             /* '=>' marks the return jump */
        for (int i = 1; i < n; i++)
            if (req[i] < head) {
                total += abs(req[i] - pos);
                pos = req[i];
                printf(" -> %d", pos);
            }
    }

    printf("\nTotal head movement (C-LOOK), jump included : %d cylinders\n", total);
    printf("Total head movement (C-LOOK), jump excluded : %d cylinders\n", total - jump);
}

int main(void)
{
    int req[MAX], n, head, disk_size;

    printf("Number of requests (1-%d): ", MAX);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) { printf("Invalid.\n"); return 1; }
    printf("Enter the request queue: ");
    for (int i = 0; i < n; i++) scanf("%d", &req[i]);
    printf("Initial head position: ");
    scanf("%d", &head);
    printf("Total number of tracks (e.g. 200 means tracks 0-199): ");
    scanf("%d", &disk_size);

    for (int i = 0; i < n; i++)
        if (req[i] < 0 || req[i] >= disk_size || head < 0 || head >= disk_size) {
            printf("Error: all track numbers must be in 0..%d\n", disk_size - 1);
            return 1;
        }

    qsort(req, n, sizeof(int), cmp);       /* sorted copy simplifies both algorithms */

    scan(req, n, head, disk_size);
    c_look(req, n, head);
    return 0;
}
