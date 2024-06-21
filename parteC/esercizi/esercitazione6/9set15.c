/*file 9set15.c code:c11*/
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
int i=0;

void handler()
{
	printf("DEBUG-Sono il figlio %d di indice %d e ho finito di leggere il mio file\n", getpid(), i); 
	exit(0); /* il figlio termina in modo normale */
}

int main(int argc, char const *argv[])
{
    /*variabili locali*/
    int *pid; /*variabile usata per salvare */
    int N; /*variabile che salva il numero di file*/
    int pidFiglio,status,ritorno=0; /*variabili per il ritorno e salvataggio dei vari processi*/
    int *confronto;
    pipe_t *pipeFiglioPadre;		/* array di pipe di comunicazione fra figli e padre */
    pipe_t *pipePadreFiglio;		/* array di pipe di comunicazione/sincronizzazione fra padre e figlio. NOTA BENE: questa sincronizzazione potrebbe essere fatta tramite l'invio di segnali da parte del padre ai figli */
    int fd;
    int j;
    char token='v';
    char c,ch;

    /*----*/
    /*controllo del numero di parametri*/
    if (argc<4)
    {
        printf("ERROR: errore nel numero di parametri passati%d\n",argc-1);
        exit(1);
    }
    N=argc-2;

    /*allocazione pid array*/
    if ((pid=(int*)malloc(N*sizeof(int)))==NULL)
    {
        printf("ERROR: errore nella allocazione dei pid\n");
        exit(2);
    }
    /*allocazione array confronto*/
    if ((confronto=(int*)malloc(N*sizeof(int)))==NULL)
    {
        printf("ERROR: errore nalla allocazione del array confronto\n");
        exit(3);
    }
    /* inizializzazione di confronto: tutti i valori a 1 perche' all'inizio si deve mandare l'indicazione al figlio di leggere! */
    for (i = 0; i < N; i++)
    {
        confronto[i]=1;
    }
    /* allocazione pipe figli-padre */    
    if ((pipeFiglioPadre=(pipe_t*)malloc(N*sizeof(pipe_t)))==NULL)
    {
        printf("ERROR: errore nella allocazione della pipeFiglioPadre\n");
        exit(4);
    }

    if((pipePadreFiglio=(pipe_t*)malloc(N*sizeof(pipe_t)))==NULL)
    {
        exit(5);
    }  
    for ( i = 0; i < N; i++)
    {
        if (pipe(pipeFiglioPadre[i])<0)
        {
            printf("ERROR: errore nella creazione pipefilgiopadre\n");
            exit(6);
        }
        if(pipe(pipePadreFiglio[i])<0)
        {
            printf("ERROR: errore nella creazione pipepadrefiglio\n");
            exit(7);
        }
    }
    signal(SIGUSR1,handler);
    for (i = 0; i < N; i++)
    {
        if ((pid[i]=fork())<0)
        {
            printf("ERROR: errore nella fork del processo figlio n %d\n",i);
            exit(-1);
        }
        if(pid[i]==0)
        {
           printf("DEBUG: in esecuzione il processo figlio %d con pid %d\n",i,getpid());
           for (j = 0; j < N; j++)
           {
                close(pipeFiglioPadre[j][0]);
                close(pipePadreFiglio[j][1]);
                if (i!=j)
                {
                    close(pipeFiglioPadre[j][1]);
                    close(pipePadreFiglio[j][0]);
                }
           }
            /*apertura del file*/
            if((fd=open(argv[i+1],O_RDONLY))<0)
            {
                printf("ERROR: errore nella apertura del file %s\n",argv[i+1]);
                exit(-1);
            }
            while (read(pipePadreFiglio[i][0],&token,1))
            {
                read(fd,&c,1);
                write(pipeFiglioPadre[i][1],&c,1);
            }
            exit(0);           
        }
    }
    /*chiusura pipe*/
    for ( i = 0; i <N; i++)
    {
        close(pipeFiglioPadre[i][1]);
        close(pipePadreFiglio[i][0]);
    }
    if ((fd=open(argv[argc-1],O_RDONLY))<0)
    {
        printf("ERROR:errore nella apertura del file %s\n",argv[argc-1]);
        exit(9);
    }
    
    while (read(fd,&ch,1))
    {
        for ( i = 0; i < N; i++)
        {
            if (confronto[i])
            {
                write(pipePadreFiglio[i][1],&token,1);
                read(pipeFiglioPadre[i][0],&c,1);
                if (ch!=c)
                {
                    confronto[i]=0;
                }
            }
            
        }
        
    }
    for(i=0;i<N;i++)
    {
	    if (!confronto[i])
		{
        	if (kill(pid[i], SIGKILL) == -1) 
               		printf("Figlio con pid %d non esiste e quindi e' gia' terminato\n", pid[i]);;
    	}	
 		else 
		{
			kill (pid[i], SIGUSR1);
 		}
    }
    for (i = 0; i < N; i++)
    {
        if ((pidFiglio=wait(&status))<0)
        {
            printf("ERROR: errore nella wait del figlio n:%d\n ",i);
            exit(7);
        }
        if((status & 0xFF)!=0)
        {
            printf("ERROR: processo figlio temrinato in modo anomalo\n");
        }
        else
        {
            ritorno=(int)((status>>8) & 0xFF);
            printf("DUBUG: il processo figlio %d ha ritornoato %d (se 255 ci sono errori)\n",i,ritorno);
            for (j = 0; j < N; j++)
            {
                if (pid[j]==pidFiglio)
                {
                    printf("DEBUG: il processo figlio %d ha confrontato i seguenti file %s %s\n",i,argv[i+1],argv[argc-1]);
                }  
            }  
        }
    }   
    exit(0);
}
