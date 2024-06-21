/*file 16feb22.c code:c11*/
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
typedef struct 
{
    int pidF;
    long int occ;
} Strut;

int main(int argc, char **argv) {
    /*variabili locali*/
    int pid;
    int pidFiglio,status,ritorno;
    int N;
    int i,j;
    char ch,c;
    pipe_t *piped;
    Strut *cur;
    int fd;
    int nr;
    /*-------------*/
    if (argc<4)
    {
        printf("ERROR: errore nel numero di parametri in input %d",argc-1);
        exit(1);
    }
    N=argc-2;
    if (argv[argc-1][1]!='\0')
    {
        printf("ERROR: il parametro %s non è un singolo carattere",argv[argc-1]);
        exit(2);
    }
    ch=argv[argc-1][0];
    if(!islower(ch)){
        printf("ERROR: il carattere %c non è minuscolo",ch);
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
    printf("DEBUG: in esecuzione il proceso padre con pid %d, ricerca del carattere %c,creazione di %d processi figli\n",getpid(),ch,N);
    for ( i = 0; i < N; i++)
    {
        if ((pid = fork()) < 0) {
            /* Fork fallita */
            printf("Errore in fork\n");
            exit(6);
        }
        
        if (pid == 0) {
            /* CODICE DEL FIGLIO*/
        
            printf("Figlio, pid: %d, indice %d\n", getpid(), i);
            for (j = 0;j < N; j++)
            {
                if(j!=i){
                    close(piped[j][1]);
                }
                if(i==0 || (j!=i-1)){
                    close(piped[j][0]);
                }
            }
            if((cur=(Strut *)malloc((i+1)*sizeof(Strut)))==NULL)
            {
                printf("ERROR: errrore nella allocazione della cur nel figlio %d",i);
                exit(-1);
            }
            cur[i].pidF=pid;
            cur[i].occ=0;
            if ((fd=open(argv[i+1],O_RDONLY))<0)
            {
                printf("ERROR: errore nella apertura del file %s",argv[i+1]);
                exit(N+1);
            }
            while (read(fd,&c,1))
            {
                if(c==ch){
                    cur[i].occ=cur[i].occ+1;
                }
            }
            if (i!=0)
            {
                nr=read(piped[i-1][0],cur,i*sizeof(Strut));
                if(nr!=i*sizeof(Strut))
                {
                    printf("ERROR: errore nella lettura nel processo figlio %d %d",i,nr);
                    exit(N+1);
                }
            }
            write(piped[i][1],cur,(i+1)*sizeof(Strut));
            exit(i);
            /* FINE CODICE DEL FIGLIO */
        }
        printf("Generato figlio con PID = %d\n", pid);
    }
    for (i = 0; i < N; i++)
    {
       close(piped[i][1]);
       if (i!=N-1)
       {
        close(piped[i][0]);
       }       
    }
    cur=(Strut *)malloc(N*sizeof(Strut));
    if(cur==NULL)
    {
        printf("ERROR: errore nella allocazione di cur nel padre");
        exit(8);
    }
    nr=read(piped[N-1][0],cur,N*sizeof(Strut));
    if(nr!= N*sizeof(Strut))
    {
        printf("ERROR: errore nella read del padre %d",nr);
        exit(9);
    }
    nr=nr/sizeof(Strut);
    printf("DEBUG: letti %d\n",nr);
    for ( i = 0; i < N; i++)
    {
        printf("DEBUG: il figlio con indice %d di pid %d ha trovato %ld occorenze del carattere %c\n",i,cur[i].pidF,cur[i].occ,ch);
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
            printf("Il figlio con pid=%d ha ritornato il carattere %c (in decimale %d, se 255 problemi,se >N problemi)\n", pidFiglio, ritorno, ritorno);
        }
    }
    exit(0);
}