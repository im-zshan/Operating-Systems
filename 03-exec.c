#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int f;

    // execl
    f = fork();
    if (f == 0) {
        printf("execl --> ls -a\n");
        int status_code = execl("/bin/ls", "ls", "-a", (char*)NULL);
        if (status_code == -1) {
            printf("Child process did not terminate correctly!\n");
            return 1;
        }
    }
    wait(NULL);

    // execv
    f = fork();
    if (f == 0) {
        printf("execv --> ls -a\n");
        char* argument_list[] = {"ls", "-a", NULL};
        int status_code = execv("/bin/ls", argument_list);
        if (status_code == -1) {
            printf("Child process did not terminate correctly!\n");
            return 1;
        }
    }
    wait(NULL);

    // execlp
    f = fork();
    if (f == 0) {
        printf("execlp --> ls -a\n");
        int status_code = execlp("ls", "ls", "-a", (char*)NULL);
        if (status_code == -1) {
            printf("Child process did not terminate correctly!\n");
            return 1;
        }
    }
    wait(NULL);

    // execvp
    f = fork();
    if (f == 0) {
        printf("execvp --> ls -a\n");
        char* argument_list[] = {"ls", "-a", NULL};
        int status_code = execvp("ls", argument_list);
        if (status_code == -1) {
            printf("Child process did not terminate correctly!\n");
            return 1;
        }
    }
    wait(NULL);

    // execle
    f = fork();
    if (f == 0) {
        printf("execle --> ls -a\n");
        char* const env_list[] = {"A=BC", "D=EF", NULL};
        int status_code = execle("/bin/ls", "ls", "-a", (char*)NULL, env_list);
        if (status_code == -1) {
            printf("Child process did not terminate correctly!\n");
            return 1;
        }
    }
    wait(NULL);

    // execve
    f = fork();
    if (f == 0) {
        printf("execve --> ls -a\n");
        char* argument_list[] = {"ls", "-a", NULL};
        char* const env_list[] = {"A=BC", "D=EF", NULL};
        int status_code = execve("/bin/ls", argument_list, env_list);
        if (status_code == -1) {
            printf("Child process did not terminate correctly!\n");
            return 1;
        }
    }
    wait(NULL);

    printf("Parent process terminated successfully!\n");

    return 0;
}
