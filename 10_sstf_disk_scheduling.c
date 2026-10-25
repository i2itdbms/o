/* Program 10: SSTF (Shortest Seek Time First) Disk Scheduling
 * At every step, serve the pending request closest to the current head.
 * Drawback to remember: far-away requests can STARVE.
 * Compile: gcc 10_sstf_disk_scheduling.c -o sstf
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX 100

int main(void)
{
    int req[MAX], done[MAX] = {0}, n, head;
    int total = 0;

    printf("Number of requests (1-%d): ", MAX);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) { printf("Invalid.\n"); return 1; }

    printf("Enter the request queue (track numbers): ");
    for (int i = 0; i < n; i++) scanf("%d", &req[i]);

    printf("Initial head position: ");
    scanf("%d", &head);

    printf("\n%-12s %-12s %-12s\n", "From", "To", "Movement");
    printf("---------------------------------------\n");

    for (int served = 0; served < n; served++) {
        int best = -1, best_dist = 0;

        for (int i = 0; i < n; i++) {
            if (done[i]) continue;
            int d = abs(req[i] - head);
            if (best == -1 || d < best_dist) {   /* first minimum wins ties */
                best = i;
                best_dist = d;
            }
        }
        printf("%-12d %-12d %-12d\n", head, req[best], best_dist);
        total += best_dist;
        head = req[best];
        done[best] = 1;
    }

    printf("---------------------------------------\n");
    printf("Total head movement   : %d cylinders\n", total);
    printf("Average seek length   : %.2f\n", (float)total / n);
    return 0;
}
