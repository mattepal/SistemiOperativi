/*file 7giu23.c code:c11*/
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
       if(!finito[i])
       {
        return 0;
       }
    }
    return 1;
}

int main(int argc, char const *argv[])
{
    /*variabili locali*/
    int pid;
    int n,i;
    int pidFiglio,status,ritorno;
    pipe_t *pipeFP;
    pipe_t *pipePF;
    int fd;
    char linea[250];
    char car;
    int nw;
    int nr;
    char chmax;
    int nlinea=0;
    int stampa=0;
    int indice;
    char ok;
    /*----------------*/
    if(argc<3)
    {
        printf("ERROR: numero di parametri in input errato %d",argc-1);
        exit(1);
    }
    N=argc-1;
    pipePF=(pipe_t*)malloc(N*sizeof(pipe_t));
    pipeFP=(pipe_t*)malloc(N*sizeof(pipe_t));
    if (pipeFP==NULL || pipePF==NULL)
    {
        printf("ERROR: errore nella allocazione delle pipe");
        exit(2);
    }
    for ( n = 0; n < N; n++)
    {
        if(pipe(pipePF[n])<0)
        {
            printf("ERROR: errore nella creazione della pipePF");
            exit(3);
        }
        if(pipe(pipeFP[n])<0){
            printf("ERROR: errore nella creazione della pipeFP");
            exit(4);
        }
    }
    finito=(int*)malloc(N*sizeof(int));
    if(finito==NULL)
    {
        printf("ERROR: errore nella allocazione dell'array finito");
        exit(5);
    }
    memset(finito, 0, N*sizeof(int));
    printf("DEBUG: sono il processo padre con pid %d e andrò a creare %d figli\n",getpid(),N);
    for (n = 0; n < N; n++)
    {
        if((pid=fork())<0)
        {
            printf("ERROR: errore nella fork del figlio %d \n",n);
            exit(6);
        }
        if (pid==0)
        {
            /* code figlio*/
            printf("DEBUG: in esecuzione il processo figlio n%d con pid %d \n",n,getpid());
            for (i = 0; i < N; i++)
            {
                close(pipePF[i][1]);
                close(pipeFP[i][0]);
                if (i!=n)
                {
                    close(pipePF[i][0]);
                    close(pipeFP[i][1]);
                }
            }
            if ((fd=open(argv[n+1],O_RDONLY))<0)
            {
                printf("ERROR: errore nella apertura del file %s",argv[n+1]);
                exit(-1);
            }
            i=0;
            stampa=0;
            while (read(fd,&linea[i],1))
            {
                if (linea[i]=='\n')
                {
                    nlinea++;
                    nw=write(pipeFP[n][1],&linea[i-1],1);
                    if(nw!=1)
                    {
                        printf("ERROR: errore nella scrittura su pipe dell'ultimo carattere");
                        exit(-1);
                    }
                    nr=read(pipePF[n][0],&ok,1);
                    if(nr!=1)
                    {
                        printf("ERROR: errore nella lettura dell ok da parte del padre");
                        exit(-1);
                    }
                    if (ok=='v')
                    {
                        linea[i+1]='\0';
                        printf("DEBUG: il processo figlio %d con pid %d ha identificato il carattere %c nella linea %d ecco la linea corrente %s",n,getpid(),linea[i-2],nlinea,linea);
                        stampa++;
                    }
                    i=0;  
                }
                else
                {
                    i++;
                }  
            }  
            exit(stampa);    
        }
    }
    for (i = 0; i < N; i++)
    {
        close(pipePF[i][0]);
        close(pipeFP[i][1]);
    }
    while (!finitof())
    {
        chmax=-1;
        for (i = 0; i < N; i++)
        {
            finito[i]=(read(pipeFP[i][0],&car,1)!=1);
            if (!finito[i])
            {
                if(car>chmax)
                {
                    chmax=car;
                    indice=i;
                }
            }
        }
        for ( n = 0; n< N; n++)
        {
            if (n==indice)
            {
                ok='v';
            }
            else
            {
                ok='f';
            }
            if (!finito[n])
            {
                nw=write(pipePF[n][1],&ok,1);
                if(nw!=1)
                {
                    printf("ERROR: errore nella scrittura da parte del padre");
                }
            }
        }
    }
    
for(n=0; n < N; n++)
{
   pidFiglio = wait(&status);
   if (pidFiglio < 0)
   {
      printf("Errore wait\n");
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
