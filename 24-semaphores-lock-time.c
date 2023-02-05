// Run Using "gcc -pthread 24_semaphores_lock_time.c && ./a.out"

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
    clock_t* time;
} str;

void* locked_increment(void* vargp) {
    str* s = (str*)vargp;
    sem_t* sem = s->sem;
    int* num = s->num;
    clock_t* time = s->time;
    free(s);

    *time = clock();
    for (int i = 0; i < OPR; i++) {
        sem_wait(sem);
        *num = *num + 1;
        sem_post(sem);
    }
    *time = clock() - *time;

    return NULL;
}

int main() {
    int num = 0;
    pthread_t tid[NUM];
    clock_t time[NUM];

    sem_init(&sem, 0, 1);

    for (int i = 0; i < NUM; i++) {
        str* s = (str*)malloc(sizeof(str));
        s->sem = &sem;
        s->num = &num;
        s->time = &time[i];
        pthread_create(&tid[i], NULL, locked_increment, (void*)s);
    }

    for (int i = 0; i < NUM; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("Number = %d\n", num);
    printf("Result: %s (Diff: %d)\n", num == NUM * OPR ? "PASS" : "FAIL", NUM * OPR - num);

    for (int i = 0; i < NUM; i++) {
        printf("Time taken for thread %d: %f\n", i, (double)time[i] / CLOCKS_PER_SEC);
    }

    return 0;
}
