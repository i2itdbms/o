/* Program 7: Banker's Algorithm (deadlock avoidance)
 *  1. Reads Allocation, Max and Available.
 *  2. Computes Need = Max - Allocation.
 *  3. Safety algorithm -> prints safe sequence (or says UNSAFE).
 *  4. Resource-request algorithm: grants a request only if the state after
 *     granting is still safe.
 * Compile: gcc 07_bankers_algorithm.c -o bankers
 */
#include <stdio.h>
#include <string.h>

#define MAXP 10
#define MAXR 10

static int n, m;
static int alloc[MAXP][MAXR], max[MAXP][MAXR], need[MAXP][MAXR], avail[MAXR];

/* Returns 1 if the current state is safe; fills seq[] with safe sequence. */
static int is_safe(int seq[])
{
    int work[MAXR], finish[MAXP] = {0}, count = 0;

    memcpy(work, avail, sizeof(int) * m);

    while (count < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (finish[i]) continue;
            int ok = 1;
            for (int j = 0; j < m; j++)
                if (need[i][j] > work[j]) { ok = 0; break; }
            if (ok) {
                /* process i can finish and will release its resources */
                for (int j = 0; j < m; j++) work[j] += alloc[i][j];
                finish[i] = 1;
                seq[count++] = i;
                found = 1;
            }
        }
        if (!found) return 0;              /* nobody can proceed -> unsafe */
    }
    return 1;
}

static void print_state(void)
{
    printf("\nProcess  Allocation   Max       Need\n");
    for (int i = 0; i < n; i++) {
        printf("P%-6d ", i);
        for (int j = 0; j < m; j++) printf("%3d", alloc[i][j]);
        printf("     ");
        for (int j = 0; j < m; j++) printf("%3d", max[i][j]);
        printf("     ");
        for (int j = 0; j < m; j++) printf("%3d", need[i][j]);
        printf("\n");
    }
    printf("Available: ");
    for (int j = 0; j < m; j++) printf("%d ", avail[j]);
    printf("\n");
}

static void print_sequence(int seq[])
{
    printf("SAFE sequence: ");
    for (int i = 0; i < n; i++)
        printf("P%d%s", seq[i], i < n - 1 ? " -> " : "\n");
}

int main(void)
{
    int seq[MAXP];

    printf("Number of processes (max %d): ", MAXP);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAXP) { printf("Invalid.\n"); return 1; }
    printf("Number of resource types (max %d): ", MAXR);
    if (scanf("%d", &m) != 1 || m < 1 || m > MAXR) { printf("Invalid.\n"); return 1; }

    printf("Enter Allocation matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) scanf("%d", &alloc[i][j]);

    printf("Enter Max matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) scanf("%d", &max[i][j]);

    printf("Enter Available vector (%d values):\n", m);
    for (int j = 0; j < m; j++) scanf("%d", &avail[j]);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
            if (need[i][j] < 0) {
                printf("Error: Max < Allocation for P%d, resource %d\n", i, j);
                return 1;
            }
        }

    print_state();
    if (is_safe(seq)) print_sequence(seq);
    else { printf("System is in an UNSAFE state.\n"); return 0; }

    /* ---- Resource request handling ---- */
    char ans;
    printf("\nMake a resource request? (y/n): ");
    scanf(" %c", &ans);
    while (ans == 'y' || ans == 'Y') {
        int p, req[MAXR];
        printf("Process number (0-%d): ", n - 1);
        scanf("%d", &p);
        if (p < 0 || p >= n) { printf("Invalid process.\n"); break; }
        printf("Request vector (%d values): ", m);
        for (int j = 0; j < m; j++) scanf("%d", &req[j]);

        int ok = 1;
        for (int j = 0; j < m; j++)
            if (req[j] > need[p][j]) {
                printf("Error: P%d asked for more than its declared maximum claim.\n", p);
                ok = 0; break;
            }
        if (ok)
            for (int j = 0; j < m; j++)
                if (req[j] > avail[j]) {
                    printf("P%d must WAIT: resources not available now.\n", p);
                    ok = 0; break;
                }
        if (ok) {
            /* pretend to allocate */
            for (int j = 0; j < m; j++) {
                avail[j] -= req[j]; alloc[p][j] += req[j]; need[p][j] -= req[j];
            }
            if (is_safe(seq)) {
                printf("Request GRANTED. New state is safe.\n");
                print_sequence(seq);
            } else {
                /* roll back */
                for (int j = 0; j < m; j++) {
                    avail[j] += req[j]; alloc[p][j] -= req[j]; need[p][j] += req[j];
                }
                printf("Request DENIED: granting it would lead to an UNSAFE state.\n");
            }
        }
        printf("\nAnother request? (y/n): ");
        scanf(" %c", &ans);
    }
    return 0;
}
