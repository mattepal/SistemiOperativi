#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int random(int n){
    int casual;
    casual=rand() % n;
    return casual;
}

int main()
{
    int pid;
    int pidFIglio;
    int status;
    int ritorno;
    printf("sono il processo padre con pid:%d",getpid());
    srand(time(NULL));
    if((pid=fork())<0){
        printf("ERROR: errore nella generazione del pid");
        exit(1);
    }
    if(pid==0){
        int r;
        printf("processo figlio con pid %d generato dal padre con pid: %d",getpid(),getppid());
        r=random(100);
        printf("processo figlio con numero generato per il ritorno %d",r);
        exit(r);
    }
    if((pidFIglio=wait(&status))<0){
        printf("ERROR:errore nella wait%d",pidFIglio);
        exit(2);
    }
    if(pid==pidFIglio){
        printf("terminato il figlio con pid: %d",pidFIglio);
    }
    else{
        printf("il pid della wait non corrisponde al pid della fork");
        exit(3);
    }
    if ((status & 0xFF)!=0){
        printf("figlio terminato in modo involontario");
    }
    else{
        ritorno=(int)((status>>8)& 0xFF);
        printf("terminato processo figlio e ritornato il numero %d",ritorno);
    }
    exit(0);
}
