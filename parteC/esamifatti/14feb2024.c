/* file 14feb24.c code:c11*/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
typedef int pipe_t[2];
typedef struct 
{
   char found[12];
   int pidN;
}Strut;


int main(int argc, char *argv[])
{
    /*variabili locali*/
    int pid;
    int status,ritorno;
    int N,n;
    int j;
    int nr,nw;
    pipe_t *piped;
    Strut *cur;
    /*----------------*/
    if (argc<4)
    {
        printf("ERROR: parametri in input errati %d ",argc-1);
        exit(1);
    }
    N=argc-2;
    
    if ((piped=(pipe_t *)malloc(N*sizeof(pipe_t)))==NULL)
    {
        printf("ERROR: errore nella allocazione della pipe");
        exit(2);
    }
    for (n = 0; n < N; n++)
    {
        if (pipe(piped[n])<0)
        {
            printf("ERROR:errore nella creazione della pipe");
            exit(3);
        }
    }

    for (n = 0; n < N; n++)
    {
        if ((pid=fork())<0)
        {
            printf("ERROR: errore nella fork del processo figlio %d",n);
            exit(4);
        }

        if (pid==0)
        {
            for ( j = 0; j < N; j++)
            {
                if(j!=n)
                {
                    close(piped[j][1]);
                }
                if (n==0 || (j!=n-1))
                {
                    close(piped[j][0]);
                }
            }
            if ((pid=fork())<0)
            {
                printf("ERROR: errore nella fork del processo nipote collegato al figlio %d",n);
                exit(1);
            }
            if (pid==0)
            {
                printf("DEBUG: in esecuzione il processo nipote con pid: %d del figlio con pid %d\n",getpid(),getppid());
                close(piped[n][1]);
                if (n!=0)
                {
                    close(piped[n-1][0]);
                }
        		printf("DEBUG-Nipote %d cerca nel file %s la stringa %s\n", getpid(), argv[1], argv[n+2]);
                close(1);
                open("/dev/null",O_WRONLY);
                close(2);
                open("/dev/null",O_WRONLY);
                execl("/bin/grep","grep",argv[n+2],argv[1],(char*)0);
                exit(1);
            }
            printf("DEBUG-Sono il processo figlio di indice %d e pid %d\n", n, getpid());
           
            if(( cur=(Strut *)malloc((n+1)*sizeof(Strut)))==NULL){
                printf("ERROR: errore nella allocazione della cur");
                exit(1);
            }
            if(n!=0){
                nr=read(piped[n-1][0],cur,n*sizeof(Strut));
                if(nr!=(n*sizeof(Strut)))
                {
                    printf("ERROR: errore nella lettura dei bit di cur\n");
                    exit(1);
                }
            }
            cur[n].pidN=pid;
            ritorno=1;
            pid=wait(&status);
            if(pid<0)
            {
                printf("ERROR: errore nella wait del pid nipote");
                strcpy(cur[n].found,"NON TROVATA");
            }
            if ((status & 0xFF)!=0)
            {
                printf("ATTENTIO: nipote terminato in modo anomalo");
                strcpy(cur[n].found,"NON TROVATA");
            }
            else{
                ritorno= (status >> 8) & 0xFF;
                if (ritorno==0)
                {
                    strcpy(cur[n].found,"TROVATA");
                }
                else{
                    strcpy(cur[n].found,"NON TROVATA");
                }
            }
            nw=write(piped[n][1],cur,(n+1)*sizeof(Strut));
            if(nw!=(n+1)*sizeof(Strut))
            {
                printf("ERROR: errore nella scrittura della cur");
                exit(1);
            }
            exit(ritorno);
        }
    }
    for ( n = 0; n < N; n++)
    {
        close(piped[n][1]);
        if(n!=N-1){
            close(piped[n][0]);
        }
    }
    if ((cur=(Strut *)malloc(N*sizeof(Strut))) == NULL)
	{
        	printf("Errore allocazione cur nel padre\n");
        	exit(5);
	}
    nr=read(piped[N-1][0],cur,N*sizeof(Strut));
    if(nr!=N*sizeof(Strut))
    {
        printf("ERROR: letto un numero sbagliato di struct");
        exit(6);
    }
    nr=nr/sizeof(Strut);
	printf("DEBUG-Padre ha letto un numero %d di strutture\n", nr);
    for ( n = 0; n < N; n++)
    {
        if (strcmp(cur[n].found,"TROVATA")==0)
        {
            printf("Il figlio di indice %d ha generato un nipote con pid %d che HA ha trovato la stringa %s nel file %s\n",n,cur[n].pidN,argv[n+2],argv[1]);
        }
        else
        {
            printf("Il figlio di indice %d ha generato un nipote con pid %d che NON ha trovato la stringa %s nel file %s\n",n,cur[n].pidN,argv[n+2],argv[1]);
        }
    }
    

	/* Il padre aspetta i figli */
	for (n=0; n < N; n++)
	{
		pid = wait(&status);
		if (pid < 0)
		{
			printf("Errore in wait\n");
			exit(7);
		}

		if ((status & 0xFF) != 0)
    			printf("Figlio terminato in modo involontario\n");
    	else
			    printf("Il figlio con pid=%d ha ritornato %d (se 1 problemi)\n", pid, (int)((status >> 8) & 0xFF));
	}

	exit(0);
}
