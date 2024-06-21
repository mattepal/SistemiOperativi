#!/bin/sh
if test $# -le 3
then
    echo ERROR: numero di paramtri errati $#
    exit 1
else
    echo DEBUG: numero di parametri corretto
fi
case $1 in 
*[!0-9]*)
    echo ERROR: il parametro $1 non è un numero intero positivo
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
echo DEBUG: primo parametro corretto
w=$1
shift
case $1 in 
*/*)
    echo ERROR: il parametro deve essere una stringa e non una directory
    exit 4
;;
*)
;;
esac
echo DEBUG: secondo parametro corretto
s=$1
shift

params=
for i 
do 
case $i in
/*)
    if test ! -d $i -a ! -x $i
    then 
        echo ERROR: il paramerto $i è un nome di una directory inesistente o non attraversabile
        exit 5
    fi
;;
*)
    echo ERROR: il parametro $i non è un nome assoluto
    exit 6
;;
esac
params="$params $i"
done

PATH=`pwd`:$PATH
export PATH
>>/tmp/nomiAssoluti

for x in $params
do 
    FCR-22-04-21.sh $x $w $s /tmp/nomiAssoluti
done

num=`wc -l < /tmp/nomiAssoluti`
echo il numero di directory trovare e: $num
if test $w -lt $num
then
    echo -n "mi dica il suo nome di battesimo "
    read battesimo
    echo -n "salve $battesimo mi dica un numero compreso tra 1 e $w per avere la corrispondete directory "
    read risp
    case $risp in 
    *[!0-9]*)
    echo ERROR: non ha inserito un numero
    exit 7
    ;;
    *)
        if test $risp -eq 0
        then
        echo ATTENTION: ha passato il valore zero
        rm /tmp/nomiAssoluti
        exit 8
        fi
        if test $risp -gt $w
        then
        echo ERROR: passato un valore maggiore di $w
        rm /tmp/nomiAssoluti
        exit 9
        fi
    esac
    echo il nome della directory corrispondente a $risp è:
    head -$risp </tmp/nomiAssoluti|tail -1 
else 
echo $w è maggiore di $num
fi

rm /tmp/nomiAssoluti