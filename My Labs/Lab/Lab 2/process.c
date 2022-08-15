// Topic - To Implement the process manipulation system calls like fork(), exec(), etc.

#include <stdio.h>
#include <unistd.h>

int main() {
    int process_id = getpid();
    printf("Process ID: %d\n", process_id);

    int parent_id = getppid();
    printf("Parent Process ID: %d\n", parent_id);

    return 0;
}
