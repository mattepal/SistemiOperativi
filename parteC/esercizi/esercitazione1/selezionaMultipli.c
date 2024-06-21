/* file selezionaMultipli.c*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd,n,nr,i=1;
    char *buffer;
    if(argc!=3){
        printf("ERROR:nunero di parametri non corretto");
        exit(1);
    }
    fd=open(argv[1],O_RDONLY);
    if(fd<0){
        printf("ERROR: parametro %s non corretto",argv[1]);
        exit(2);
    }
    printf("DEBUG-OK: 1^ parametro corretto\n");
    n=atoi(argv[2]);
    if (n<=0){
        printf("ERROR: il parametro %s non è un numero",argv[2]);
        exit(3);
    }
    printf("DEBUG-OK: 2^ parametro corretto\n");
    buffer=malloc(n*sizeof(char));
    if(buffer==NULL){
        printf("ERROR: errore nella creazione della malloc");
        exit(4);
    }
    printf("selezioniamo i caratteri multipli di %d\n",n);
    while ((nr=read(fd,buffer,n))>0)
    {
        if(nr==n){
            printf("il %d^ carattere all'interno del file %s multiplo di %d è: %c\n",i,argv[1],n,buffer[n-1]);
            i++;
            }
        else{
            printf("Non esiste un carattere multiplo di %d",n);
        }
    }
    
    
    exit(0);
}
