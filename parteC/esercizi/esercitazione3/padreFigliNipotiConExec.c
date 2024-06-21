/*file padrefiglinipoticonexec.c*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

/*definisco il permesso per i file che creo*/
#define PERM 0644

int main(int argc, char const *argv[])
{
    int N; //variabile che salva il numero passato in input
    int pid;    //utilizzato per la fork
    int fdw; //per la creazione del file
    int i; //indice
    int pidFiglio; // per valore del pid figlio
    int status, ret; // èper salvare il ritorno del pid figlio
    char *fOUT;

    //controllo del numero dei parametri
    if(argc<4){
        printf("ERROR: numero di parametri non corretto");
        exit(1);
    }
    // trovo il numero di file passati
    N=argc-1;

    printf("sono il processo padre con pid: %d e invoco %d processi figlio\n", getpid(),N);
    //creazione dei procesi figlio
    for(i=0;i<N;i++){
        //controllo della giusta creazione del processo figlio
        if((pid=fork())<0){
            printf("ERROR: errore nella creazione del processo figlio");
            exit(-1);
        }
        if(pid==0){
            //codice del figlio
            printf("DEBUG:sono il processo figlio con pid:%d e indice %d\n",getpid(),i+1);
            fOUT=(char*)malloc(strlen(argv[i+1])+6); //alloco dinamicamente una stringa lunga quanto il nome del file + .sort
            //controllo la corretta creazione della malloc
            if(fOUT==NULL){
                printf("ERROR: erroer nella creazione della malloc nel processo figlio %d",i+1);
                exit(-1);
            }
            //inserisco nella malloc il nome del file e il .sort
            strcpy(fOUT,argv[i+1]);
            strcat(fOUT,".sort");
            if((fdw=creat(fOUT,PERM))<0){
                printf("ERROR: errore nella creazione del file");
                exit(-1);
            }
            close(fdw);
             if((pid=fork())<0){
            printf("ERROR: errore nella creazione del processo nipote");
            exit(-1);
            }
            if(pid==0){
                    //processo nipote
                printf("DEBUG:in eseguizione il processo nipote %d del figlio %d\n",getpid(),getppid());
                close(0);
            if (open(argv[i+1], O_RDONLY) < 0)
				{	
					printf("Errore: FILE %s NON ESISTE\n", argv[i+1]); 
					exit(-1);
				}
			/* chiudiamo lo standard output  */
        	close(1);
			/* apriamo il file creato in sola scrittura */
			if (open(fOUT, O_WRONLY) < 0)
				{	
					printf("Errore: FILE %s NON si riesce ad aprire in scrittura\n", fOUT); 
					exit(-1);
				}
/* Il nipote diventa il comando-filtro sort */
             execlp("sort", "sort", (char *)0);
		       	/* usiamo perror che scrive su standard error, dato che lo standard output e' collegato ad un file */
        	perror("ERROR:Problemi di esecuzione del sort da parte del nipote");
    		exit(-1); 
            }
            if ((pidFiglio=wait(&status)) < 0)
                	{
                		printf("ERROR:Errore in wait\n");
                        	exit(-1); 
                	}
                	if ((status & 0xFF) != 0)
                	{
                		printf("ERROR:Nipote con pid %d terminato in modo anomalo\n", pidFiglio);
                        	exit(-1); 
                	}
                	else
				ret=(int)((status >> 8) & 0xFF);
               		/* il figlio ritorna il valore ricevuto dal nipote  */
                	exit(ret);
        }
        for(i=0;i<N;i++){

        	if ((pidFiglio=wait(&status)) < 0)
        	{
                	printf("ERROR: Errore in wait\n");
                	exit(3);
        	}	
        	if ((status & 0xFF) != 0)
                	printf("ATTENTIO: Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        	else
        	{ 
			ret=(int)((status >> 8) & 0xFF);
        		printf("DEBUG:Il figlio con pid=%d ha ritornato %d (se 255 problemi!)\n", pidFiglio, ret);
        	} 
        }
    }
    exit(0);
}
