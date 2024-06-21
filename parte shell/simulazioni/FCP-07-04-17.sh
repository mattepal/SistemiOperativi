#!/bin/sh
if test $# -le 2
then
    echo ERROR: il numero di parametri passati non è sufficente $#
    exit 1    
fi
echo DEBUG: numero di parametri corretto
case $1 in
*/*)
    echo ERRORE: il parametro $1 deve essere un nome di un file semplice
    exit 2
;;
*)
;;
esac   
x=$1
shift
params=
for i 
do 
    case $i in
    /*)
        if test ! -d $i -a ! -x $i
        then
            echo ERROR: il parametro $i è una directory inesistente o non attraversabile
            exit 3
        fi
    ;;
    *)
        echo ERROR: il parametro $i non è un nome assoluto
        exit 4
    ;;
    esac
    params="$params $i"
done

PATH=`pwd`:$PATH
export PATH
>>/tmp/conta$$

for i in $params
do
    FCR-07-04-17.sh $i $x /tmp/conta$$
done

echo il numero di file creati è `wc -l /tmp/conta$$`

for i in `cat /tmp/conta$$`
do 
    echo creato il file $i
    echo "la prima riga del file è `head -n 1 $i`"
    echo "l'ultima riga del file è `tail -n 1 $i`"
done
rm /tmp/conta$$