#!/bin/sh
# controllo il numero dei parametri
case $# in
0|1|2) 
    echo ERROR: numero di paramtri non accettabile
    exit 1
;;
*)
    echo DEBUG-OK: numero di parametri corretto
;;
esac
case $1 in
*/*)	
    echo Errore: $1 non deve contenere il carattere \/
    exit 2
;;
*)
;;
esac
F=$1 # nome specificato nel testo
shift
params=
for i 
do
case $i in
/*) 
    if test ! -d $i -a  ! -x $i
    then
        echo ERROR: il parametro $i non è una directory esistente o attraversabile 
        exit 3
    fi;;
*) 
    echo ERROR: parametro $i non è un nome assoluto
    exit 4
;;
esac
params="$params $i"
done
# aggiugo la directory corrente al path e la esporto
PATH=`pwd`:$PATH
export PATH
# creo un file temporaneo
> /tmp/tmp$$

for i in $params
do 
    FCR1-5-04-19.sh $i $F /tmp/tmp$$
done
cnt=1
echo il numero di file trovati è `wc -l </tmp/tmp$$`
for i in `cat /tmp/tmp$$`
do
    if test `expr $cnt % 2` -ne 0
    then
        echo il file trovato è $i 
        nomefile=$i
    else
        echo la lunghezza in linee è $i
        echo inserisci un numero compreso tra 1 e $i 
        read X
        if test $X -gt $i
        then
            echo ERROR:numero inserito errato
            exit 7
        fi
        echo le prime $X linee sono :
        head -$X $nomefile
    fi
cnt=`expr $cnt + 1`
done

#cancello il file temporaneo
rm /tmp/tmp$$


