#!/bin/sh
if test $# -le 2
then
    echo ERROR: numero di parametri passati errato $#
    exit 1
fi
case $1 in
*[!0-9]*)
    echo ERROR: il parametro $1 non è un intero positivo
    exit 2
;;
*)
    if test $1 -eq 0
    then 
    echo ATTENTION: il parametro $1 ha valore zero
    exit 3
    fi
;;
esac
echo DEBUG: paramtro $1 corretto
x=$1
shift 
params=
for i
do
case $i in
/*)
    if test ! -d $i -a ! -x $i 
    then
    echo ERROR: il parametro $i è una direcory inesistente o non attraversabile 
    exit 4
    fi
;;
*)
    echo ERROR: il parametro $i non è un nome assoluto
    exit 5
;;
esac
params="$params $i"
done

PATH=`pwd`:$PATH
export PATH

n=1

for i in $params
do
    >/tmp/nomeAssoluti$$-$n
    FCR-13-04-22.sh $i $x /tmp/nomeAssoluti$$-$n
    echo il numero di file è `wc -l < /tmp/nomeAssoluti$$-$n`
    n=`expr $n + 1`
done

for file1 in `cat /tmp/nomeAssoluti$$-1`
do
    n=1
    for i 
    do
        if test $n -gt 1
        then
        for file2 in `cat /tmp/nomeAssoluti$$-$n`
        do 
            echo controllo del file $file1 e $file2
            if diff $file1 $file2 > /dev/null 2>&1
            then 
                echo i file sono uguali
            else 
                echo i file sono differenti
            fi
        done
        fi
    done
    n=`expr $n + 1`
done

n=1
for g
do 
rm /tmp/nomeAssoluti$$-$n
n=`expr $n + 1`
done