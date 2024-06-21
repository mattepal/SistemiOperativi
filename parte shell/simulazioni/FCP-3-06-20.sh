#!/bin/sh
if test $# -le 2
then 
    echo ERROR: passati un numero di parametro non corretto $#
    exit 1
fi
case $1 in
?)
;;
*)
    echo ERROR: il parametro $1 non è un carattere singolo
    exit 2
;;
esac
c=$1
shift
param=
for i 
do 
    case $i in
    /*)
        if test ! -d $i -a ! -x $i
        then
            echo ERROR il parametro $i non è una directory esistente o attraversabile
            exit 3
        fi
    ;;
    *)
        echo ERROR: il parametro $i non è un nome assoluto di direcoty
        exit 4
    ;; 
    esac
    param="$param $i"
done

PATH=`pwd`:$PATH
export PATH
>>/tmp/nomiAssoluti

for i in $param
do
    FCR-3-06-20.sh $i $c /tmp/nomiAssoluti
done

echo il numero di directori trovate è `wc -l </tmp/nomiAssoluti`
for i in `cat /tmp/nomiAssoluti`
do 
    echo la directory trovata è: $i
    echo -n "elena vuoi visualizzare i file trovati inclusi i nascosti?(yes no)"
    read answer
    case $answer in
    Y* |y*)
        echo il contenuto è:
        ls -a $i 
    ;;
    *)
    ;;
    esac
done

rm /tmp/nomiAssoluti