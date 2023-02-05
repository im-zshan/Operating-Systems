// Run Using "gcc -pthread 09_thread_array_sum.c && ./a.out"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct array_index {
    int* ptr;
    int* arr;
    int start;
    int end;
} AI;

void* array_sum(void* vargp) {
    AI* str = (AI*)vargp;
    int* ptr = str -> ptr;
    int* arr = str -> arr;
    int start = str -> start;
    int end = str -> end;
    free(str);
    for (int i=start; i<end; i++)
        *ptr += arr[i];
    return NULL;
}

int main() {
    int t = 5;
    pthread_t tid[t];
    int result[t];

    for (int i=0; i<t; i++)
        result[i] = 0;

    int n = 21;
    int arr[n];

    for (int i=0; i<n; i++)
        arr[i] = i + 1;
    
    int size = n/(t-1);
    int last = n%(t-1);
    
    for (int i=0; i<t; i++) {
        // AI str; // Unexpected Result Because AI Gets Destroyed
        AI* str = malloc(sizeof(AI));
        str -> ptr = &result[i];
        str -> arr = arr;
        str -> start = size*i;
        str -> end = (i == t-1) ? n : size*(i+1);
        pthread_create(&tid[i], NULL, array_sum, (void*)str);
    }

    for (int i = 0; i < t; i++) {
        pthread_join(tid[i], NULL);
    }

    for (int i=0; i<t; i++)
        printf("result[%d] = %d\n", i, result[i]);
    
    int sum = 0;
    for (int i=0; i<t; i++)
        sum += result[i];
    
    printf("sum = %d\n", sum);
    printf("result = %s\n", sum == n*(n+1)/2 ? "Pass" : "Fail");

    return 0;
}
