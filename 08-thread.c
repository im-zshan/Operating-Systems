// Run Using "gcc -pthread 08_thread.c && ./a.out"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* thread_function(void* vargp) {
    int* i = (int*)vargp;
    printf("Executing: thread_function(%d)\n", *i);
    return NULL;
}

int main() {
    pthread_t tid[5];

    // Shared Variable
    printf("Shared Variables:\n");
    for (int i=0; i<5; i++) {
        pthread_create(&tid[i], NULL, thread_function, (void*)&i);
    }
    for (int i=0; i<5; i++) {
        pthread_join(tid[i], NULL);
    }

    // Unsahared Variable
    printf("Unshared Variables:\n");
    int arr[5];
    for (int i=0; i<5; i++) {
        arr[i] = i;
        pthread_create(&tid[i], NULL, thread_function, (void*)&arr[i]);
    }
    for (int i=0; i<5; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}
