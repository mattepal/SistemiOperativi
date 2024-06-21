#!/bin/sh
if test $# -lt 3 #controllo che il numero di parametri sia corretto
then 
    echo ERROR: il numero di parametri non è corretto #stampo errore se il numero è minore di 3
    exit 1
fi
echo ciao
num=1 #contatore percontrollare che il numero del parametro
params=
x=
for i
do 
    if test $num -ne $# #controllo per arrivare fino al penultimo parametro e escludere il numero intero
    then
        case $i in #controllo che la directory sia in nome assoluto
        /*)
            if test ! -d $i 
            then 
                echo ERROR: il nome del parametro $i non è un directory esistente
                exit 2
            fi
            if test ! -x $i
            then
                echo ERROR: la directory $i non è attraversavile 
                exit 3
            fi
        ;;
        *) 
            echo ERROR: $i non è un nome assoluto di direcoty
            exit 4
        ;;
        esac 
        params="$params $i"
    else 
        case $i in
        *[!0-9]*)
            echo ERROR: il parametro $i non è un numero intero positvo
            exit 5
        ;;
        *)
            if test $i -eq 0
            then
                echo ATTENTION: il parametro $i equivale a zero
                exit 6
            fi
        ;;
        esac
        x=$i
    fi
    num=`expr $num + 1`
done    
PATH=`pwd`:$PATH
export PATH
>>/tmp/conta$$
for i in $params
do 
    echo ricorsione in $i
    FCR-15-03-16.sh $i $x /tmp/conta$$
done
echo il numero di directory che soddisfano i requisisti sono: `wc -l< /tmp/conta$$`
for i in `cat /tmp/conta$$`
do 
    echo trovata la directory $i contenente;
    cd $i
    for file in *
    do 
    echo "-f `pwd`/$file" 
    echo la $x-esima riga è `tail -$x $file | head -1` 
    done

done
rm /tmp/conta$$

