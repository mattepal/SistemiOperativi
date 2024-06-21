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
#define PERM 0664
typedef int pipe_t[2];



int main(int argc, char **argv) {
    /*variabili locali*/
    int pid;
    int pidFiglio,status,ritorno;
    int Q;
    int q,i;
    char linea[250];
    int fdcreato,fd;
    pipe_t *piped;
    /*------*/
    if (argc<3)
    {
        printf("ERROR: errore nel numero di paramerti iniziali %d",argc-1);
        exit(1);
    }
    Q=argc-1;
    printf("DEBUG: processo padre in esecuzione con pid: %d, creazione di %d processi figlio\n",getpid(),Q);
    /* Creazione di N pipe */
    if ((piped = (pipe_t *) malloc(Q * sizeof(pipe_t))) == NULL) {
        printf("Errore allocazione pipe\n");
        exit(2);
    }
    for (i = 0; i < Q; i++) {
        if (pipe(piped[i]) < 0) {
            printf("Errore creazione pipe\n");
            exit(3);
        }
    }
    if ((fdcreato=creat("Camilla",PERM))<0)
    {
        printf("ERROR: errore nella creazione del file %s","camilla");
        exit(4);
    }
    for ( q = 0; q < Q; q++)
    {
        if ((pid = fork()) < 0) {
            /* Fork fallita */
            printf("Errore in fork\n");
            exit(5);
        }
        
        if (pid == 0) {
            /* CODICE DEL FIGLIO*/
        
            printf("Figlio, pid: %d, indice %d\n", getpid(), 1);
            for (i = 0; i < Q; i++)
            {
                close(piped[i][0]);
                if(i!=q)
                {
                    close(piped[i][1]);
                }
            }
            if ((fd=open(argv[q+1],O_RDONLY))<0)
            {
                printf("ERROR: errore nella apertura del file %s",argv[q+1]);
                exit(-1);
            }
            i=0;
            printf("\nb\n");
            ritorno=0;
            while (read(fd,&(linea[i]),1))
            {
                if (linea[i]=='\n')
                {
                    if(isdigit(linea[0]) && (i<9))
                    {
                        write(piped[q][1],linea,(i+1)*sizeof(char));
                        ritorno++;
                    }
                    i=0;
                }
                else
                {
                    i++;
                }
            }
            exit(ritorno);
            /* FINE CODICE DEL FIGLIO */
        }
        printf("Generato figlio con PID = %d\n", pid);
    }
    for (i = 0; i < Q; i++)
    {
        close(piped[i][1]);
    }
    printf("\na\n");
    for (q = 0; q < Q; q++)
    {
        i=0;
        while (read(piped[q][0],&(linea[i]),1))
        {
            if(linea[i]=='\n'){
                linea[i+1]='\0';
                printf("DEBUG: il figlio con indice %d ha trovato sul file %s la linea: %s\n",q,argv[q+1],linea);
                write(fdcreato,linea,i+1);
                i=0;
            }
            else{
                i++;
            }
        }
    }
    /* Il padre aspetta i figli salvando lo status */
    for (int i = 0; i < Q; i++) {
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