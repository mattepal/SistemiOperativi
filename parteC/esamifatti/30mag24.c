/*file 10giu20.c*/
/*code:c11*/
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


int main(int argc, char **argv) {
    /*variabili locali*/
    int pid;
    int pidFiglio,status,ritorno;
    pipe_t *piped;
    pipe_t p;
    int N;
    int outline;
    int n,i,j;
    char buffer[250];
    char pidNipote[8];
    int nw;
  /*----------------*/
    if (argc!=3)
    {
        printf("ERROR: errore nel numero di paramtri in input (%d!=2)",argc-1);
        exit(1);
    }
    N=atoi(argv[1]);
    if(N<=0){
        printf("ERROR: errore il parametro %s non è un valore intero positivo",argv[1]);
        exit(2);
    }
    /* creazione file in /tmp */
    /* usato la open in versione estesa per azzerare il file nel caso esista gia' */
    if ((outline=open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0644)) < 0) {
        printf("Errore nella creazione del file %s\n", "/tmp/creato");
        exit(3);
    }
    /* Creazione di N pipe */
    if ((piped = (pipe_t *) malloc(N * sizeof(pipe_t))) == NULL) {
        printf("Errore allocazione pipe\n");
        exit(4);
    }
    for (i = 0; i < N; i++) {
        if (pipe(piped[i]) < 0) {
            printf("Errore creazione pipe\n");
            exit(5);
        }
    }
    printf("DEBUG: in esecuzione il processo padre con pid: %d che andrà a creare %d processi figli\n",getpid(),N);
    for (n = 0; n <N ;n++)
    {
        if ((pid = fork()) < 0) {
            /* Fork fallita */
            printf("Errore in fork\n");
            exit(6);
        }
        
        if (pid == 0) {
            /* CODICE DEL FIGLIO*/
        
            printf("Figlio, pid: %d, indice %d\n", getpid(),n );
            for ( i = 0; i < N; i++)
            {
               close(piped[i][0]);
               if(i!=n)
               {
                    close(piped[i][1]);
               }
            }
            if(pipe(p)<0){
                printf("ERROR: errore nella creazione della pipe tra figlio e nipote");
                exit(-1);
            }
            if((pid=fork())<0)
            {
                printf("ERROR: errore nella fork del figlio %d",n);
                exit(-1);
            }
            if(pid==0){
                /*inizio processo nipote*/
                printf("DEBUG: in esecuzione il processo nipote con pid %d del figlio con pid %d\n",getpid(),getppid());
                close(piped[n][1]);
                close(1);
                dup(p[1]);
                close(p[1]);
                close(p[0]);
                execlp("ps","ps",(char*)0);
                perror("ERROR: errore nel comando ps");
                exit(-1);
                /*fine processo nipote*/
            }
            close(0);
            dup(p[0]);
            close(p[0]);
            close(p[1]);
            close(1);
            dup(piped[n][1]);
            sprintf(pidNipote,"%d",pid);
            execlp("grep","grep",pidNipote,(char*)0);
            perror("ERROR: errore nel grep del figlio ");
            /* FINE CODICE DEL FIGLIO */
        }
        printf("Generato figlio con PID = %d\n", pid);
    }
    for (i = 0; i < N; i++)
    {
        close(piped[i][1]);
    }

    for ( i = 0; i < N; i++)
    {
        j=0;
        while (read(piped[i][0],&(buffer[j]),1))
        {
            if (buffer[j]=='\n')
            {
                nw=write(outline,buffer,(j+1)*sizeof(char));
                if(nw!=(j+1))
                {
                    printf("ERROR: errore nella write del padre");
                    exit(7);
                }
                j=0;
             }
             else
            {
                j++;
         }
        }   
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