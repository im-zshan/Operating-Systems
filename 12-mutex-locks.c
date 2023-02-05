// Run Using "gcc -pthread 12_mutex_locks.c && ./a.out"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* simple_increment(void* vargp) {
    int* num = (int*)vargp;
    
    for (int i = 0; i < 10000; i++) {
        *num = *num + 1;
    }

    return NULL;
}

pthread_mutex_t status;

typedef struct lock {
    pthread_mutex_t* status;
    int *num;
} lock;

void* locked_increment(void* vargp) {
    lock* l = (lock*)vargp;
    int* num = l -> num;
    pthread_mutex_t* status = l -> status;

    for (int i = 0; i < 10000; i++) {
        pthread_mutex_lock(status);
        *num = *num + 1;
        pthread_mutex_unlock(status);
    }

    return NULL;
}

int main() {
    int num;
    pthread_t tid[5];

    // Mutex Locked Increment
    printf("Locked Increment:\n");

    num = 0;

    lock l;
    pthread_mutex_init(&status, NULL);
    l.status = &status;
    l.num = &num;

    for (int i=0; i<5; i++) {
        pthread_create(&tid[i], NULL, locked_increment, (void*)&l);
    }
    for (int i=0; i<5; i++) {
        pthread_join(tid[i], NULL);
    }
    printf("Number = %d\n", num);
    printf("Result: %s (Diff: %d)\n", num == 50000 ? "PASS" : "FAIL", 50000 - num);

    return 0;
}
