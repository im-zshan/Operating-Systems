// Run Using "gcc -pthread 16_producer_consumer.c && ./a.out"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX 10
#define OPR 25

pthread_mutex_t buf_lock;
pthread_mutex_t num_lock;

typedef struct str {
    pthread_mutex_t* buf_lock;
    pthread_mutex_t* num_lock;
    int* buffer;
    int* head;
    int* tail;
    int* num;
} str;

void* producer(pthread_mutex_t* buf_lock, int* buffer, int* head, int* tail, int data) {
    pthread_mutex_lock(buf_lock);
    if ((*tail + 1) % MAX == *head) {
        printf("Buffer is full. Producer is waiting.\n");
        pthread_mutex_unlock(buf_lock);
        while ((*tail + 1) % MAX == *head);
        pthread_mutex_lock(buf_lock);
    }
    buffer[*tail] = data;
    *tail = (*tail + 1) % MAX;
    pthread_mutex_unlock(buf_lock);

    return NULL;
}

int consume(pthread_mutex_t* buf_lock, int* buffer, int* head, int* tail) {
    pthread_mutex_lock(buf_lock);
    if (*head == *tail) {
        printf("Buffer is empty. Consumer is waiting.\n");
        pthread_mutex_unlock(buf_lock);
        while (*head == *tail);
        pthread_mutex_lock(buf_lock);
    }
    int data = buffer[*head];
    *head = (*head + 1) % MAX;
    pthread_mutex_unlock(buf_lock);

    return data;
}

void* producer_thread(void* vargp) {
    str* s = (str*)vargp;
    pthread_mutex_t* buf_lock = s->buf_lock;
    pthread_mutex_t* num_lock = s->num_lock;
    int* buffer = s->buffer;
    int* head = s->head;
    int* tail = s->tail;
    int* num = s->num;
    free(s);
    srand(time(NULL));

    for (int i = 0; i < OPR; i++) {
        int data = rand() % 100 + 1;
        pthread_mutex_lock(num_lock);
        printf("num = %d + %d = %d\n", *num, data, *num + data);
        *num = *num + data;
        pthread_mutex_unlock(num_lock);
        producer(buf_lock, buffer, head, tail, data);
        // usleep(100);
    }

    return NULL;
}

void* consumer_thread(void* vargp) {
    str* s = (str*)vargp;
    pthread_mutex_t* buf_lock = s->buf_lock;
    pthread_mutex_t* num_lock = s->num_lock;
    int* buffer = s->buffer;
    int* head = s->head;
    int* tail = s->tail;
    int* num = s->num;
    free(s);
    srand(time(NULL));

    for (int i = 0; i < OPR; i++) {
        int data = consume(buf_lock, buffer, head, tail);
        pthread_mutex_lock(num_lock);
        printf("num = %d - %d = %d\n", *num, data, *num - data);
        *num = *num - data;
        pthread_mutex_unlock(num_lock);
        // usleep(100);
    }

    return NULL;
}

int main() {
    int buffer[MAX];
    int head = 0, tail = 0;
    int num = 0;

    pthread_t p;
    pthread_t c;

    // Mutex Lock Implementation
    printf("Mutex Lock Implementation (Single Producer-Consumer):\n");

    str* s1 = (str*)malloc(sizeof(str));
    s1->buf_lock = &buf_lock;
    s1->num_lock = &num_lock;
    s1->buffer = buffer;
    s1->head = &head;
    s1->tail = &tail;
    s1->num = &num;

    pthread_create(&p, NULL, producer_thread, s1);

    str* s2 = (str*)malloc(sizeof(str));
    s2->buf_lock = &buf_lock;
    s2->num_lock = &num_lock;
    s2->buffer = buffer;
    s2->head = &head;
    s2->tail = &tail;
    s2->num = &num;

    pthread_create(&c, NULL, consumer_thread, s2);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    printf("Result: %s\n", num == 0 ? "Success" : "Fail");

    return 0;
}
