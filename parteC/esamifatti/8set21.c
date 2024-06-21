#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define PERM 0644
typedef int pipe_t[2];
typedef char lin[250];


int main(int argc, char **argv) {
    /*variabili locali*/
    int pid;
    int N;
    int n,i;
    lin linea;
    int nro;
    int numlin;
    int fcreato;
    pipe_t *piped;
    int fd;
    int nr1,nr2;
    int L1,L2;
    int pidFiglio,status,ritorno;

    if(argc<2)
    {
        printf("ERROR: errore nel numero di parametri in ingresso %d",argc-1);
        exit(1);
    }
    if((fcreato=creat("PALETTI.log",PERM))<0)
    {
        printf("ERROR: errore nella creazione del file PALETTI.log");
        exit(2);
    }
    N=argc-1;
    /* Creazione di N pipe */
    if ((piped = (pipe_t *) malloc(N*2 * sizeof(pipe_t))) == NULL) {
        printf("Errore allocazione pipe\n");
        exit(3);
    }
    for (i = 0; i < N*2; i++) {
        if (pipe(piped[i]) < 0) {
            printf("Errore creazione pipe\n");
            exit(4);
        }
    }
    printf("DEBUG: in esecuzione il processo padre con pid %d, passati %d file, creazione di %d procesi figli\n",getpid(),N,N*2);
    for ( n = 0; n < N*2; n++)
    {
        if ((pid = fork()) < 0) {
            /* Fork fallita */
            printf("Errore in fork\n");
            exit(5);
        }
        
        if (pid == 0) {
            /* CODICE DEL FIGLIO*/
        
            printf("Figlio, pid: %d, indice %d\n", getpid(), n);
            for (i = 0; i < N*2; i++)
            {
                close(piped[i][0]);
                if(i!=n)
                {
                    close(piped[i][1]);
                }
            }
            if((fd=open(argv[n/2+1],O_RDONLY))<0){
                printf("ERROR: errore nella apertura del file %s",argv[n/2+1]);
                exit(-1);
            }
            i=0;
            nro=-1;
            numlin=0;
            /* leggi tutto il file */
            while (read(fd, &(linea[i]), 1)) {
                if(linea[i]=='\n')
                {
                    i++;
                    numlin++;
                    if(((n%2)!=0) && ((numlin%2)!=0)){
                        write(piped[n][1],&i,sizeof(i));
                    }
                    if(((n%2)==0) && ((numlin%2)==0))
                    {
                        write(piped[n][1],&i,sizeof(i));
                    }
                    if(i>nro){
                        nro=i;
                    }
                    i=0;
                }
                else
                {
                    i++;
                }
            }
            exit(nro);
            /* FINE CODICE DEL FIGLIO */
        }
        printf("Generato figlio con PID = %d\n", pid);
    }
    for (  n= 0; n < N*2; n++)
    {
        close(piped[n][1]);        
    }
    for (n = 0; n < N; n++)
    {
        numlin=1;
        printf("DEBUG: la lunghezza delle righe del file %s sono:\n",argv[n+1]);
        do
        {
            nr1=read(piped[n*2+1][0],&L1,sizeof(L1));
            nr2=read(piped[n*2][0],&L2,sizeof(L2));
            if(nr1!=0)
            {
                printf("DEBUG: linea num:%d lunga %d\n",numlin,L1);
                numlin++;
            }
            if(nr2!=0)
            {
                printf("DEBUG: linea num:%d lunga %d\n",numlin,L2);
                numlin++;
            }
        } while (nr1||nr2);
        
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
            sprintf(linea,"Il figlio con pid=%d ha ritornato il carattere %c (in decimale %d, se 255 problemi)\n", pidFiglio, ritorno, ritorno);
            write(fcreato,linea,strlen(linea));
        }
    }
    exit(0);
}