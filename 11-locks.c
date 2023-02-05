// Run Using "gcc -pthread 11_locks.c && ./a.out"

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

enum status{isfree, isheld};

typedef struct lock {
    enum status* status;
    int *num;
} lock;

void* locked_increment(void* vargp) {
    lock* l = (lock*)vargp;
    int* num = l -> num;
    enum status* status = l -> status;

    for (int i = 0; i < 10000; i++) {
        while (*status == isheld) {
            usleep(1);
        }
        // context switch here will cause inconsistencies
        *status = isheld;
        *num = *num + 1;
        *status = isfree;
    }

    return NULL;
}

int main() {
    int num;
    pthread_t tid[5];

    // Simple Increment
    printf("Simple Increment:\n");
    num = 0;
    for (int i=0; i<5; i++) {
        pthread_create(&tid[i], NULL, simple_increment, (void*)&num);
    }
    for (int i=0; i<5; i++) {
        pthread_join(tid[i], NULL);
    }
    printf("Number = %d\n", num);
    printf("Result: %s (Diff: %d)\n", num == 50000 ? "PASS" : "FAIL", 50000 - num);

    // Locked Increment
    printf("Locked Increment:\n");

    num = 0;

    lock l;
    enum status status = isfree;
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
