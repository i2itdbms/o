#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, head, i, j, total = 0;
    int req[20], visited[20] = {0};


    printf("Enter the number of request:");
    scanf("%d",&n);

    printf("Enter the disk Request:");
    for(i = 0; i<n; i++)
        scanf("%d" , &req[i]);
    
    printf("Enter intial head Position:");
    scanf("%d", &head);

    printf("Head Movement: %d -> ", head);



    for( i =0; i<n; i++){
        int min = 9999 , pos = -1;

        for(j = 0; j<n; j++){
            if(!visited[j] && abs(head-req[j]) < min){
                min = abs(head -req[j]);
                pos = j;

            }
        }

        total += min;

        printf("%d ->", req[pos]);
        head = req[pos];
        visited[pos] = 1;

    }   

    printf("\nTotal head movement = %d" , total);
    
    return 0;


}
