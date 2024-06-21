/*file 14giu17.c*/
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
    /*variabili ocali*/
    int pid;
    int pidFiglio,status,ritorno=0;
    int i,j;
    int N;
    int ret=0;
    long int pos;
    pipe_t *pipePadreFiglio;
    pipe_t *pipeFiglioPadre;
    char ch,c,cx,scarto;
    int finito=0;
    int fd;
    int nr;
    /*----*/
    /*controllo dei parametri*/
    if (argc<3)
    {
        printf("ERROR: errore numero di parametri non corretto %d\n",argc-1);
        exit(1);   
    }
    N=argc-2;
    if (argv[argc-1][1]!='\0')
    {
        printf("ERROR: errore l'ultimo parametro non è un singolo carattere %s\n",argv[argc-1]);
        exit(2);
    }
    ch=argv[argc-1][0];
    /*inizializzazione delle pipe*/
    pipeFiglioPadre=(pipe_t*)malloc(N*sizeof(pipe_t));
    pipePadreFiglio=(pipe_t*)malloc(N*sizeof(pipe_t));
    if (pipeFiglioPadre==NULL || pipePadreFiglio==NULL)
    {
        printf("ERROR:errore nella allocazione di una pipe");
        exit(3);
    }
    for (i = 0; i < N; i++)
    {
        if (pipe(pipeFiglioPadre[i])<0)
        {
            printf("ERROR: errore nalla creazione della pipe figlio padre\n");
            exit(4);
        }
        if (pipe(pipePadreFiglio[i])<0)
        {
            printf("ERROR: errore nella crezione pipe padre figlio\n");
            exit(5);
        }
    }
    printf("DEBUG: sono il processo padre con pid: %d, andrò a creare %d porcessi figli alla ricerca del carattere %c\n",getpid(),N,ch);
    
    /*ciclo per i figli*/
    for ( i = 0; i < N; i++)
    {
        if ((pid=fork())<0)
        {
            printf("ERROR: errore nella fork del processo figlio %d\n",i);
            exit(6);
        }
        if (pid==0)
        {
            /* codice del figlio */
            printf("DEBUG: in eseguzione il processo figlio num: %d con pid %d\n",i,getpid());
            /*chiusura delle pipe non usate*/
            for ( j = 0; j<N; j++)
            {
                close(pipeFiglioPadre[j][0]);
                close(pipePadreFiglio[j][1]);
                if (j!=i)
                {
                    close(pipeFiglioPadre[j][1]);
                    close(pipePadreFiglio[j][0]);
                }
            }
            if ((fd=open(argv[i+1],O_RDWR))<0)
            {
                printf("ERROR: errore nalla apertura del file %s\n",argv[i+1]);
                exit(-1);
            }
            while (read(fd,&c,1))
            {
                if(ch==c)
                {
                    printf("DEBUG: trovato il carattere %c\n",ch);
                    pos=lseek(fd,0L,SEEK_CUR);
                    write(pipeFiglioPadre[i][1],&pos,sizeof(pos));
                    read(pipePadreFiglio[i][0],&cx,1);
                    printf("DEBUG: letto il carattere %c\n",cx);
                    if(cx!='\n')
                    {
                        lseek(fd,-1L,SEEK_CUR);
                        write(fd,&cx,1);
                        ret++;
                    }
                }  
            }
            exit(ret);
        }
    }
    /*codice padre*/
    for (i = 0; i < N; i++)
    {
        close(pipeFiglioPadre[i][1]);
        close(pipePadreFiglio[i][0]);
    }
    while(!finito)
    {
        finito=1;
        for ( i = 0; i < N; i++)
        {
            nr=read(pipeFiglioPadre[i][0],&pos,sizeof(pos));
            if (nr!=0)
            {
                finito=0;
                printf("DEBUG: nel file %s trovato il carattere in pos %ld come vuoi sostituirlo?\n",argv[i+1],pos);
                read(0,&c,1);
                if (c!='\n')
                {
                    read(0,&scarto,1);
                }
                write(pipePadreFiglio[i][1],&c,1);
            } 
        }
    }
	for (i=0; i < N; i++)
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
		{ 	ritorno=(int)((status >> 8) &	0xFF); 
		  	printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi nel figlio)\n", pidFiglio, ritorno);
		}
	}

	exit(0);

}
