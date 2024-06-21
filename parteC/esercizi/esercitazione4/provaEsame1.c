/* file provaEsame1.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef int pipe_t[2];

int main(int argc, char const *argv[])
{
    /*variabili locali*/
    int pid;
    int pidFiglio,status,ritorno;
    int N;
    pipe_t *piped;
    
    /*--------*/
    /*controllo dei parametri in input*/
    if(argc<3){
        printf("ERROR: %d numero di parametri non corretto",argc-1);
        exit(1);
    }

    exit(0);
}
