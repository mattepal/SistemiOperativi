#!/bin/sh
#File FCP-12-04-13.sh
#controllo il numero dei parametri
case $# in 
3)  #controllo stretto su 3
    echo DEBUG-OK:$# numero di parametri corretto

;;
*) #stapo l'errore
    echo ERROR:$# numero di parametri insufficiente
    exit 1
;;
esac
#controllo del primo parametro
case $1 in #controllo sia nome assoluto
/*)
    if test ! -d $1 -a ! -x $1 #controllo sia una cartella e attraversabile
    then
        echo ERROR: il parametro $1 non è una cartella esistente o attraversabile
        exit 2
    fi
    ;;
*)
    echo ERROR: $1 non nome assoluto
    exit 3
    ;;
esac
case $2 in
*/*)
	echo Errore: $2 non deve contenere il carattere \/
    exit 4;;
esac
case $3 in
*[!0-9]*)
    echo ERROR: $3 non numerico
    exit 5
   ;;
*)
    if test $3 -eq 0
    then 
        echo ATTENTION: passato il paramtro $3 con valore in 0
        exit 1
    fi
    echo DEBUG OK: $3 ok
;;
esac
echo Fase A
#aggiugo la directory corrente al path e la esporto
PATH=`pwd`:$PATH
export PATH
# creo un file temporaneo
> /tmp/tmp$$
FCR-12-04-13.sh $* /tmp/tmp$$

echo numero di cartelle trovate: `wc -l < /tmp/tmp$$`
echo nomi assoluti delle cartelle:
cat < /tmp/tmp$$
DIR=`wc -l </tmp/tmp$$`
echo $DIR
if test $DIR -gt $3
then
echo fase B
echo inserisci un numero compreso tra 1 e $3
read N
if test $N -gt $3
then
echo ERROR: numero troppo grande
rm /tmp/tmp$$
exit 1
fi
case $N in
*[!0-9]*)
    echo ERROR: $N non numerico
    rm /tmp/tmp$$
    exit 2
    ;;
*)
    if test $N -eq 0
    then
        echo ATTENTION: inserito il valore 0
        rm /tmp/tmp$$
        exit 7
    fi
    echo DEBUG OK: $N ok
    ;;
esac
params=`head -$N /tmp/tmp$$| tail -1`
echo selezionata la cartella: $params
cd $params
for i in *
do 
case $i in 
*.$2)
    echo file: $i 
    echo prima linea : `head -1<$i `
;;
esac
done

fi

#cancello il file temporaneo
rm /tmp/tmp$$
