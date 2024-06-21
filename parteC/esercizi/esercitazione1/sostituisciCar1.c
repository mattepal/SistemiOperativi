/*file sostituisciCar.c*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc,char **argv){

    int fd;
    char c,caract;
    if(argc!=4){
        printf("ERROR: numero di parametri non corretto");
        exit(1);
    }
    fd=open(argv[1],O_RDWR);
    if(fd<=0){
        printf("ERROR: il parametro %s non è un file",argv[1]);
        exit(2);
    }
    printf("DEBUG-OK: primo prarametro corretto");
    if(strlen(argv[2])!=1){
        printf("ERROR: il parametro %s non è un carattere",argv[2]);
        exit(3);
    }
    printf("DEBUG-OK: secondo parametro corretto");
    c=argv[2][0];
    char change;
    if(strlen(argv[3])!=1){
         printf("ERROR: il parametro %s non è un carattere",argv[3]);
        exit(4);
    }
    printf("DEBUG-OK: terzo parametro corretto");
    change=argv[3][0];
    while (read(fd,&caract,1))
    {
        if(c==caract){
            lseek(fd,-1L,1);
            write(fd,&change,1);
        }
    }
    
    exit(0);
}