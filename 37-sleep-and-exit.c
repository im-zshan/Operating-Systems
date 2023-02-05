#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    for(int i=0;i<10;i++){
        sleep(1);
        printf("\n i am line %d \n",i);
        if(i==7)
        exit(1);
    }
    return 0;
}