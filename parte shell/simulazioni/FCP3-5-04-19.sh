#!/bin/sh
if test $# -le 2
then 
    echo ERROR:numero di parametri errati
    exit 1
fi
echo DEBUG: passati $# parametri
NUM=1
params=
for i 
do
    if test $NUM -ne $#
    then
        case $i in 
        /*)
            if test ! -d $i -a ! -x $i 
            then 
                echo ERROR: il parametro $i non è una directory esistente o attraversabile
                exit 2
            fi 
        ;;
        *)
            echo ERROR: il parametro $i non è un nome assoluto di directory
            exit 3
        ;;
        esac
    params="$params $i"
    else
        case $i in
        */*)
            echo ATTENTIO: inserirsci il valore di una stringa semplice
            exit 4
        ;;
        *)
        ;;
        esac
        x=$i
    fi
    NUM=`expr $NUM + 1`
done
PATH=`pwd`:$PATH
export PATH
>>/tmp/conta$$
for i in $params
do
    FCR3-5-04-19.sh $i $x /tmp/conta$$
done
cnt=0
echo i file trovati sono in totale: `wc -l < /tmp/conta$$`
for i in `cat /tmp/conta$$`
do 
    cnt=`expr $cnt + 1`
    if test `expr $cnt % 2` -eq 1
    then 
        echo numero di righe= $i 
    else 
        echo percorso del file= $i
        echo -n "vuoi ordinare il file? (yes no)"
        read risposta
        case $risposta in
        y* | Y*)
            sort -f $i 
        ;;
        *)
        echo nessun ordinamento
        ;;
        esac
    fi

done

rm /tmp/conta$$