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

typedef struct
{
    int pidN;
    char ultimaLin[250];
    int lungUltimLin;
} Struct;
int main(int argc, char const *argv[])
{
    /*variabili locali*/
    int pid;
    int pidFiglio,status,ritorno;
    int N;
    int i,j;
    pipe_t *piped;
    pipe_t p;
    Struct s;
    char buffer[250];
    int nr;
    /*-----------------*/
    if(argc<3)
    {
        printf("ERROR: numero di parametri errato %d",argc-1);
        exit(1);
    }
    N=argc-1;
    piped=(pipe_t*)malloc(N*sizeof(pipe_t));
    if(piped==NULL)
    {
        printf("ERROR: errore nella allocazione della pipe");
        exit(2);
    }
    for(i=0;i<N;i++)
    {
        if (pipe(piped[i])<0)
        {
            printf("ERROR: errore nella creazione della pipe");
            exit(3);
        }
    }
    for (i = 0; i < N; i++)
    {
        if ((pid=fork())<0)
        {
            printf("ERROR: Errore nella fork del processo figlio");
            exit(4);
        }
        if(pid==0){
            /*codice figlio*/
            for ( j = 0; j < N; j++)
            {
                close(piped[j][0]);
                if (j!=i)
                {
                    close(piped[j][1]);
                }
            
            }
            if (pipe(p)<0)
            {
                printf("ERROR:errore nella creazione della pipe tra figlio %d e nipote",i);
                exit(-1);
            }
            if((pid=fork())<0){
                printf("ERROR: errore nella fork del porcesso figlio %d",i);
                exit(-1);
           }
            if(pid==0)
            {
                /*processo nipote*/
                close(piped[i][1]);
                close(1);
                dup(p[1]);
                close(p[1]);
                close(p[0]);
                execlp("rev","rev",argv[i+1],(char*)0);
                perror("ERROR: rev non andato a buon fine");
                exit(-1);
            }
            close(p[1]);
            j=0;
            s.pidN=pid;
            while (read(p[0],&(buffer[j]),1))
            {
                if (buffer[j]=='\n')
                {
                    s.lungUltimLin=j;
                    j=0;
                }
                else{
                    j++;
                }
            }
            for (j = 0; j < s.lungUltimLin+1; j++)
            {
                s.ultimaLin[j]=buffer[j];
            }
            write(piped[i][1],&s,sizeof(s));
            if((pidFiglio=wait(&status))<0){
                printf("ERROR: errore nella wait del figlio nipote");
                exit(-1);
            }
           if ((status & 0xFF) != 0)
            {
                printf("Nipote con pid %d terminato in modo anomalo\n", pid);
                exit(-1); 
            }
            else{
                if ((int)((status >> 8) & 0xFF) != 0) /* in questo caso c'e' stato un errore nel nipote */
				    ritorno = -1;
			    else
			        ritorno=s.lungUltimLin+1;
                 exit(ritorno); 
            }
        }
    }
    for ( i = 0; i < N; i++)
    {
        close(piped[i][1]);
    }
    
    for ( i = 0; i < N; i++)
    {
        nr=read(piped[i][0],&s,sizeof(s));
        if (nr!=0)
        {
            s.ultimaLin[s.lungUltimLin+1]='\0';
        }
          printf("Il nipote con pid %d ha letto dal file %s questa linea '%s' che ha lunghezza (NON compreso il terminatore) di %d caratteri:\n", s.pidN, argv[i+1], s.ultimaLin, s.lungUltimLin);
    }
    
    for (i=0; i < N; i++)
    {
        pid = wait(&status);
        if (pid < 0)
        {
           	printf("Errore in wait\n");
           	exit(5);
        }
        if ((status & 0xFF) != 0)
         	printf("Figlio con pid %d terminato in modo anomalo\n", pid);
        else
        {
            ritorno=(int)((status >> 8) & 0xFF);
         	printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi nel figlio o nel nipote)\n", pid, ritorno);
        }
    }
    exit(0);
}
