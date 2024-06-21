#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int pid;
    int pidFiglio;
    int status;
    int ret;
    if(argc!=2){
        printf("ERROR: numero di parametri non corretto");
        exit(1);
    }
    if((pid=fork())<0){
        printf("ERROR: errore nella creazione della fork");
        exit(2);
        }
    if(pid==0){
        int fd;
        printf("DEBUG: esecuzione del processo figlio con pid:%d\n",getpid());
        close(0);
        if((fd=open(argv[1],O_RDONLY))<0){
            printf("ERROR: errore nella apertura del file");
            exit(-1);
        }
        printf("DEBUG: ho aperto il file %s con fd= %d\n",argv[1],fd);
        execl("mycat","mycat",(char*)0);
        printf("ERROR:errore in execl");
        exit(-1);
    }
    if((pidFiglio=wait(&status))<0){
        printf("ERROR: errore nella wait del figlio");
        exit(3);
    }
    if((status & 0xFF) !=0){
        printf("ATTENTION: figlio terminato in modo anomalo");
    }
    else{
        ret=(int)((status>>8) & 0xFF);
        printf("DEBUG: il figlio ha ritornato il valore %d\n",ret);
    }
    exit(0);
}
