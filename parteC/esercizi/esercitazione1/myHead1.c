/*file myHead1.c*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    int n;
    char c;
    if(argc!=2){
        printf("ERROR:numero di parametri errato");
        exit(1);
    }
    if(argv[1][0]!='-'){
        printf("ERROR: il parametro %s deve inizare con (-)",argv[1]);
        exit(2);
    }
    n=atoi(&(argv[1][1]));
    if(n<=0){
        printf("ERROR: a seguito del - va in serito un numero intero positivo");
        exit(3);
    }
    int i=1;
    while (read(0,&c,1))
    {
        if(c=='\n'){
            i++;
        }
        write(0,&c,1);
        if(i>n){
            break;
            }
    }
    
    exit(0);
}
