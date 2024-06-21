/*file parametri1.c*/
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv){

    int i;
    int N;
    if(argc<2){
        printf("ERROR: numero di parametri non corretto\n");
        exit(1);
    }
    N=argc-1;
    printf( "eseguo il programma %s per i %d  parametti\n",argv[0],argc-1 );
    for(i=1;i<=N;i++){
        printf("il parametro alla posizione %d è %s\n",i,argv[i]);
    }
    exit(0);
}
