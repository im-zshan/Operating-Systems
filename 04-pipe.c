#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int p;
    int f;
    int pipefds[2];
    char *pin;
    char buffer[5];

    p = pipe(pipefds);

    if (p == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    f = fork();

    if (f == 0) {
        close(pipefds[0]);
        pin = "1248\0";
        write(pipefds[1], pin, 5);
        printf("Intentional Delay...\n");
        sleep(2);
        pin = "8421\0";
        for (int i=0; i!=5; ++i) {
            sleep(1);
            write(pipefds[1], pin, 5);
        }
        exit(EXIT_SUCCESS);
    } else {
        int r;
        int count = 0;
        close(pipefds[1]);
        do {
            r = read(pipefds[0], buffer, 5);
            if (r)
                printf("Received PIN %d: %s\n", count++, buffer);
        } while (r);
        wait(NULL);
        close(pipefds[0]);
    }

    return EXIT_SUCCESS;
}
