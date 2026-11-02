#include <stdio.h>
int main() {
    int allocation[5][3] = {
        {0, 1, 1},
        {1, 3, 2},
        {0, 1, 0},
        {0, 0, 2},
        {2, 0, 0}
    };

    int available[3] = {3, 3, 2};

    int need[5][3];

    int max[5][3] = {
        {7, 5, 3},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int finish[5] = {0};
    int work[3];
    int safe[5];

    int i, j, k;
    int count = 0;
    int found;

    printf("Need Matrix:\n");

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 3; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }

    for (j = 0; j < 3; j++) {
        work[j] = available[j];
    }

    while (count < 5) {
        found = 0;

        for (i = 0; i < 5; i++) {

            if (finish[i] == 0) {

                for (j = 0; j < 3; j++) {
                    if (need[i][j] > work[j]) {
                        break;
                    }
                }

                if (j == 3) {

                    for (k = 0; k < 3; k++) {
                        work[k] += allocation[i][k];
                    }

                    safe[count] = i;
                    count++;

                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        if (!found) {
            printf("\nUnsafe state\n");
            return 0;
        }
    }

    printf("\nSystem is in safe state\n");

    printf("Safe sequence:\n");

    for (i = 0; i < 5; i++) {
        printf("P%d ", safe[i]);
    }

    printf("\n");

    return 0;
}
