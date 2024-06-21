#!/bin/sh
#controllo del numero di parametri parametro
if test $# -ne 2
then 
    echo "ERROR: non sono stati passati il numero di parametri corretti"
    exit 1
fi
#controllo del primo parametro
case $1 in 
/*)
    if test ! -d $1 #controllo che la variabile sia una directory
    then
        echo "ERROR: il parametro non è una directory"
        exit 2    
    fi
    if test ! -x $1 #controllo che sia una directory attrversabile
    then 
        echo "ERROR: il parametro non è una cartella attraversabile"
        exit 4
    fi
;;
*)
    echo "ERROR; il parametro $1 non è un nome assoluto"
    exit 3
;;
esac
#controllo del secondo parametro
case $2 in 
*[!0-9]*) #caso in cui contenga numeri
    echo "ERROR: il parametro $2 non è un numero intero positivo"
    exit 5
;;
*)
    #controllo che il parametro non sia zero
    if test $2 -eq 0
    then
     echo "ATTENTION: il paramtro $2 è zero" 
    exit 6
    fi
;;
esac
#esporto e setto il valore di path
PATH=`pwd`:$PATH
export path
#effetto la chiamata al file ricorsivo
FCR.sh $*

