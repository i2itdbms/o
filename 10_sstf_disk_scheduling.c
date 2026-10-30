10


#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, head, i, j, total = 0;
    int req[20], visited[20] = {0};

    printf("Enter number of requests: ");
    scanf("%d", &n);

    printf("Enter disk requests: ");
    for(i = 0; i < n; i++)
        scanf("%d", &req[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    for(i = 0; i < n; i++)
    {
        int min = 9999, pos = -1;

        for(j = 0; j < n; j++)
        {
            if(!visited[j] && abs(head - req[j]) < min)
            {
                min = abs(head - req[j]);
                pos = j;
            }
        }

        total += min;
        head = req[pos];
        visited[pos] = 1;
    }

    printf("Total Head Movement = %d", total);

    return 0;
}
