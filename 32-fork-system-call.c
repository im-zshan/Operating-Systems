#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    printf("\nPID of Parent process is %d\n",getpid());
    int s=getpid();
    fork();
    sleep(1);
    if(getpid()==s){
        printf("\n Parent Process is Running \n");
    }else{
        sleep(1);
        printf("\n Child Process is Running \n  PID of new Process is %d \n",getpid());
    }
    sleep(1);
    return 0;
}