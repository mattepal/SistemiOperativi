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
#define PERM 0644
typedef int pipe_t[2];
typedef char lin[250];

int main(int argc, char **argv) {
    /*variabili locali*/
    int linean=1;
    int pid; /* process identifier per le fork() */
    int N; /* numero di file passati sulla riga di comando (uguale al numero di file) */
    int n,i,j,k; /* indici per i cicli */
    int nrolinee;
    lin linea;
    lin *tutteLinee;
    int pidFiglio,status,ritorno; /* per valore di ritorno dei vari processi (figli/nipoti) */
    pipe_t *piped; /* array dinamico di pipe descriptors per comunicazioni figli-padre  */
    int fdout,fd; /*file description per il file creato,e file passati*/
    int nr,nw; /* variabili per salvare valori di ritorno di read/write da/su pipe */
    if (argc<4)
    {
        /* code */
        printf("ERROR: errore nel numero di variabili iniziali");
        exit(1);
    }
    N=argc-2;
    nrolinee=atoi(argv[argc-1]);
    if(nrolinee<=0)
    {
        printf("ERROR: errore nel numero di linee del file");
        exit(2);
    }
    printf("DEBUG: processo padre con pid: %d creazione di %d processi\n",getpid(),N);
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
    if ((fdout=creat("PALETTI",PERM)) < 0) {
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
            for (j = 0; j < N; j++)
            {
                if(j!=n)
                {
                    close(piped[j][1]);
                }
                if (j!=(n-1) || n==0)
                {
                    close(piped[j][0]);
                }
            }
            /* apertura file */
            if ((fd = open(argv[n+1], O_RDONLY)) < 0) {
                printf("Impossibile aprire il file %s\n", argv[1]);
                exit(-1);
            }
            /* leggi tutto il file */
            j=0;
            while (read(fd, &(linea[j]), 1)!=0) { 
                if (linea[j]=='\n')
                {
                    printf("linea %d\n",linean);
                    linean++;
                    if (n!=0)
                    {
                        nr=read(piped[n-1][0],tutteLinee,N*sizeof(lin));
                        if (nr!=N*sizeof(lin))
                        {
                            printf("ERROR: errore nella lettura nel figlio %d, %d\n",n,nr);
                            exit(-1);
                        }
                    }

                    for ( k= 0; k<=j;k++)
                    {
                        tutteLinee[n][k]=linea[k];
                    }
                    
                    nw=write(piped[n][1],tutteLinee,N*sizeof(lin));
                    if (nw!=N*sizeof(lin))
                    {
                        printf("ERROR: errore nella scrittura nel figlio %d, %d\n",n,nw);
                    }
                    ritorno=j+1;
                    j=0;
                }
                else
                {
                    j++;
                } 
            }
            exit(ritorno);
            /* FINE CODICE DEL FIGLIO */
        }
        printf("Generato figlio con PID = %d\n", pid);
    }
    for ( n = 0; n < N; n++)
    {
        close(piped[n][1]);
        if(n!=N-1)
        {
            close(piped[n][0]);
        }
    }
    for (j = 1; j <= nrolinee; j++){
        nr=read(piped[N-1][0],tutteLinee,N*sizeof(lin));
        if (nr != N*sizeof(lin))
        {
            printf("ERROR: errore nella lettura del padre %d\n",nr);
            exit(10);
        }
        for ( i = 0; i < N; i++)
        {
            for (k= 0; k < 250; k++)
            {
                write(fdout,&(tutteLinee[i][k]),1);
                if (tutteLinee[i][k]=='\n')
                {
                    break;
                }
                
            }
            
        }
        
    }    
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
        


