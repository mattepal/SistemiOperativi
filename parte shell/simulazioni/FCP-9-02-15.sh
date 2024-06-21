#!/bin/sh
if test $# -ne 2
then 
    echo ERROR:il numero di parametri non è corretto
    exit 1
fi
case $1 in
/*)
    if test ! -d $1
    then 
        echo ERROR: directory inesistente
        exit 2
    fi
    if test ! -x $1
    then
        echo ERROR: directory non attraversabile
        exit 3
    fi
;;
*)
    echo ERROR: il parametro $1 non è un nome assoluto di directory
    exit 4
;;
esac
case $2 in
*[!0-9]*)
    echo ERROR: il parametro $2 non è un valore intero positivo
    exit 5
;;
*)
    if test $2 -eq 0
    then 
    echo ATTENTION: il parametro $2 equivale al valore zero
    exit 6
    fi
;;
esac
PATH=`pwd`:$PATH
export PATH
FCR-9-02-15.sh $*