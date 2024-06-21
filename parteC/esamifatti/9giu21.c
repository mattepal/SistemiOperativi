/*file 9giu21.c*/
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
    int pid;
    int N;
    int pidFiglio,status,ritorno;
    int fdtmp;
    pipe_t p;
    pipe_t *piped;
    char linea[200];
    int n,j;
    int x;
    int valore;
    int fd;

    if(argc<2)
    {
        printf("ERROR: errore nel numero di parametri %d",argc-1);
        exit(1);
    }
    N=argc-1;
    if ((fdtmp=creat("/tmp/matteopaletti",0664))<0)
    {
        printf("ERROR: errore nella creazioe del file tmp");
        exit(2);
    }
    /* Creazione di N pipe */
    if (pipe(p) < 0) 
    {
        printf("Errore creazione pipe\n");
        exit(4);
        }
    if((pid=fork())<0)
    {
        printf("ERROR: errore nella dork del figlio special");
        exit(3);
    }
    if(pid==0)
    {
       	close(0);
    	if (open(argv[1], O_RDONLY))
        {
            printf("Errore in apertura file %s\n", argv[1]);
         	exit(-1); /* ritorniamo -1 al padre che verra' interpretato come 255 e quindi come errore */
        }
        close(1);
        dup(p[1]);
        close(p[1]);
        close(p[0]);
        execlp("wc","wc","-l",(char*)0);
        perror("ERROR:errore nel comando wc");
        exit(-1);
    }
    close(p[1]);
    j=0;
    while (read(p[0],&(linea[j]),1))
    {
        j++;
    }
    if(j!=0)
    {
        linea[j-1]='\0';
        x=atoi(linea);
        printf("DEBUG: il padre ha ricevuto %d\n",x);
    }
    else
    {
        printf("ERROR: errore nel calcolo delle linee");
        exit(5);
    }
    close(p[0]);
    /* Creazione di N pipe */
    if ((piped = (pipe_t *) malloc(N * sizeof(pipe_t))) == NULL) {
        printf("Errore allocazione pipe\n");
        exit(6);
    }
    for (n = 0; n < N; n++) {
        if (pipe(piped[n]) < 0) {
            printf("Errore creazione pipe\n");
            exit(7);
        }
    }
    for ( n = 0; n < N; n++)
    {
        if ((pid = fork()) < 0) {
            /* Fork fallita */
            printf("Errore in fork\n");
            exit(9);
        }
        
        if (pid == 0) {
            /* CODICE DEL FIGLIO*/
            printf("Figlio, pid: %d, indice %d\n associato al file %s\n", getpid(), n,argv[n+1]);

            /* chiusura di tutte le pipe che il figlio non usa */
            for(j = 0; j < N; j++) {
                close(piped[j][0]);
                if (j != n)
                    close(piped[j][1]);
            }
            if ((fd = open(argv[n+1], O_RDONLY)) < 0) {
                printf("Errore in apertura file per '%s' -> FILE NON ESISTE\n", argv[n+1]);
                exit(-1);
            }
            j=0;
            while (read(fd,&(linea[j]),1))
            {
                if (linea[j]=='\n')
                {
                   j++;
                   write(piped[n][1],&j,sizeof(j));
                   write(piped[n][1],linea,j);
                   ritorno=j;
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

    }
    /* chiusura di tutte le pipe che il padre non usa */
    for(j = 0; j < N; j++) {
        close(piped[j][1]);
    }
    for ( j = 1; j < x; j++)
    {
        for (  n= 0; n < N; n++)
        {
         read(piped[n][0],&valore,sizeof(valore));
         read(piped[n][0],linea,valore); 
         write(fdtmp,linea,valore);  
        }
        
    }
    /* Il padre aspetta i figli salvando lo status */
    for (int i = 0; i < N+1; i++) {
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
