// Run Using "gcc -pthread 10_thread_matrix_mul.c && ./a.out"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct matrix_index {
    int* ptr;
    int* m1;
    int* m2;
    int* r1;
    int* c1;
    int* r2;
    int* c2;
    int *i;
} MI;

void* matrix_mul(void* vargp) {
    MI* str = (MI*)vargp;
    int* ptr = str -> ptr;
    int* m1 = str -> m1;
    int* m2 = str -> m2;
    int r1 = *(str -> r1);
    int c1 = *(str -> c1);
    int r2 = *(str -> r2);
    int c2 = *(str -> c2);
    int i = *(str -> i);
    free(str);

    for (int j=0; j<c2; j++) {
        for (int k=0; k<c1; k++) {
            *(ptr + i*c2 + j) += m1[i*c1 + k] * m2[k*c2 + j];
        }
    }

    return NULL;
}

int main() {
    int r1, c1, r2, c2;
    r1 = 5; c1 = 4; r2 = 4; c2 = 6;

    int* m1 = malloc((r1 * c1) * sizeof(int));
    int* m2 = malloc((r2 * c2) * sizeof(int));
    int* result = malloc((r1 * c2) * sizeof(int));

    m1 = (int[]){1, 2, 3, 4, 4, 3, 2, 1, 1, 2, 2, 1, 4, 3, 2, 1, 1, 2, 3, 4};
    m2 = (int[]){1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 1, 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6};

    for (int i=0; i<r1*c2; i++)
        *(result + i) = 0;

    int t = r1;
    pthread_t tid[t];

    int index[t];
    for (int i=0; i<t; i++)
        index[i] = i;

    for (int i=0; i<t; i++) {
        // MI str; // Unexpected Result Because AI Gets Destroyed
        MI* str = malloc(sizeof(MI));
        str -> ptr = result;
        str -> m1 = m1;
        str -> m2 = m2;
        str -> r1 = &r1;
        str -> c1 = &c1;
        str -> r2 = &r2;
        str -> c2 = &c2;
        str -> i = &index[i];
        pthread_create(&tid[i], NULL, matrix_mul, (void*)str);
    }

    for (int i = 0; i < t; i++) {
        pthread_join(tid[i], NULL);
    }

    for (int i=0; i<r1; i++) {
        for (int j=0; j<c2; j++)
            printf("%d ", *(result + i*c2 + j));
        printf("\n");
    }

    return 0;
}
