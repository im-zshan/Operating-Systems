#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    // create
    int fd1 = creat("test 1.txt", 0642);
    if (fd1 == -1) {
        printf("Cannot create file!\n");
    }
    else {
        printf("File created sucessfully\n");
        // close
        if (close(fd1) == -1)
            printf("Cannot close file!\n");
        else
            printf("File closed sucessfully\n");
    }

    sleep(1);

    // open
    int fd2 = open("test 2.txt",  O_CREAT|O_RDWR, 0642);
    if (fd2 == -1)
        printf("Cannot open file!\n");
    else
        printf("File opened sucessfully\n");

    // write
    char buffer1[] = "hello world";
    write(fd2, buffer1, strlen(buffer1));
    printf("Write: %s\n", buffer1);

    // seek
    lseek(fd2, 0, SEEK_SET);

    // read
    char buffer2[12];
    read(fd2, buffer2, 11);
    buffer2[11] = 0;
    printf("Read: %s\n", buffer2);

    // close
    if (close(fd2) == -1)
        printf("Cannot close file!\n");
    else
        printf("File closed sucessfully\n");

    return 0;
}
