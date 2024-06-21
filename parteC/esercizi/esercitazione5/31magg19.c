/*file 31magg19.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef int pipe_t[2];
typedef struct 
{
    int pid_nipote;
    int lung_linea;
    char prima_linea[250];
}Struct;


int main(int argc, char const *argv[])
{
    /*variabili locali*/
    int pid;
    int pidFiglio,status,ritorno;
    int N;
    int i,j;
    pipe_t *piped;
    pipe_t p;
    Struct S;
    int nr;
    /*-------*/
    /*controllo parametri in input*/
    if(argc<4){
        printf("ERROR: %d numero di parametri passati errato\n",argc-1);
        exit(1);
    }

    N=argc-1;
    piped=(pipe_t*)malloc(N*sizeof(pipe_t));
    if(piped==NULL){
        printf("ERROR: errore nella allocazione di memoria della pipe\n");
        exit(2);
    }
    for(i=0;i<N;i++){
        if(pipe(piped[i])<0){
            printf("ERROR: errore nella creazione della pipe\n");
            exit(3);
        }
    }
    printf("DEBUG: in esecuzione il processo padre con pid %d, stanno per essere creati %d processi figlio\n",getpid(),N);
    /*ciclo per la creazione dei figli*/
    for(i=0;i<N;i++){
        if((pid=fork())<0){
            printf("ERROR: errore nella creazione della fork\n");
            exit(4);
        }
        if(pid==0){
              /*codice figlio*/
            printf("DEBUG: in esecuzione %d^ processo figlio, con pid %d\n",i+1,getpid());
            for(j=0;j<N;j++){
                close(piped[j][0]);
                if(j!=i){
                    close(piped[j][1]);
                }
            }
            if(pipe(p)<0){
                printf("ERROR: errore nella creazione della pipe tra figlio e nipote\n");
                exit(-1);
            }
            if((pid=fork())<0){
                printf("ERROR: errore nella creazione del nipote del processo figlio numero: %d\n",i+1);
                exit(-1);
            }
            if(pid==0){
                /*codice nipote*/
                printf("DEBUG: in esecuzione il processo nipote %d del figlio con pid:%d\n",getpid(),getppid());
                close(piped[i][0]);
                close(1);
                dup(p[1]);
                close(p[0]);
                close(p[1]);

                execlp("sort","sort","-f",argv[i+1],(char*)0);
                perror("errore nel processo nipote\n");
                exit(-1);
            }
            /*codice figlio*/
            close(p[1]);
            j=0;
            S.pid_nipote=pid;
            while (read(p[0],&(S.prima_linea[j]),1))
            {
             if(S.prima_linea[j]=='\n'){
                S.lung_linea=j+1;
                break;
             }  
             else{
                j++;
             } 
            }
            write(piped[i][1],&S,sizeof(S));
            if((pidFiglio=wait(&status))<0){
                printf("ERROR: errore nella wait del nipote con pid %d\n",pidFiglio);
                exit(-1);
            }
            if((status && 0xFF)!=0){
                printf("ATTENTION: processo nipote %d terminato in modo anomalo\n",pidFiglio);
            }
            else{
                printf("il processo nipote con pid %d temrinato e ritornato %d\n",pidFiglio,((status>>8) && 0xFF));
                ritorno=S.lung_linea;
            }
            exit(ritorno);
        }
   
    }   
       /*codice padre*/
    for(i=0;i<N;i++){
        close(piped[i][1]);
    }
    for (i = 0; i <N; i++)
    {
        nr=read(piped[i][0],&S,sizeof(S));
        if(nr!=0){
            S.prima_linea[S.lung_linea]='\0';
            printf("letto dal file %s la seguente riga :: %s :: con lunghezza %d catatteri\n",argv[i+1],S.prima_linea,S.lung_linea);
        }
    }
    for(i=0;i<N;i++){
        if((pidFiglio=wait(&status))<0){
            printf("ERROR: errore nella wait del figlio con pid %d\n",pidFiglio);
            exit(-1);
        }
        if((status && 0xFF)!=0){
            printf("ATTENTION: processo figlio %d terminato in modo anomalo\n",pidFiglio);
        }
        else{
            ritorno=(status>>8) && 0xFF;
            printf("il processo figlio con pid %d temrinato e ritornato %d\n",pidFiglio,ritorno);
        }
    }
    exit(0);
}
