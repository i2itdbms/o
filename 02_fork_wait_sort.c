#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

void sort(int a[], int n, int desc) {
    int temp;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if ((!desc && a[j] > a[j + 1]) ||
                (desc && a[j] < a[j + 1])) {

                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n, arr[100], i;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter array elements: ");
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed");
        return 1;
    }

    if (pid == 0) {
        printf("This is child (Ascending Order)\n");
        sort(arr, n, 0);

        for (i = 0; i < n; i++)
            printf("%d ", arr[i]);

        printf("\n");
    } 
    else {
        wait(NULL);
        printf("This is parent (Descending Order)\n");

        sort(arr, n, 1);

        for (i = 0; i < n; i++)
            printf("%d ", arr[i]);

        printf("\n");
    }

    return 0;
}

