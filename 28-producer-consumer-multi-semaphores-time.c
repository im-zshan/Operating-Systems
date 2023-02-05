// Run Using "gcc -pthread 28_producer_consumer_multi_semaphores_time.c && ./a.out"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX 25
#define OPR 2500
#define PRO 25
#define CON 25

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
    clock_t* time;
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
    clock_t* t = s->time;
    free(s);
    srand(time(NULL));

    *t = clock();
    for (int i = 0; i < OPR; i++) {
        int data = rand() % 100 + 1;
        sem_wait(num_lock);
        // printf("num = %d + %d = %d\n", *num, data, *num + data);
        *num = *num + data;
        sem_post(num_lock);
        producer(empty, full, buf_lock, buffer, head, tail, data);
        // usleep(100);
    }
    *t = clock() - *t;

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
    clock_t* t = s->time;
    free(s);
    srand(time(NULL));

    *t = clock();
    for (int i = 0; i < OPR; i++) {
        int data = consume(empty, full, buf_lock, buffer, head, tail);
        sem_wait(num_lock);
        // printf("num = %d - %d = %d\n", *num, data, *num - data);
        *num = *num - data;
        sem_post(num_lock);
        // usleep(100);
    }
    *t = clock() - *t;

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

    clock_t t_pro[PRO];
    clock_t t_con[CON];

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
        s1->time = &t_pro[i];
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
        s2->time = &t_con[i];
        pthread_create(&c[i], NULL, consumer_thread, s2);
    }

    for (int i = 0; i < PRO; i++)
        pthread_join(p[i], NULL);

    for (int i = 0; i < CON; i++)
        pthread_join(c[i], NULL);

    printf("Result: %s\n", num == 0 ? "Success" : "Fail");

    // for (int i = 0; i < PRO; i++)
    //     printf("Producer %d: %f\n", i, (double)t_pro[i] / CLOCKS_PER_SEC);
    
    // for (int i = 0; i < CON; i++)
    //     printf("Consumer %d: %f\n", i, (double)t_con[i] / CLOCKS_PER_SEC);

    double total_pro = 0;
    for (int i = 0; i < PRO; i++)
        total_pro += (double)t_pro[i] / CLOCKS_PER_SEC;
    printf("Total Producer Time: %f\n", total_pro);

    double total_con = 0;
    for (int i = 0; i < CON; i++)
        total_con += (double)t_con[i] / CLOCKS_PER_SEC;
    printf("Total Consumer Time: %f\n", total_con);

    double total_time = total_pro + total_con;
    printf("Total Time: %f\n", total_time);

    printf("Average Producer Time: %f\n", total_pro / PRO);
    printf("Average Consumer Time: %f\n", total_con / CON);
    printf("Average Time: %f\n", total_time / (PRO + CON));

    return 0;
}
