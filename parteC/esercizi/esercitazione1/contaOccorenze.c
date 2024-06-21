/*file contaOccorrenze.c*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char **argv){
    long int tot=0;
    int f;
    int c;
    char cr;
    char Cx;
    if(argc!=3){
        printf("ERROR: numero di parametri errato");
        exit(1);
    }
    f=open(argv[1],O_RDONLY);
    if(f<0){
        printf("ERROR: il parametro %s non è un file",argv[1]);
        exit(2);
    }
    printf("DEBUG-OK: 1^ parametro corretto\n");
    c=strlen(argv[2]);
    if(c!=1){
        printf("ERROR: il parametro %s non è un carattere\n",argv[2]);
        exit(3);
    }
     printf("DEBUG-OK: 2^ parametro corretto\n");
    Cx=argv[2][0];
    printf("eseguo il programma %s con parametri %s %c\n",argv[0],argv[1],Cx);
    while (read(f,&cr,1))
    {
        if(cr==Cx){
            tot++;
        }
    }
    printf("il numero di occorrenze del carattere %c nel file %s è: %ld\n",Cx,argv[1],tot);
    exit(0);
}