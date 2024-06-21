/*file 1giu23.c code:c11*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#define MSGSIZE 3
typedef int pipe_t[2];



int main(int argc, char **argv) {
    /*variabili locali*/
    int pid;
    int N;
    int n,i,j;
    char linea[MSGSIZE];
    char buffer[MSGSIZE];
    pipe_t *piped;
    int fd;
    int pidFiglio,status,ritorno;
    /*--------------*/
    if(argc<4)
    {
        printf("ERROR: errore nel numero di paraemtri di input %d",argc-1);
        exit(1);
    }
    N=argc-1;
    /* Creazione di N pipe */
    if ((piped = (pipe_t *) malloc((N-1) * sizeof(pipe_t))) == NULL) {
        printf("Errore allocazione pipe\n");
        exit(2);
    }
    for (i = 0; i < (N-1); i++) {
        if (pipe(piped[i]) < 0) {
            printf("Errore creazione pipe\n");
            exit(3);
        }
    }
    for (n = 0; n < N; n++)
    {
        if ((pid = fork()) < 0) {
            /* Fork fallita */
            printf("Errore in fork\n");
            exit(4);
        }
        
        if (pid == 0) {
            /* CODICE DEL FIGLIO*/
        
            printf("Figlio, pid: %d, indice %d\n", getpid(), n);
            if(n==0)
            {
                for (i = 0; i < (N-1); i++)
                {
                    close(piped[i][0]);
                }
            }
            else{
                for ( i = 0; i < (N-1); i++)
                {
                    close(piped[i][1]);
                    if(i!=(n-1))
                    {
                        close(piped[i][0]);
                    }
                }
            }
            if((fd=open(argv[n+1],O_RDONLY))<0)
            {
                printf("EEROR: errore nela apertura del file %s\n",argv[n+1]);
                exit(-1);
            }
            if(n==0)
            {
                while (read(fd,linea, MSGSIZE))
                {
                    linea[MSGSIZE-1]='\0';
                    for ( j = 0; j < N-1; j++)
                    {
                        write(piped[j][1],linea,MSGSIZE);
                    }   
                } 
            } 
            else
            {
                while (read(piped[n-1][0],buffer,MSGSIZE))
                {
                    while (read(fd,linea,MSGSIZE))
                    {
                        linea[MSGSIZE-1]='\0';
                    
                        if ((pid = fork()) < 0) {
                        /* Fork fallita */
                            printf("Errore in fork\n");
                            exit(-1);
                        }

                        if (pid == 0) {
                           /* CODICE DEL FIGLIO*/
                            printf("nipote, pid: %d, indice %d\n", getpid(), i);
                            close(1);
                            open("/dev/null",O_WRONLY);
                            close(2);
                            open("/dev/null",O_WRONLY);
                            execlp("diff","diff",buffer,linea,(char *)0);
                            exit(-1);
                        /* FINE CODICE DEL FIGLIO */
                        }
                        printf("Generato nipote con PID = %d\n", pid);
                    /* Il padre aspetta i figli salvando lo status */
                    if ((pidFiglio = wait(&status)) < 0) {
                        printf("Errore in wait\n");
                        exit(-1);
                    }
                    if ((status & 0xFF) != 0){   
                        printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
                        exit(-1);
                        }
                    else 
                    {
                        ritorno = (int)((status >> 8) & 0xFF);
                        if(ritorno==0){
                            printf("DEBUG: i fle %s e %s sono uguali\n",buffer,linea);
                        }
                        else{
                        printf("DEBUG: i file %s e %s sono diversi\n",buffer,linea);
                        }
                        
                    }
                }
                lseek(fd,0L,0);
            }
        } 
            exit(n);    
            /* FINE CODICE DEL FIGLIO */
        }
        printf("Generato figlio con PID = %d\n", pid);
    }
    for ( i = 0; i < N-1; i++)
    {
        close(piped[i][0]);
        close(piped[i][1]);
    }
    /* Il padre aspetta i figli salvando lo status */
    for (int i = 0; i < N; i++) {
        if ((pidFiglio = wait(&status)) < 0) {
            printf("Errore in wait\n");
            exit(5);
        }
        if ((status & 0xFF) != 0)
            printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else {
            ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato il carattere %c (in decimale %d, se 255 problemi)\n", pidFiglio, ritorno, ritorno);
        }
    }    
    
    exit(0);
}
