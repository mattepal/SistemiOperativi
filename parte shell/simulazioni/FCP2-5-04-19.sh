#!/bin/sh
# controllo il numero dei parametri
case $# in
0|1|2)
    echo ERROR: numero di paramtri non accettabile
     exit 1;;
*)
    echo DEBUG-OK: numero di parametri corretto
;;
esac
case $1 in
*/*)	
    echo Errore: $1 non deve contenere il carattere \/
    exit 1
;;
*)
;;
esac
D=$1 # nome specificato nel testo
shift
params=
for i 
do
case $i in
/*)
    if test ! -d $i -a ! -x $i
    then
        echo ERROR: il parametro $i non è una direcory esistente o attraversabile
    fi
;;
*)
    echo ERROR: il parametro $i non è un nome assoluto
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
    FCR2-5-04-19.sh $i $D /tmp/tmp$$
done

echo numero di file trovati: `wc -l </tmp/tmp$$`

cnt=1
for i in `cat /tmp/tmp$$`
do 
    if test `expr $cnt % 2` -eq 0
    then 
        echo il file trovato è: $i
    else
    echo il numero di righe è: $i
    fi
done













#cancello il file temporaneo
rm /tmp/tmp$$