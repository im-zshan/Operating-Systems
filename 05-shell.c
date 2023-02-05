#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_CMD_BUFFER 100
#define MAX_CMD_ARGS 25
#define MAX_ARG_LEN 50

#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RESET "\x1b[0m"

// Configure Arrow Keys
// Tab Complete

void new_command() {
    char cwd[PATH_MAX];
    printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET " $ ", getcwd(cwd, sizeof(cwd)));
}

void sigint_handler(int signum) {
    printf("\n");
    new_command();
    fflush(stdout);
}

void cmd_help() {
    printf("Command     Description\n");
    printf("=======     ===========\n");
    printf("help        Display help menu\n");
    printf("pid         Display process id of current process\n");
    printf("ppid        Display process id of parent of current process\n");
    printf("exit        Exit shell\n");
    printf("\n");
    printf("Other shell commands are also supported!\n");
}

void cmd_pid() {
    printf("Process ID: %d\n", getpid());
}

void cmd_ppid() {
    printf("Parent Process ID: %d\n", getppid());
}

int get_command(char cmd[], char command[]) {
    int quote = 0;
    int i = 0;
    int j = 0;
    int len = strlen(cmd);
    for (i = 0; i != len; ++i) {
        if (cmd[i] == '\n') {
            break;
        } else if (cmd[i] == '\\') {
            command[j] = cmd[i + 1];
            ++j;
            ++i;
        } else if (cmd[i] == '\"') {
            quote = !quote;
        } else if (cmd[i] == ' ' && !quote) {
            break;
        } else {
            command[j] = cmd[i];
            ++j;
        }
    }
    command[j] = '\0';
    return i;
}

int get_args_list(char cmd[], char* args[], int j) {
    char* arg;
    int len = strlen(cmd);
    int i;
    int k;
    int quote;
    for (i = 1; i < MAX_CMD_ARGS && j < len - 1; ++i) {
        arg = (char*)malloc(MAX_ARG_LEN * sizeof(char));
        j += 1;
        k = 0;
        quote = 0;
        for (; j < len; ++j) {
            if (cmd[j] == '\n') {
                break;
            } else if (cmd[j] == '\\') {
                arg[k] = cmd[j + 1];
                ++j;
                ++k;
            } else if (cmd[j] == '\"') {
                quote = !quote;
            } else if (cmd[j] == ' ' && !quote) {
                break;
            } else {
                arg[k] = cmd[j];
                ++k;
            }
        }
        arg[k] = '\0';
        args[i] = arg;
    }
    return i;
}

void handle_exec(char cmd[]) {
    char command[MAX_CMD_BUFFER];
    char* args_list[MAX_CMD_ARGS + 1];
    int command_index = get_command(cmd, command);
    args_list[0] = (char*)malloc(MAX_ARG_LEN * sizeof(char));
    strcpy(args_list[0], command);
    int total_args = get_args_list(cmd, args_list, command_index);
    args_list[total_args] = NULL;

    int status_code;
    if (!strcmp("cd", command)) {
        status_code = chdir(args_list[1]);
        if (status_code == -1) {
            printf("[EXCEPTION] Invalid path\n");
        }
    } else {
        int f = fork();
        if (f == 0) {
            status_code = execvp(command, args_list);
            if (status_code == -1) {
                printf("[EXCEPTION] Process did not terminate correctly!\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    wait(NULL);
}

int handle_command(char cmd[]) {
    if (!strcmp("exit\n", cmd)) {
        return 0;
    } else if (!strcmp("help\n", cmd)) {
        cmd_help();
    } else if (!strcmp("pid\n", cmd)) {
        cmd_pid();
    } else if (!strcmp("ppid\n", cmd)) {
        cmd_ppid();
    } else {
        handle_exec(cmd);
    }
    return 1;
}

int main() {
    signal(SIGINT, sigint_handler);
    char cmd[MAX_CMD_BUFFER];

    do {
        new_command();
        fgets(cmd, MAX_CMD_BUFFER, stdin);
    } while (handle_command(cmd));

    return 0;
}
