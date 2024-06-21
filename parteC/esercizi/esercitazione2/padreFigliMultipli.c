   #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char const *argv[])
{
    int pid;
    int pidFiglio;
    int status;
    int ritorno;
    if (argc >2){
        printf("ERROR: input errato");
        exit(1);
    }
        int N=atoi(argv[1]);
    if(N<=0 || N>255){
        printf("ERROR: parametro in non corretto");
        exit(2);
    }

    printf("sono il processo padre con PID:%d\n",getpid());
    for(int i=0;i<N;i++){
        if((pid=fork())<0){
            printf("ERROR:errore nella creazione del pid figlio\n");
            exit(3);
        }
        if(pid==0){
        printf("creato %d^ processo figlio con pid %d del padre con pid %d\n",i+1,getpid(),getppid());  
        exit(i+1);  
        }
    }
    for(int i=0;i<N;i++){
        if((pidFiglio=wait(&status))<0){
            printf("ERROR: errore nella wait del figlio");
            exit(5);
        }
        if ((status & 0xFF)!=0)
        {
            printf("ERROR; figlio terminato in modo anomalo\n");
        }
        else{
            ritorno=(int)((status>>8)& 0xFF);
            printf("il figlio con pid:%d ha ritornato %d\n",pidFiglio,ritorno);
        }
    }
    exit(0);
}
