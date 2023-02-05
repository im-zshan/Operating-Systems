// Run Using "gcc -pthread 27_producer_consumer_multi_semaphores.c && ./a.out"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX 10
#define OPR 25
#define PRO 3
#define CON 3

sem_t empty, full;
sem_t buf_lock;
sem_t num_lock;

typedef struct str {
    sem_t* empty;
    sem_t* full;
    sem_t* buf_lock;
    sem_t* num_lock;
    int* buffer;
    int* head;
    int* tail;
    int* num;
} str;

void* producer(sem_t* empty, sem_t* full, sem_t* buf_lock, int* buffer, int* head, int* tail, int data) {
    sem_wait(empty);
    sem_wait(buf_lock);
    buffer[*tail] = data;
    *tail = (*tail + 1) % MAX;
    sem_post(buf_lock);
    sem_post(full);

    return NULL;
}

int consume(sem_t* empty, sem_t* full, sem_t* buf_lock, int* buffer, int* head, int* tail) {
    sem_wait(full);
    sem_wait(buf_lock);
    int data = buffer[*head];
    *head = (*head + 1) % MAX;
    sem_post(buf_lock);
    sem_post(empty);

    return data;
}

void* producer_thread(void* vargp) {
    str* s = (str*)vargp;
    sem_t* empty = s->empty;
    sem_t* full = s->full;
    sem_t* buf_lock = s->buf_lock;
    sem_t* num_lock = s->num_lock;
    int* buffer = s->buffer;
    int* head = s->head;
    int* tail = s->tail;
    int* num = s->num;
    free(s);
    srand(time(NULL));

    for (int i = 0; i < OPR; i++) {
        int data = rand() % 100 + 1;
        sem_wait(num_lock);
        printf("num = %d + %d = %d\n", *num, data, *num + data);
        *num = *num + data;
        sem_post(num_lock);
        producer(empty, full, buf_lock, buffer, head, tail, data);
        // usleep(100);
    }

    return NULL;
}

void* consumer_thread(void* vargp) {
    str* s = (str*)vargp;
    sem_t* empty = s->empty;
    sem_t* full = s->full;
    sem_t* buf_lock = s->buf_lock;
    sem_t* num_lock = s->num_lock;
    int* buffer = s->buffer;
    int* head = s->head;
    int* tail = s->tail;
    int* num = s->num;
    free(s);
    srand(time(NULL));

    for (int i = 0; i < OPR; i++) {
        int data = consume(empty, full, buf_lock, buffer, head, tail);
        sem_wait(num_lock);
        printf("num = %d - %d = %d\n", *num, data, *num - data);
        *num = *num - data;
        sem_post(num_lock);
        // usleep(100);
    }

    return NULL;
}

int main() {
    int buffer[MAX];
    int head = 0, tail = 0;
    int num = 0;

    pthread_t p[PRO];
    pthread_t c[CON];

    sem_init(&empty, 0, 2);
    sem_init(&full, 0, 0);
    sem_init(&buf_lock, 0, 1);
    sem_init(&num_lock, 0, 1);

    // Semaphores Implementation
    printf("Semaphores Implementation (Multiple Producer-Consumer):\n");

    str* s1;

    for (int i = 0; i < PRO; i++) {
        s1 = (str*)malloc(sizeof(str));
        s1->empty = &empty;
        s1->full = &full;
        s1->buf_lock = &buf_lock;
        s1->num_lock = &num_lock;
        s1->buffer = buffer;
        s1->head = &head;
        s1->tail = &tail;
        s1->num = &num;
        pthread_create(&p[i], NULL, producer_thread, s1);
    }

    str* s2;

    for (int i = 0; i < CON; i++) {
        s2 = (str*)malloc(sizeof(str));
        s2->empty = &empty;
        s2->full = &full;
        s2->buf_lock = &buf_lock;
        s2->num_lock = &num_lock;
        s2->buffer = buffer;
        s2->head = &head;
        s2->tail = &tail;
        s2->num = &num;
        pthread_create(&c[i], NULL, consumer_thread, s2);
    }

    for (int i = 0; i < PRO; i++)
        pthread_join(p[i], NULL);

    for (int i = 0; i < CON; i++)
        pthread_join(c[i], NULL);

    printf("Result: %s\n", num == 0 ? "Success" : "Fail");

    return 0;
}
