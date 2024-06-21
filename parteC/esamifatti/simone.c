/* Simone Gaglione */
/* Used c11*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>

#define PERM 0644	/* DIRITTI di lettura e scrittura al proprietario, lettura al gruppo e agli altri */

typedef int pipe_t[2];	/* tipo per array di pipe */

int main(int argc, char const *argv[])
{
    int Q;              /* numero di processi figli */
    int q;              /* indice per processi figli */
    char linea[250];    /* per memorizzare la linea (sia da parte del padre che dei figli) */
    int pid;            /* per la fork */
    pipe_t *p;          /* array dinamico per la comunicazione padre-figli */
    int j;              /* indice */
    int fp;             /* per la open del padre */
    int ff;             /* per la open dei figli */
    int ritorno=0;      /* variabile ritornata dai figli al padre */
    int status,pidFiglio;   /* per la wait */

    /* controllo il numero di parametri */
    if(argc<3){
        printf("ERROR: numero di parametri insuff\n");
        exit(1);
    }

    /* salvo il numero di file passati */
    Q=argc-1;

    /* allocazione array di pipe */
    p=(pipe_t*)malloc(Q*sizeof(pipe_t));
    if(p==NULL){
        printf("ERROR: errore malloc p\n");
        exit(2);
    }

    /* creazione pipe */
    for(q=0;q<Q;q++){
        if(pipe(p[q])<0){
            printf("ERROR: errore creazione pipe\n");
            exit(3);
        }
    }

    /* creazione file */
    fp=creat("camilla",PERM);
    if(fp<0){
        printf("ERROR: errore creazione file\n");
        exit(4);
    }

    /* generazione figli */
    for(q=0;q<Q;q++){
        if((pid=fork())<0){
            printf("ERROR: errore fork\n");
            exit(5);
        }
        if(pid==0){
            /* figlio */
            printf("DEBUG: figlio con indice %d, pid=%d analizza file '%s'\n",q,getpid(),argv[q+1]);
            /* chiusura pipe inutilizzate */
            for(j=0;j<Q;j++){
                close(p[j][0]); /* non legge da alcuna pipe */
                if(j!=q)
                    close(p[j][1]); /* scrive solo sulla pipe q-esima*/
            }

            /* apertura del proprio file in lettura */
            ff=open(argv[q+1],O_RDONLY);
            if(ff<0){
                printf("ERROR: file non esistente o non leggibile\n");
                exit(-1);
            }

            /* inizializzo indice di caratteri nella linea */
            j=0;
            /* leggo le linee del file */
            while(read(ff,&(linea[j]),1)){
                if(linea[j]=='\n'){
                    if(isdigit(linea[0]) && (j<9)){  /* se rispettate le specifiche del testo */
                        write(p[q][1],linea,(j+1)*sizeof(char));
                        ritorno++;  /* incrementiamo il conteggio delle linee inviate al apdre */
                    }
                    j=0;
                }
                else{
                    j++;
                }
            }
            exit(ritorno);
        }
    }
    /* padre */
    /* chiusura pipe inutilizzate */
    for(q=0;q<Q;q++)
        close(p[q][1]); /* non scrive */
    /* legge dalle pipe tutte le linee per ogni figlio */
    for(q=0;q<Q;q++){
        j=0;
        printf("\na\n");
        while(read(p[q][0],&(linea[j]),1)){
            if(linea[j]=='\n'){
                linea[j+1]='\0';
                /* stampiamo su stdout */
                printf("Figlio di indice %d ha trovato nel file '%s' la linea:\n %s \n che inizia con un carattere numerico e ha lungh<10\n",q,argv[q+1],linea);
                /* scriviamo sul file camilla senza il terminatore di stringa */
                write(fp,linea,j+1);
                j=0;    /* resettiamo indice di linea per la successiva */
            }
            else{
                j++;
            }
        }
    }
    /* attesa figli */
        for(q=0;q<Q;q++){
        if((pidFiglio=wait(&status))<0){
            printf("Errore nella wait\n");
            exit(6);
        }
        if((status & 0xFF)!=0){
            printf("figlio con pid %d terminato in modo anomalo\n",pidFiglio);
        }
        else{
            ritorno=(int)((status >> 8) & 0xFF);
            printf("il figlio con pid=%d ha ritornato %d (se 255 problemi)\n",pidFiglio,ritorno);
        }
    }
    exit(0);
}