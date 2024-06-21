/*file 25gen23.c code:c11*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
typedef int pipe_t[2];
int N;
int *finito;

int finitof(){
    for (int i = 0; i < N; i++)
    {
        if(!finito[i]){
            return 0;
        }
    }
    return 1;
}

int main(int argc, char const *argv[])
{
    /*variabili locali*/
    int pid;
    int i;
    int n;
    int fdw,fd;
    char chs[2];
    pipe_t *piped;
    int pidFiglio,status,ritorno=0;
    int nr,nw;
    if(argc<4)
    {
        printf("ERROR:errore nel numero di parametri in input %d",argc-1);
        exit(1);
    }
    N=argc-2;
    piped=(pipe_t*)malloc(N*sizeof(pipe_t));
    if(piped==NULL)
    {
        printf("ERROR: errore nella allocazione di memoria della pipe");
        exit(2);
    }
    for (i = 0; i < N; i++)
    {
        if((pipe(piped[i]))<0)
        {
            printf("ERROR: errore nella creazione della pipe");
            exit(3);
        }
    }
    finito=(int*)malloc(N*sizeof(int));
    memset(finito,0,N*sizeof(int));
    if(finito==NULL){
        printf("ERROR: errore nella allocazione dell array finito");
        exit(4);
    }
    fdw=open(argv[N+1],O_WRONLY);
    if(fdw<0)
    {
        printf("ERROR: errore nella apetrura del file %s",argv[N+1]);
        exit(5);
    }
    lseek(fdw,0L,SEEK_END);
    printf("DEBUG: in esecuzione il processo padre con pid %d andra a creare %d figlio\n",getpid(),N);
    for ( n = 0; n < N; n++)
    {
        if((pid=fork())<0)
        {
            printf("ERROR: errore nella fork del figlio %d",n);
            exit(6);
        }
        if (pid==0)
        {
            printf("DEBUG: in esecuzione il processo figlio numero %d con pid: %d andrà a leggere dal file %s\n",n,getpid(),argv[n+1]);
            /* nel caso di errore in un figlio decidiamo di ritornare -1 che non e' un valore accettabile */
            for (i = 0; i < N; i++)
            {
                close(piped[i][0]);
                if(i!=n){
                    close(piped[i][1]);
                }
            }
            if((fd=open(argv[n+1],O_RDONLY))<0)
            {
                printf("ERROR: errore nella apertura del file %s nel figlio %d",argv[n+1],n);
                exit(-1);
            }
            while ((nr=read(fd,chs,2))!=0)
            {
                nw=write(piped[n][1],chs,nr);
                if (nw!=nr)
                {
                    printf("ERROR: errore nella scrittura nel figlio %d",n);
                }
                ritorno+=nr;                
            }
            exit(ritorno);
        }
        
    }
    for (i = 0; i < N; i++)
    {
        close(piped[i][1]);
    }
    while (!finitof())
    {
        for ( i = 0; i < N; i++)
        {
        nr=read(piped[i][0],chs,2);
        if(nr==0)
        {
            finito[i]=1;
        }
        if(!finito[i])
        {
            nw=write(fdw,chs,nr);
            if(nw!=nr)
            {
                printf("ERROR: errore in writre");
            }
        }
        }
    }
    /* Il padre aspetta i figli */
for (n=0; n < N; n++)
{
        pidFiglio = wait(&status);
        if (pidFiglio < 0)
        {
                printf("Errore in wait\n");
                exit(7);
        }
        if ((status & 0xFF) != 0)
                printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else
        { 
		ritorno=(int)((status >> 8) & 0xFF);
        	printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi)\n", pidFiglio, ritorno);
        } 
}
   exit(0);
}
