/*file 13lug22.c code:11*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#define PERM 0644

typedef int pipe_t[2];

int main(int argc, char const *argv[])
{
    int pid;
    int pidFiglio,status,ritorno;
    int N;
    int n,i;
    int fcreato,fd;
    char car;
    char *cur;
    pipe_t *piped;
    int ncar;
     int nw;
    if(argc<3)
    {
        printf("ERROR: errore nel numero di parametri iniziali passati %d\n",argc-1);
        exit(1);
    }
    N=argc-1;
    piped=(pipe_t*)malloc(N*sizeof(pipe_t));
    if(piped==NULL)
    {
        printf("ERROR: errore nella allocazione della memoria pipe\n");
        exit(2);
    }
    for (n= 0; n <N; n++)
    {
        if ((pipe(piped[n]))<0)
        {
            printf("ERROR: errore nella creazione della pipe\n");
            exit(3);
        }
    }
     printf("DEBUG: processo padre con pid: %d creazione %d figli\n",getpid(),N);
    cur=(char*)malloc(N*sizeof(char));
    if(cur==NULL){
        printf("ERROR: errore nella allocazione della memoria cell'array cur\n");
        exit(4);
    }
    fcreato=creat("PALETTI",PERM);
    if(fcreato<0)
    {
        printf("ERROR: errore nella creazione del file 'COGNOME'\n");
        exit(5);
    }
   
    for (n= 0; n < N; n++)
    {
        if((pid=fork())<0)
        {
            printf("ERROR: errore nella fork del figlio %d\n",n);
            exit(4);
        }
        if(pid==0)
        {
            printf("DEBUG: in esecuzione il processo figlio numero %d con pid: %d\n",n,getpid());
            for(i=0;i<N;i++)
            {
                if(i!=n)
                {
                    close(piped[i][1]);
                }
                if(i==0 || i!=n-1)
                {
                    close(piped[i][0]);
                }
            }
            if((fd=open(argv[n+1],O_RDONLY))<0)
            {
                printf("ERROR: errore nella apertura del file %s\n",argv[n+1]);
                exit(-1);
            }
            printf("DEBUG:aperto file %s\n",argv[n+1]);
            ncar=0;
            while (read(fd,&car,1))
            {
                ncar++;
                if((ncar%2)==1)
                {
                    if(n!=0)
                    {
                        read(piped[n-1][0],cur,N*sizeof(char));
                        /*if(nr != N*sizeof(char))
                        {
                            printf("ERROR: errore nella lettura nel figlio %d\n",i);
                            exit(-1);
                        }*/
                    }
                    cur[n]=car;
                    nw=write(piped[n][1],cur,N*sizeof(char));
                    if (nw!= N*sizeof(char))
                    {
                        printf("ERROR: errore nella scrittura nel figlio %d\n",n);
                        exit(-1);
                    }
                    ritorno=car;
                }
            }
            exit(ritorno);
        }
    }
    
    for(i=0;i<N;i++)
    {
        close(piped[i][1]);
        if(i!=N-1)
        {
            close(piped[i][0]);
        }
    }
    while (read(piped[n-1][0],cur,N*sizeof(char)))
	{
		write(fcreato, cur, N*sizeof(char));
	}
    	/* Il padre aspetta i figli */
	for (n=0; n < N; n++)
	{
        	pidFiglio = wait(&status);
        	if (pidFiglio < 0)
        	{
                printf("Errore in wait\n");
                exit(9);
        	}
        	if ((status & 0xFF) != 0)
                	printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        	else
        	{	 
			    ritorno=(int)((status >> 8) & 0xFF);
        		printf("Il figlio con pid=%d ha ritornato %c (e in decimale %d se 255 problemi)\n", pidFiglio, ritorno, ritorno);
        	} 
	}
	exit(0);	
}
