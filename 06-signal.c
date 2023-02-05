#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sigint_handler(int signum) {
    printf("\rYou Pressed \"Crtl+C\"...\n");
    printf("Press \"Crtl+\\\" to Quit...\n");
}

void sigquit_handler(int signum) {
    printf("\rYou Pressed \"Crtl+\\\"...\n");
    printf("Exiting Program...\n");
    exit(0);
}

int main() {
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
    for (int i=1;; ++i) {
        printf("%d: Main Function\n", i);
        sleep(1);
    }
    return 0;
}
