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

int main(int argc, char const *argv[])
{
    /*variabili locali*/
    int pid;
    int pidFiglio, status,ritorno;
    int Q;
    int q,i;
    int L;
    int fd;
    char ch;
    int occ=0;
    char ok;
    int nr;
    pipe_t *piped;
    
    /*---------*/  
    if(argc<5)
    {
        printf("ERROR: errore nel numero di parametri in input %d\n",argc-1);
        exit(1);
    }  
    L=(int)atoi(argv[2]);
    if(L<=0)
    {
        printf("ERROR: numero di righe minore o uguale a zero\n");
        exit(2);
    }
    Q=argc-3;
    for ( i = 0; i < Q; i++)
    {
       if (argv[3+i][1]!='\0')
       {
        printf("ERROR: errore il parametro %s non è un carattere\n",argv[3+i]);
        exit(3);
       }
    }
    /*allocazione pipe*/
    piped=(pipe_t*)malloc((Q+1)*sizeof(pipe_t));
    if (piped==NULL)
    {
        printf("ERROR: errore nella allocazione della pipe\n");
        exit(4);
    }
    for ( q = 0;q < Q+1; q++)
    {
        if (pipe(piped[q])<0)
        {
            printf("ERROR: errore nella creazione della pipe\n");
            exit(5);
        }
    }
    printf("DEBUG: sono il processo padre con pid: %d e sto andando a creare %d figli\n ",getpid(),Q);
    
    for ( q = 0; q < Q; q++)
    {
        if ((pid=fork())<0)
        {
            printf("ERROR: errore nella fork\n");
            exit(6);
        }
        if(pid==0)
        {
            for (i = 0; i < Q+1; i++)
            {
                if (i!=q)
                {
                    close(piped[i][0]);
                }
                if (i!=q+1)
                {
                    close(piped[i][1]);
                }
            }
            if ((fd=open(argv[1],O_RDONLY))<0)
            {
                printf("ERROR: errore nella apertura del file %s\n",argv[1]);
                exit(-1);
            }
            while (read(fd,&ch,1))
            {
                if(ch=='\n')
                {
                    nr=read(piped[q][0],&ok,sizeof(char));
                    if (nr!=sizeof(char))
                    {
                        printf("ERROR: numero di byte letti sbagliato\n");
                        exit(-1);
                    }
                    printf("Figlio con indice %d e pid %d ha letto %d caratteri %c nella linea corrente\n", q, getpid(), occ, argv[q+3][0]);
                    nr=write(piped[q+1][1],&ok,sizeof(char));
                    if (nr!=sizeof(char))
                    {
                        printf("ERROR: numero di byte letti sbagliato\n");
                        exit(-1);
                    }
                    ritorno=occ;
                    occ=0;
                }
                else{
                    if (ch==argv[3+q][0])
                    {
                        occ++;
                    }
                    
                }
            }
        exit(ritorno); 
        }
    }
    for ( q = 0; q < Q+1; q++)
    {
        if(q!=0)
        {
            close(piped[q][1]);
        }
        if(q!=Q)
        {
            close(piped[q][0]);
        }
    }
    for (i = 0; i <L; i++)
    {
        printf("DEBUG: linea %d del file %s \n",i+1,argv[1]);
        nr=write(piped[0][1],&ok,sizeof(char));
        if (nr!=sizeof(char))
        {
            printf("ERROR: numero di byte scritti errato\n");
            exit(12);
        }
        nr=read(piped[Q][0],&ok,sizeof(char));
        if(nr!=sizeof(char))
        {
            printf("ERROR: numero di byte letti errato\n");
        }
        
    }    
	/* Il padre aspetta i figli */
	for (q=0; q < Q; q++)
	{
        	if ((pidFiglio = wait(&status)) < 0)
        	{
                	printf("Errore in wait\n");
                	exit(8);
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
