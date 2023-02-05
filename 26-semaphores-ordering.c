// Run Using "gcc -pthread 26_semaphores_ordering.c && ./a.out"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PRO 5

int order[] = {2, 4, 3, 1, 0};

sem_t sem[PRO];

typedef struct str {
    sem_t* wait;
    sem_t* signal;
    int id;
} str;

void* process(void* vargp) {
    str* s = (str*)vargp;
    sem_t* wait = s->wait;
    sem_t* signal = s->signal;
    int id = s->id;
    free(s);

    if (wait != NULL)
        sem_wait(wait);
    printf("Process: %d\n", id);
    if (signal != NULL)
        sem_post(signal);

    return NULL;
}

int main() {
    pthread_t tid[PRO];

    for (int i = 0; i < PRO; i++) {
        sem_init(&sem[i], 0, 0);
    }

    printf("Default Order:\n");
    for (int i = 0; i < PRO; i++) {
        str* s = (str*)malloc(sizeof(str));
        s->wait = i == 0 ? NULL : &sem[i - 1];
        s->signal = i == PRO - 1 ? NULL : &sem[i];
        s->id = i;
        pthread_create(&tid[i], NULL, process, (void*)s);
    }

    for (int i = 0; i < PRO; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("Predefined Order:\n");
    for (int i = 0; i < PRO; i++) {
        str* s = (str*)malloc(sizeof(str));
        s->wait = order[i] == 0 ? NULL : &sem[order[i] - 1];
        s->signal = order[i] == PRO - 1 ? NULL : &sem[order[i]];
        s->id = i;
        pthread_create(&tid[i], NULL, process, (void*)s);
    }

    for (int i = 0; i < PRO; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}
