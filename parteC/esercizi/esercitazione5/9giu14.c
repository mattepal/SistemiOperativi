/*file 9giu14.c*/
/*parte include*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef int pipe_t[2];
/*corpo del programma*/

int main(int argc, char const *argv[])
{
    /*variabili locali*/
    int N; /*varivabile utile a salvare il numero di file passato in input al programma*/
    int pid; /*variabile per la creazione dei vari processi*/
    long int somma=0;
    char numero[11];
    int valore;
    pipe_t *piped;
    pipe_t p;
    int pidFiglio,ritorno,status; /*variabili per il ritorno del processo figlio*/
    int i,j; /*contatori per i vari cicli*/
    /*controllo del numero di parametri con messaggio di errore*/
    if(argc<=2){
        printf("ERROR: %d numero di parametri errato",argc);
        exit(1);
    }
    /*salviamo il numero di file passati e controlliamo sia avvenuto correttamente*/
    N=argc-1;
    if(N<2){
        printf("ERROR: errore nella creazione della variabile N");
        exit(2);
    }
    piped=(pipe_t*)malloc(N*sizeof(pipe_t));
    if(piped==NULL){
        printf("ERROR: errore nalla allocazione dei memoria");
        exit(3);
    }
    for(i=0;i<N;i++){
        if(pipe(piped[i])<0){
            printf("ERROR:errore nella creazione della pipe");
            exit(4);
        }
    }

    printf("DEBUG: in esecuzione il processo padre con pid;%d, generazione di %d sottoprocessi in corso \n",getpid(),N);
    /*ciclo per la generazione dei figli*/
    for(i=0;i<N;i++){
        if((pid=fork())<0){
            printf("ERROR: errore nella creazione del %d^ processo figlio",i+1);
            exit(-1);
        }
        if(pid==0){
            /*codice del figlio*/
            printf("DEBUG: esecuzione del %d processo figlio con pid %d \n",i+1,getpid());
            for(j=0;j<N;j++){
                close(piped[j][0]);
                if(i!=j){
                    close(piped[j][1]);
                }
            }
            /*creazione della pipe tra figlio e nipote e controllo*/
            if(pipe(p)<0){
                printf("ERROR:errore nella creazione della pipe tra figlio e nipote");
                exit(-1);
            }
            if((pid=fork())<0){
                printf("ERROR:errore nella creazione del processo nipote");
                exit(-1);
            }
            if(pid==0){
                /*codice del nipote*/
                printf("DEBUG: in esecuzione il processo nipote con pid %d del figlio con pid %d\n",getpid(),getppid());
                close(piped[i][1]);
                close(0);
                if((open(argv[i+1],O_RDONLY))<0){
                    printf("ERROR:errore nella apertura del file %s",argv[i+1]);
                    exit(-1);
                }
                close(1);
                dup(p[1]);
                close(p[0]);
                close(p[1]);
                close(2);
                open("/dev/null",O_WRONLY);
                execlp("wc", "wc", "-l", (char *)0);
                exit(-1);
            }
        /*codice figlio*/
        close(p[1]);
        j=0;
        while(read(p[0],&(numero[j]),1)){
            j++;
        }
        if(j!=0){
            numero[j-1]='\0';
            valore=atoi(numero);
        }
        else{
            valore=0;
        }
        write(piped[i][1],&valore,sizeof(valore));
        ritorno=-1;
        if((pidFiglio=wait(&status))<0){
            printf("ERROR: errore nella wait del nipote %d\n",pidFiglio);
        }
        if((status & 0xFF)!=0){
            printf("ATTENTION: nipote terminato in modo anomalo");
        }
        else{
            ritorno=(int)((status>>8) & 0xFF);
            printf("DEBUG: nipote con pid %d ha ritononato %d\n",pidFiglio,ritorno);
        }
        exit(ritorno);
        }
    }
        /*codice del padre */
        for(i=0;i<N;i++){
            close(piped[i][1]);
        }
        for(i=0;i<N;i++){
            read(piped[i][0],&valore,sizeof(valore));
            printf("letto il valore %d dal file %s\n",valore,argv[i+1]);
            somma=somma+(long int)valore;
        }
        printf("la risultante della somma delle righe è %ld\n",somma);
        for(i=0;i<N;i++){ 
        if((pidFiglio=wait(&status))<0){
            printf("ERROR: errore nella wait del figlio %d\n",pidFiglio);
        }
        if((status & 0xFF)!=0){
            printf("ATTENTION: figlio terminato in modo anomalo");
        }
        else{
            ritorno=(int)((status>>8) & 0xFF);
            printf("DEBUG: figlio con pid %d ha ritononato %d(se 255 problemi)\n",pidFiglio,ritorno);
        }
        }
   exit(0);
}
