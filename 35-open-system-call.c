#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdio.h>

int main(int argc, char *argv) {
        int fd = open("file", O_RDONLY);

        if (fd < 0){
                perror("Opening of the file is failed\n");
        }
        else {
                printf("file sucessfully opened\n");
        }

        close(fd); 
        return 0;
}