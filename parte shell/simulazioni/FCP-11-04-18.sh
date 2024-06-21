#!/bin/sh
if test $# -le 2 #controllo il numero di parametri iniziali
then 
    echo ERROR: numero di parametri errato #messaggio se il numero di parametri errato
    exit 1
fi
echo DEBUG: numero parametro corretto #messaggio di numero parametri corretti
direct=
# ciclo for che esplora tutti i parametri di input iniziali
case $1 in 
*[!0-9]*) #controllo che l'ultimo parametro non sia un intero positivo
    echo ERROR: il parametro $1 non è un numero intero positivo #messagigo di errore
    exit 5
;;
*)
    if test $1 -eq 0
    then
        echo ATTENTION: il parametro $1 equivale a zero
        exit 6 
        fi
;;
esac
x=$1
shift
for i 
do   
    case $i in
    /*) #controllo che il nome sia assoluto
        if test ! -d $i -a ! -x $i  #controllo che la cartella passata sia esistente e attraversabile
        then
            echo ERROR: il parametro $i non è un directory o non è attraversabile 
            exit 2
        fi
    ;;
    *) #caso in cui il parametro non sia un nome assoluto
        echo ERROR:il parametro $i non è un nome assoluto #messaggio di errore
        exit 4
    ;;
    esac
    direct="$direct $i"
done
PATH=`pwd`:$PATH
export PATH
>>/tmp/conta$$
for i in $direct
do 
    FCR-11-04-18.sh $i $x /tmp/conta$$
done
echo il numero di file creati sono: `ec -l </tmp/conta$$`
for i in `cat /tmp/conta$$`
do
    echo trovato il file $i
    case $i in 
    *[NO]*)
        echo il file non ha 5 righe
    ;;
    *)
        echo il file ha 5 righe 
        cat $i
    ;;
    esac
done
rm /tmp/conta$$