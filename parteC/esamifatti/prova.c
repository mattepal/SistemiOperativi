/* Soluzione parte C esame del 14 Febbraio 2024 */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef int pipe_t[2];
 
typedef struct {
		 char t[12];  		/* campo c1 del testo */
					/* bastano 12 caratteri per contenere sia la stringa "TROVATA" che la stringa "NON TROVATA", insieme con il terminatore di stringa (vedi specifica) */
	         int pid_nipote;        /* campo c2 del testo */
		 } Strut;

int main(int argc, char **argv) {

        int N;                  /* numero di processi figli */
        int pid;                /* variabile per fork */
   	pipe_t *pipes;  	/* array di pipe usate a pipeline da primo figlio, a secondo figlio .... ultimo figlio e poi a padre: ogni processo (a parte il primo) legge dalla pipe i-1 e scrive sulla pipe i */
   	Strut *cur;     	/* array di strutture usate dai figli e dal padre: ogni processo crea l'array della dimensione minima per le sue esigenze! */
        int n, j;               /* indici per cicli */
        int nr, nw;             /* per controlli read e write su/da pipe */
        int status, ritorno; 	/* per wait */

	/* Controllo sul numero di parametri */
	if (argc < 4) /* Meno di tre parametri: nome file e almeno due stringhe */ 
	{
		printf("Errore nel numero dei parametri dato che argc=%d (bisogna passare un file e almeno due stringhe!)\n", argc);
		exit(1);
	}

	/* Calcoliamo il numero di stringhe passate e quindi di figli da creare */
	N = argc - 2;
	
	printf("DEBUG-Numero di processi da creare %d\n", N);

	/* Allocazione dell'array di N pipe descriptors*/
	if ((pipes=(pipe_t *)malloc(N*sizeof(pipe_t))) == NULL)
	{
        	printf("Errore allocazione pipe\n");
        	exit(2);
	}
	
	/* Creazione delle N pipe usate in pipeline */
	for (n=0;n<N;n++)
        	if(pipe(pipes[n])<0)
        	{
                	printf("Errore creazione pipe\n");
                	exit(3);
        	}
	
	/* Ciclo di generazione dei figli */
	for (n=0; n < N; n++)
	{
		if ( (pid = fork()) < 0)
		{
			printf("Errore nella fork");
			exit(4);
		}
		
		if (pid == 0) /* codice del figlio */
		{
			/* In caso di errori sia nel figlio che nel nipote decidiamo di tornare 1 che verra' interpretato come stringa NON TROVATA */
			/* Chiusura dei lati non utilizzati delle pipe */
                	for (j=0;j<N;j++)
                	{
                        	if (j!=n)
                                	close (pipes[j][1]);
                        	if ((n == 0) || (j != n-1))
                                	close (pipes[j][0]);
                	}
			
			/* Creazione del processo nipote: la chiusura delle pipe puo' essere fatta prima dato che il nipote NON deve usare alcuna pipe! */
			if ( (pid = fork()) < 0)
			{
				printf("Errore nella fork");
				exit(1);
			}
			
			if (pid == 0) /* codice del nipote */
			{
				/* Chiusura degli unici lati di pipe rimasti aperti: il nipote non usa le pipe! */
				close(pipes[n][1]);
				if (n!=0)	close(pipes[n-1][0]);
	
				printf("DEBUG-Nipote %d cerca nel file %s la stringa %s\n", getpid(), argv[1], argv[n+2]);
				
				/* Ridirezione dello standard output su /dev/null (per evitare messaggi di grep a video)*/
				close(1);
				open("/dev/null", O_WRONLY);
			
				/* ridirezionamo anche lo standard error su /dev/null perche' ci interessa solo se il comando grep ha successo o meno */
				close(2);
				open("/dev/null", O_WRONLY);

				/* Il nipote diventa il comando grep */				
				execl("/bin/grep", "grep", argv[n+2], argv[1], (char *)0);
				/* attenzione all'ordine dei parametri nella esecuzione della grep: prima stringa e poi file e quindi terminatore della lista. Il file si trova usando l'indice 1 (cioe' e' il primo parametro), mentre la stringa la si trova usando l'indice i incrementato di 2 (cioe' per il primo processo n=0 la stringa file e' argv[2])  */
			
				/* si esegue l'istruzione seguente SOLO in caso di fallimento della execlp ATTENZIONE SE LA EXEC FALLISCE NON HA SENSO NE' FARE printf("Errore in execlp\n"); ne' perror */
				exit(1); /* torno un valore diverso da zero per indicare insuccesso*/
			}
			
			/* codice del figlio */
			printf("DEBUG-Sono il processo figlio di indice %d e pid %d\n", n, getpid());
                	/* allocazione dell'array di strutture specifico di questo figlio */
                	/* creiamo un array di dimensione n+1 anche se leggeremo n strutture, dato che poi ci servira' riempire la n+1-esima struttura! */
                	if ((cur=(Strut *)malloc((n+1)*sizeof(Strut))) == NULL)
                	{
                        	printf("Errore allocazione cur\n");
                        	exit(1);
                	}
                	if (n!=0)
                	/* lettura da pipe dell'array di strutture per tutti i figli a parte il primo */
                	{
                        	nr=read(pipes[n-1][0],cur,n*sizeof(Strut));
                        	if (nr != n*sizeof(Strut))
                        	{
                                	printf("Figlio %d ha letto un numero di strutture sbagliate %d\n", n, nr);
                                	exit(1);
                        	}
                	}
                	/* inizializziamo l'ultima struttura che e' quella specifica del figlio corrente (nel caso del primo figlio sara' l'unica struttura */
                	cur[n].pid_nipote = pid;
			
			ritorno=1; /* si veda commento precedente */

			/* Aspettiamo la fine del nipote/grep */
			pid = wait(&status);
			if (pid < 0)
			{
				printf("Errore in wait\n");
				/* si decide di mandare la stringa NON TROVATA al padre e tornare il valore 1 */
                                strcpy(cur[n].t, "NON TROVATA");
			}

			if ((status & 0xFF) != 0)
    			{
				printf("Nipote terminato in modo involontario\n");
				/* si decide di mandare la stringa NON TROVATA al padre e tornare il valore 1 */
                                strcpy(cur[n].t, "NON TROVATA");
 				}
    			else
    			{				
				/* recupero il suo codice di ritorno */
				ritorno = (status >> 8) & 0xFF;
				if (ritorno == 0) /* stringa trovata */
					strcpy(cur[n].t, "TROVATA");
				else
					strcpy(cur[n].t, "NON TROVATA");
			 }

                	/* tutti i figli mandano in avanti, l'ultimo figlio manda al padre un array di strutture (per tutti i figli a parte il primo sono i strutture ricevute dal processo precedente e la i+1-esima che e' la propria) */
                	nw=write(pipes[n][1],cur,(n+1)*sizeof(Strut));
                        if (nw != (n+1)*sizeof(Strut))
                        {
                               	printf("Figlio %d ha scritto un numero di strutture sbagliate %d\n", n, nr);
                                exit(1);
                        }
			exit(ritorno); /* fine codice del figlio */
		}	
	}
	
	/* Codice del padre */
	/* chiusura pipe: tutte meno l'ultima in lettura */
	for(n=0;n<N;n++)
	{
        	close(pipes[n][1]);
        	if (n != N-1) close (pipes[n][0]);
	}

	/* allocazione dell'array di strutture specifico per il padre */
	/* creiamo un array di dimensione N quanto il numero di figli! */
	if ((cur=(Strut *)malloc(N*sizeof(Strut))) == NULL)
	{
        	printf("Errore allocazione cur nel padre\n");
        	exit(5);
	}

	/* il padre deve leggere l'array di strutture che gli arriva dall'ultimo figlio */
	nr=read(pipes[N-1][0],cur,N*sizeof(Strut));
	if (nr != N*sizeof(Strut))
	{
        	printf("Padre ha letto un numero di strutture sbagliate %d\n", nr);
        	exit(6);
	}
	nr=nr/sizeof(Strut);
	printf("DEBUG-Padre ha letto un numero %d di strutture\n", nr);
	/* il padre deve stampare i campi delle strutture ricevute */
	for(n=0;n<N;n++)
	{
		if (strcmp(cur[n].t, "TROVATA")==0)
        		printf("Il figlio di indice %d ha generato un nipote con pid %d che ha trovato la stringa %s nel file %s\n", n, cur[n].pid_nipote, argv[n+2], argv[1]);
		else
        		printf("Il figlio di indice %d ha generato un nipote con pid %d che NON ha trovato la stringa %s nel file %s\n", n, cur[n].pid_nipote, argv[n+2], argv[1]);

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
