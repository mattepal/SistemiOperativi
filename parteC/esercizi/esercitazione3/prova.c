/*file prova.c*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    char *param[]={"./prova",NULL};
    int x;
    
    printf("DEBUG: esecuzione del programma\n");
    printf("inserisci il valore zero se vuoi terminare il programma \n");
    scanf("%d",&x);
    if(x!=0){
        printf("DEBUG: sto per eseguire %s\n",param[0]);
        execvp(param[0],param);
        printf("ERROR: errrore in execvp\n");
        exit(1);
    }
    else{
        exit(0);
    }
}