// Run Using "gcc -pthread 23_semaphores_lock.c && ./a.out"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM 25
#define OPR 10000

sem_t sem;

typedef struct str {
    sem_t* sem;
    int* num;
} str;

void* locked_increment(void* vargp) {
    str* s = (str*)vargp;
    sem_t* sem = s->sem;
    int* num = s->num;

    for (int i = 0; i < OPR; i++) {
        sem_wait(sem);
        *num = *num + 1;
        sem_post(sem);
    }

    return NULL;
}

int main() {
    int num = 0;
    pthread_t tid[NUM];

    sem_init(&sem, 0, 1);

    str s;
    s.sem = &sem;
    s.num = &num;

    for (int i = 0; i < NUM; i++) {
        pthread_create(&tid[i], NULL, locked_increment, (void*)&s);
    }

    for (int i = 0; i < NUM; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("Number = %d\n", num);
    printf("Result: %s (Diff: %d)\n", num == NUM * OPR ? "PASS" : "FAIL", NUM * OPR - num);

    return 0;
}
