/*file 14giu17.c code:c11*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef int pipe_t[2];

int main(int argc, char const *argv[])
{
    int pid;
    int i,j;
    int N;
    int fd;
    int nr;
    char cx;
    char scarto;
    int finito;
    char ch;
    long int pos;
    int ritorno=0;
    pipe_t *pipedPF;
    pipe_t *pipedFP;
    int status;

    if(argc<3)
    {
        printf("ERROR: numero di  parametri errato");
        exit(1);
    }
    N=argc-2;
    if(argv[argc-1][1]!='\0')
    {
        printf("ERROR: errore il parametro %s non è un singolo carattere\n",argv[argc-1]);
        exit(2);
    }
    cx=argv[argc-1][0];
    printf("DEBUG: in esecuzione il proceso padre con pid %d che va a creare %d procesi figli cercando il carattere %c\n",getpid(),N,cx);
    pipedFP=(pipe_t*)malloc(N*sizeof(pipe_t));
    pipedPF=(pipe_t*)malloc(N*sizeof(pipe_t));
    if(pipedFP==NULL || pipedPF==NULL)
    {
        printf("ERROR: errore nella allocazione della memoria per la pipe\n");
        exit(3);
    }
    for ( i = 0; i < N; i++)
    {
        if(pipe(pipedPF[i])<0)
        {
            printf("ERROR: errore nella creazione della pipe padre figlio\n");
            exit(4);
        }
        if(pipe(pipedFP[i])<0){
            printf("ERROR: errore nella creazione della pipe figlio padre\n");
            exit(5);
        }
    }

    for ( i = 0; i < N; i++)
    {
        if((pid=fork())<0)
        {
            printf("ERROR: errore nella creazione del processo figlio numero %d\n",i+1);
            exit(-1);
        }
        if(pid==0)
        {
            printf("DEBUG: in esecuzione il processo figlio %d con pid:%d\n",i+1,getpid());
            for(j=0;j<N;j++)
            {
                close(pipedFP[j][0]);
                close(pipedPF[j][1]);
                if (j!=i)
                {
                    close(pipedPF[j][0]);
                    close(pipedFP[j][1]);
                } 
            }
            if ((fd=open(argv[i+1],O_RDWR))<0)
            {
                printf("ERROR: errore nell'apertura del file %s \n",argv[i+1]);
                exit(-1);
            }
            while (read(fd,&ch,1))
            {
                if(cx==ch)
                {
                    pos=lseek(fd,0L,SEEK_CUR);
                    write(pipedFP[i][1],&pos,sizeof(pos));
                    read(pipedPF[i][0],&cx,1);
                    printf("DUBUG: letto il carattere %c\n",cx);
                    if(cx!='\n')
                    {
                        lseek(fd, -1L, SEEK_CUR);
					    write(fd, &cx, 1);
                        ritorno++;
                    }
                }
            }
            exit(ritorno);
        }
    }
    	for (i=0; i < N; i++)
 	{
		close(pipedFP[i][1]);
		close(pipedPF[i][0]);
        }

/* Il padre recupera le informazioni dai figli: prima in ordine di posizioni e quindi in ordine di indice */
 	finito = 0; /* all'inizio supponiamo che non sia finito nessun figlio */
        while (!finito)
        {
		finito = 1; /* mettiamo finito a 1 perche' potrebbero essere finiti tutti i figli */
                for (i=0; i<N; i++)
                {
                 /* si legge la posizione inviata  dal figlio i-esimo */
                	nr = read(pipedFP[i][0], &pos, sizeof(pos));
			if (nr != 0)
                        { 
				finito = 0; /* almeno un processo figlio non e' finito */
                		printf("Il figlio di indice %d ha letto dal file %s nella posizione %ld il carattere %c. Se vuoi sostituirlo, fornisci il carattere altrimenti una linea vuota?\n", i,  argv[i+1], pos, cx);
        			read(0, &cx, 1);
				if (cx != '\n') read(0, &scarto, 1); /* se e' stato letto un carattere, bisogna fare una lettura a vuoto per eliminare il carattere corrispondente all'invio */
				write(pipedPF[i][1], &cx, 1);  /* inviamo comunque al figlio */

                        }
                }
         }

	/* Il padre aspetta i figli */
	for (i=0; i < N; i++)
	{
		pid = wait(&status);
		if (pid < 0)
		{
			printf("Errore in wait\n");
			exit(7);
		}
		if ((status & 0xFF) != 0)
    			printf("Figlio con pid %d terminato in modo anomalo\n", pid);
    		else
		{ 	ritorno=(int)((status >> 8) &	0xFF); 
		  	printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi nel figlio)\n", pid, ritorno);
		}
	}
    exit(0);
}
