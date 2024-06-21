/*file giu21.c*/
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
typedef char lin[250];

int main(int argc, char **argv) {
    /*variabili locali*/
    int pid; /* process identifier per le fork() */
    int N; /* numero di file passati sulla riga di comando (uguale al numero di file) */
    int n,i; /* indici per i cicli */
    lin linea;
    lin *tutteLinee;
    int pidFiglio,status,ritorno; /* per valore di ritorno dei vari processi (figli/nipoti) */
    pipe_t *piped; /* array dinamico di pipe descriptors per comunicazioni figli-padre  */
    int fdout,fd; /*file description per il file creato,e file passati*/
    int nr,nw; /* variabili per salvare valori di ritorno di read/write da/su pipe */
    if (argc<3)
    {
        /* code */
        printf("ERROR: errore nel numero di variabili iniziali");
        exit(1);
    }
    N=argc-1;
    printf("DEBUG: processo padre con pid: %d creazione di %d processi",getpid(),N);
    /* Creazione di N pipe */
    if ((piped = (pipe_t *) malloc(N * sizeof(pipe_t))) == NULL) {
        printf("Errore allocazione pipe\n");
        exit(2);
    }
    for (i = 0; i < N; i++) {
        if ((pipe(piped[i])) < 0) {
            printf("Errore creazione pipe\n");
            exit(3);
        }
    }
    /* creazione file PALETTI*/
    if ((fdout=creat("PALETTI",0644)) < 0) {
        printf("Errore nella creazione del file %s\n", "PALETTI");
        exit(4);
    }
    tutteLinee=(lin*)malloc(N*sizeof(lin));
    if (tutteLinee==NULL)
    {
        printf("ERROR: errore nella creazione dell'array tutttelinee");
        exit(5);
    }
    
    for (n = 0; n < N; n++)
    {
        if ((pid = fork()) < 0) {
            /* Fork fallita */
            printf("Errore in fork\n");
            exit(6);
        }
        
        if (pid == 0) {
            /* CODICE DEL FIGLIO*/
            printf("Figlio, pid: %d, indice %d\n", getpid(), n);
            for (i = 0; i < N; i++)
            {
                if(i!=n)
                {
                    close(piped[i][0]);
                }
                if (i!=(n+1)%N)
                {
                    close(piped[i][1]);
                }
            }
            /* apertura file */
            if ((fd = open(argv[n+1], O_RDONLY)) < 0) {
                printf("Impossibile aprire il file %s\n", argv[1]);
                exit(-1);
            }
            /* leggi tutto il file */
            i=0;
            while (read(fd, &(linea[i]), 1)!=0) { 
                if (linea[i]=='\n')
                {
                    nr=read(piped[n][0],tutteLinee,N*sizeof(lin));
                    if (nr!=N*sizeof(lin))
                    {
                        printf("ERROR: errore nella lettura nel figlio %d, %d\n",n,nr);
                        exit(-1);
                    }
                    for ( int j = 0; j <= i;j++)
                    {
                        tutteLinee[n][j]=linea[j];
                    }
                    
                    nw=write(piped[(n+1)%N][1],tutteLinee,N*sizeof(lin));
                    if (nw!=N*sizeof(lin))
                    {
                        printf("ERROR: errore nella scrittura nel figlio %d, %d\n",n,nw);
                    }
                    if (n==N-1)
                    {
                        for ( int j = 0; j < N; j++)
                        {                         
                            for ( int k = 0; k < 250; k++)
                            {
                                printf("%c\n",tutteLinee[j][k]);
                                write(fdout,&(tutteLinee[j][k]),1);
                                if (tutteLinee[j][k]=='\n')
                                {
                                    break;
                                }
                            }  
                        }

                    } 
                    ritorno=i+1;
                    i=0;
                }
                else
                {
                    i++;
                } 
            }
            printf("%d\n",ritorno);
            exit(ritorno);
            /* FINE CODICE DEL FIGLIO */
        }
        printf("Generato figlio con PID = %d\n", pid);
    }
    for ( i = 0; i < N; i++)
    {
        if(i!=0){
            close(piped[i][1]);
            close(piped[i][0]);
        }
    }
    nw=write(piped[0][1],tutteLinee,N*sizeof(lin));
    if(nw!=N*sizeof(lin))
    {
        printf("ERROR: errore nella write del padre");
        exit(7);
    }
    close(piped[0][1]);
    /* Il padre aspetta i figli salvando lo status */
    for (int i = 0; i < N; i++) {
        if ((pidFiglio = wait(&status)) < 0) {
            printf("Errore in wait\n");
            exit(8);
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
        


