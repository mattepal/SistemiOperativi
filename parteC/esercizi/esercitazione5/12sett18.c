#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>

typedef int pipe_t[2];

int main(int argc, char const *argv[])
{
    int pid;
    int N;
    long int trasformazioni=0L;
    int i,j;
    pipe_t *pipedPF;
    pipe_t *pipedPN;
    char ch;
    int fd;
    int status;
    int ritorno;
    if(argc<3){
        printf("ERROR: %d numero di parametri passato non corretto\n",argc-1);
        exit(1);
    }
    N=argc-1;
    pipedPF=(pipe_t*)malloc(N*sizeof(pipe_t));
    pipedPN=(pipe_t*)malloc(N*sizeof(pipe_t));
    if(pipedPF==NULL || pipedPN==NULL)
    {
        printf("ERROR: errore  nella allocazione della memeoria della pipe\n");
        exit(2);
    }
    for (i = 0; i < N; i++)
    {
        if(pipe(pipedPF[i])<0)
        {
            printf("ERROR:errore nella creazione della pipe tra padre e figlio\n");
            exit(3);
        }
        if(pipe(pipedPN[i])<0){
            printf("ERROR: errore nella creazione della pipe tra padre e nipote\n");
            exit(4);
        }
    }
    printf("DEBUG: in esecuzione il processo padre con pid:%d \n",getpid());
    for ( i = 0; i < N; i++)
    {
        if((pid=fork())<0)
        {
            printf("ERROR: errore nella fork del porcesso figlio\n");
            exit(5);
        }
        if (pid==0)
        {
            printf("DEBUG: in esecuzione il processo figlio num:%d con pid:%d\n",i,getpid());
            for ( j = 0; j < N; j++)
            {
               close(pipedPF[j][0]);
               if(j!=i)
               {
                close(pipedPF[j][1]);
               }
            }
            if((pid=fork())<0)
            {
                printf("EORRO: errore nella fork del nipote n:%d\n",i);
                exit(-1);
            }
            if (pid==0)
            {
                printf("DEBUG: in esecuzione il processo nipote con pid:%d numero.%d\n",getpid(),i);
                close(pipedPF[i][1]);
                for ( j = 0; j < N; j++)
                {
                    close(pipedPN[j][0]);
                    if (j!=i)
                    {
                        close(pipedPN[j][1]);
                    }
                }
                if ((fd=open(argv[i+1],O_RDWR))<0)
                {
                    printf("ERROR: errore nella apertura del file %s\n",argv[i+1]);
                    exit(-1);
                }
                while (read(fd,&ch,1))
                {
                    if (islower(ch))
                    {
                        lseek(fd, -1L, SEEK_CUR);
                        ch = ch - 32;
                        write(fd,&ch,1);
                        trasformazioni++;
                        printf("a %ld",trasformazioni);
                    } 
                }
                write(pipedPN[i][1],&trasformazioni,sizeof(int));
                ritorno=trasformazioni/256;
                exit(ritorno);
            }
            for (j = 0; j < N; j++)
            {
                close(pipedPN[j][0]);
                close(pipedPN[j][1]);
            }
            if ((fd=open(argv[i+1],O_RDWR))<0)
            {
                printf("ERROR: errore nella apertura del file %s\n",argv[i+1]);
                exit(-1);
            }
            while (read(fd,&ch,1))
            {
                if(isdigit(ch))
                {
                    lseek(fd,-1L,SEEK_CUR);
                    ch=' ';
                    write(fd,&ch,1);
                    trasformazioni++;
                }
            }
            write(pipedPF[i][1],&trasformazioni,sizeof(trasformazioni));
            if ((pid = wait(&status)) < 0)
			{	
				printf("Errore in wait\n");
			}
			if ((status & 0xFF) != 0)
			{	
    			printf("Nipote con pid %d terminato in modo anomalo\n", pid);
			}
    		else
				printf("Il nipote con pid=%d ha ritornato %d\n", pid, ritorno=(int)((status >> 8) & 0xFF));
			ritorno=trasformazioni/256;	
			exit(ritorno); 
        }
    }
    for (j = 0; j < N; j++)
    {
        close(pipedPF[j][1]);
        close(pipedPN[j][1]);
    }
    for (i = 0; i < N; i++)
    {
        read(pipedPF[i][0],&trasformazioni,sizeof(trasformazioni));
        printf("DEBUG: il processo figlio %d ha effettuato %ld trasformazini\n",i,trasformazioni);
        read(pipedPN[i][0],&trasformazioni,sizeof(trasformazioni));
        printf("DEBUG: il processo nipote %d ha effettuato %ld trasformazioni\n",i,trasformazioni);
    }
    
    for (i=0; i < N; i++)
	{
		if ((pid = wait(&status)) < 0)
		{
			printf("Errore in wait\n");
			exit(7);
		}

		if ((status & 0xFF) != 0)
    			printf("Figlio con pid %d terminato in modo anomalo\n", pid);
    		else
		{ 
			ritorno=(int)((status >> 8) &	0xFF); 
		  	if (ritorno==255)
 				printf("Il figlio con pid=%d ha ritornato %d e quindi vuole dire che ci sono stati dei problemi\n", pid, ritorno);
		  	else  	printf("Il figlio con pid=%d ha ritornato %d\n", pid, ritorno);
		}
	}

    exit(0);
}
