#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int process_id = getpid();
    printf("Process ID: %d\n", process_id);

    int parent_id = getppid();
    printf("Parent Process ID: %d\n", parent_id);

    int f = fork();

    if (f == 0) {
        printf("Child:\n");
        process_id = getpid();
        printf("Process ID: %d\n", process_id);
        parent_id = getppid();
        printf("Parent Process ID: %d\n", parent_id);
    } else {
        wait(NULL);
        printf("Parent:\n");
        process_id = getpid();
        printf("Process ID: %d\n", process_id);
        parent_id = getppid();
        printf("Parent Process ID: %d\n", parent_id);
    }

    return 0;
}
