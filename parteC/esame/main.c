/*file Main.c*/
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
    int K; /*numero di file passati*/
    int k; /*parametro per il conteggio dei figli*/
    int i; /*contatore per i cicli for*/
    int pid; /* variabile per la fork*/
    int pidFiglio,status,ritorno; /*variabili per il ritorno dei figi*/
    char buffer[250],linea[250]; /*campi di testo per salvare la linea*/
    char file[250]; /*campo di testo per la creazine del file*/
    int l_line; /*parametro per il dalvataggio della lunghezza massima*/
    int lung; /* lunghezza della linea letta*/
    int nlinea; /*numero di linea letta*/
    int createdF,fd; /*parametri per i file creati per salvataggio file dir*/
    pipe_t *piped; /* pipe per la comunicazione tra padre e figlio*/
    /*----------------*/
    /* controllo numero parametri */
    if (argc <2) {
        printf("ERROR: '%d' numero errato di parametri\n", argc - 1);
        exit(1);
    }
    K=argc-1; /*salvatoggio numero di file passati*/
    /* Allocazione array di N pipe */
    if ((piped = (pipe_t *) malloc(K * sizeof(pipe_t))) == NULL) {
        printf("Errore allocazione pipe\n");
        exit(2);
    }
    /* Creazione pipe */
    for (i = 0; i < K; i++) {
        if (pipe(piped[i]) < 0) {
            printf("Errore creazione pipe\n");
            exit(3);
        }
    }
    printf("DEBUG: sono il processo padre con pid:%d,andrò a creare %d processi figli \n",getpid(),K*2);
    /*iclo creazione figli*/
    for ( k = 0; k < K*2; k++)
    {
        if ((pid = fork()) < 0) {
            /* Fork fallita */
            printf("ERROR:Errore in fork\n");
            exit(4);
        }
        
        if (pid == 0) {
            /* CODICE DEL FIGLIO*/
        
            printf("DEBUG:Figlio, pid: %d, indice %d\n", getpid(), k);
            /*chiusura delle pipe non usate dal figlio*/
            for (i = 0; i < K; i++)
            {
                if(k>=K)
                {
                    close(piped[i][1]);
                    if(i!=k){
                        close(piped[i][0]);
                    }
                }
                else
                {
                    close(piped[i][0]);
                    if(i!=k)
                    {
                        close(piped[i][1]);
                    }
                }
            }
            if(k<K){
                if((fd=open(argv[k+1],O_RDONLY))<0)
                {
                    printf("ERROR: errore nella apertura del file %s nel filgio %d",argv[k+1],k);
                    exit(-1);
                }
            }
            else
            {
                if((fd=open(argv[k+1-K],O_RDONLY))<0)
                {
                    printf("ERROR: errore nella apertura del file %s nel filgio %d",argv[k+1],k);
                    exit(-1);
                }
                /* creazione file */
                /* usato la open in versione estesa per azzerare il file nel caso esista gia' */
                sprintf(file,"%s.MAX",argv[k+1-K]);
                if ((createdF=open(file, O_CREAT|O_WRONLY|O_TRUNC, 0644)) < 0) {
                    printf("Errore nella creazione del file %s\n", argv[k+1-k]);
                    exit(-1);
                }
            }
            lung=0;
            nlinea=0;
            while(read(fd,&(buffer[lung]),1)!=0)
            {
                if(buffer[lung]=='\n'){
                    nlinea++;
                    lung++;
                    if (k>=K && (nlinea%2)==0 ) /*controllo che sia il processo che controlli le linee pari e che la linea sia pari*/
                    {
                        read(piped[k-K][0],&l_line,sizeof(l_line)); /* lettura dei dati passati dal figlio 1*/
                        read(piped[k-K][0],linea,l_line);
                        if (l_line<lung) /* controllo il numero di linea maggiore*/
                        {
                            l_line=lung;

                            write(createdF,buffer,l_line); /*scrivo sul file creato*/
                        }
                        else
                        {
                            write(createdF,linea,l_line); /*scrivo sul file creato*/
                        }
                        
                    }
                    if(k<K && (nlinea%2)!=0 ) /*controllo che sia il figlio che controlla linee dispari e che la linea sia dispari*/
                    {

                        write(piped[k][1],&lung,sizeof(lung)); /* scrivo sulla pipe la lunghezza della linea*/
                        write(piped[k][1],buffer,lung); /* scrivo sulla pipe la linea*/
                    }
                    lung=0;  /*azzero la lunghezza della linea*/                      
                }
                else
                {
                    lung++; /*incremento lunghezza della linea*/
                }
            }
            exit(l_line);/* effettuo la exit con la linea max trovata se ce errore esce con -1*/
            /* FINE CODICE DEL FIGLIO */
        }
        printf("DEBUG:Generato figlio con PID = %d\n", pid);
    }
    /*chiusura della pipe non usata dal padre*/
    for (i = 0; i < K; i++)
    {
        close(piped[i][0]);
        close(piped[i][1]);
    }
    /* Il padre aspetta i figli */
    for(int i = 0; i < K*2; i++) {
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