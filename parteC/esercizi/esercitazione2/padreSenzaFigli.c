#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(){
    int pidFiglio;
    int status;
    printf("creato processo padre con pid %d",getpid());
    if((pidFiglio=wait(&status))<0){
        printf("ERROR: errore in wait ");
        exit(1);
    }
    exit(0);
}