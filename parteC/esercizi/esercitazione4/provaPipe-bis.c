#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    int fd1,fd2;
    int piped[2];
    if(argc!=3){
        printf("ERROR: numero di parametri non corretto");
        exit(1);
    }
    if((fd1=open(argv[1],O_RDONLY))<0){
        printf("ERROR: errore nella apertura del file 1");
        exit(2);
    }
    printf("valore di fd1=%d",fd1);
    if((fd2=open(argv[2],O_RDONLY))<0){
        printf("ERROR: errore nella apertura del file 2");
        exit(3);
    }
    printf("valore di fd2=%d",fd2);
    close(fd1);
    if(pipe(piped)<0){
        printf("ERROR:errore nella creazione del pipe");
        exit(3);
    }
    printf("creata pipe con piped[0]=%d\n",piped[0]);
    printf("creata pipe con piped[1]=%d\n",piped[1]);

    exit(0);
}
