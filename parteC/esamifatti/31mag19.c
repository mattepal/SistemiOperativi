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
typedef int pipe_t[2];
typedef struct{
    int c1;		/* campo c1 del testo */
    int c2;		/* campo c1 del testo */
    char c3[250];		/* campo c1 del testo */
} nomeStrut ;


int main(int argc, char **argv) {
    /*variabili locali*/
    int pid;
    int pidFiglio,status,ritorno;
    pipe_t *piped;
    pipe_t p;
    int N;
    int i,j;
    int lung;
    int nr;
    nomeStrut cur;
    /*----------------*/
    /* controllo numero parametri */
    if (argc <4) {
        printf("ERROR: '%d' numero errato di parametri\n", argc - 1);
        exit(1);
    }
    N=argc-1;
    /* Allocazione array di N pipe */
    if ((piped = (pipe_t *) malloc(N * sizeof(pipe_t))) == NULL) {
        printf("Errore allocazione pipe\n");
        exit(2);
    }
    /* Creazione pipe */
    for (i = 0; i < N; i++) {
        if (pipe(piped[i]) < 0) {
            printf("Errore creazione pipe\n");
            exit(3);
        }
    }
    for ( i = 0; i < N; i++)
    {
        if ((pid = fork()) < 0) {
            /* Fork fallita */
            printf("Errore in fork\n");
            exit(4);
        }
        
        if (pid == 0) {
            /* CODICE DEL FIGLIO*/
            printf("Figlio, pid: %d, indice %d\n", getpid(), i);
            for ( j = 0; j < N; j++)
            {
                close(piped[j][0]);
                if(i!=j)
                {
                    close(piped[j][1]);
                }
            }
            if(pipe(p)){
                printf("ERROR: errore nella creazione della pipe tra figlio e nipote\n");
                exit(-1);
            }
            if ((pid = fork()) < 0) {
                /* Fork fallita */
                printf("ERROR:Errore in fork\n");
                exit(-1);
            }
            
            if (pid == 0) {
                /* CODICE DEL NIPOTE*/
            
                printf("DEBUG:nipote, pid: %d\n", getpid());
                close(1);
                dup(p[1]);
                close(p[1]);
                close(p[0]);
                execlp("sort","sort","-f",argv[i+1],(char*)0);
                perror("ERROR: errore nella sort");
                exit(-1);
                /* FINE CODICE DEL NIPOTE */
            }
            printf("DEBUG:Generato nipote con PID = %d\n", pid);
            close(p[1]);
            cur.c1=pid;
            lung=0;
            while (read(p[0],&(cur.c3[lung]),1))
            {
                if(cur.c3[lung]=='\n'){
                    lung++;
                    cur.c2=lung;
                    write(piped[i][1],&cur,sizeof(nomeStrut));
                    break;
                }
                else{
                    lung++;
                }
            }
            ritorno=-1;
            /* Il padre aspetta i figli */
                if((pidFiglio=wait(&status))<0){
                    printf("ERROR in wait\n");
                    exit(-1);
                }
                if((status & 0xFF) != 0){
                    printf("nipote con pid=%d terminato in modo anomalo\n",pidFiglio);
                    exit(-1);
                }
                else{
                    ritorno=(cur.c2-1);
                }
            exit(ritorno);
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
        nr=read(piped[i][0],&cur,sizeof(nomeStrut));
        if(nr!=sizeof(nomeStrut))
        {
            printf("ERROR: errore nella lettura del padre");
        }
        cur.c3[cur.c2+1]='\0';
        printf("DEBUG: il processo nipote con pid %d ha ritornato una riga lunga %d contenuto: %s",cur.c1,cur.c2,cur.c3);
    }
    /* Il padre aspetta i figli */
    for(int i = 0; i < N; i++) {
        if((pidFiglio=wait(&status))<0){
            printf("ERROR in wait\n");
            exit(5);
        }
        if((status & 0xFF) != 0){
            printf("figlio con pid=%d terminato in modo anomalo\n",pidFiglio);
        }
        else{
            ritorno=(int)((status >> 8) & 0xFF);
            printf("DEBUG: figlio %d con pid=%d ha ritornato %d (se 255 problemi)\n",i,pidFiglio,ritorno);
        }
    }    

    exit(0);
}
