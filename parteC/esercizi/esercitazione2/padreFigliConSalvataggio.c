/*file padreFigliConSalvataggio.c*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int random(int n){
    int casual;
    casual=rand()%n;
    return casual;
}
int main(int argc,char **argv){
    int *pid;
    int pidFiglio;
    int status;
    int ret;
    if(argc!=2){
        printf("ERROR: numero di parametri non corretto");
        exit(1);
    }
    int N;
    N=atoi(argv[1]);
    if(N<=0 || N>155){
        printf("ERROR: il parametro passato non è un numero intero positivo minore di 155");
        exit(2);
    }
    printf("in esecuzione il processo padre con pid:%d, il parametro passato è %d\n",getpid(),N);
    if((pid=(int*)malloc(N+sizeof(int)))==NULL){
        printf("ERROR: errore nella creazione della malloc");
        exit(3);
    }
    srand(time(NULL));
    for(int i=0;i<N;i++){
        if((pid[i]=fork())<0){
            printf("ERROR: errore nella creazione del processo figlio");
            exit(5);
        }
        if(pid[i]==0){
            printf("Debug:creato processo figlio %d con indice %d\n",getpid(),i+1);
            int r=random(101+i);
            exit(r);
        }
    }
    for(int i=0;i<N;i++){
        if((pidFiglio=wait(&status))<0){
            printf("ERROR: errore nella wait");
            exit(4);
        }
        if((status & 0xFF)!=0){
            printf("ERROR: figlio terminato in modo anomalo");
        }
        else{
            ret=(int)((status>>8)& 0xFF);
            for(int j=0;j<N;j++){
                if(pid[j]==pidFiglio){
                    printf("il processo figlio con pid %d ha ritornato %d\n",pidFiglio,ret);
                    break;
                }
            }
        }
    }
    exit(0);
}