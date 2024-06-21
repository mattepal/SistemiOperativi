/*file parametri2.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc,char **argv){
    int fd;
    int N;
    int lencar;
    if(argc!=4){
        printf("ERROR: numero di parametri non corretto");
        exit(1);
    }
    fd=open(argv[1],O_RDONLY);
    if(fd<0){
        printf("ERROR: il parametro %s non è un file\n",argv[1]);
        exit(2);
    }
    printf("DEBUG-OK: 1^ parametro corretto\n");
    N=atoi(argv[2]);
    if(N<=0){
        printf("ERROR:il parametro %s non è un numero\n",argv[2]);
        exit(3);
    }
    printf("DEBUG-OK: 2^ parametro correttto\n");
    lencar=strlen(argv[3]);
    if(lencar!=1){
        printf("ERROR: il parametro %s non è un singolo carattere\n",argv[3]);
        exit(4);
    }
    printf("DEBRUG-OK: 3^ parametro corretto\n");
    printf("il nome del file eseguibile è: %s e il numero di parametri è %d\n", argv[0],argc);
    printf("il primo parametro è un file di nome: %s\n",argv[1]);
    printf("il secondo parametro è un numero: %s\n",argv[2]);
    printf("il terzo parametro è un carattere:%s\n",argv[3]);
    exit(0);
}