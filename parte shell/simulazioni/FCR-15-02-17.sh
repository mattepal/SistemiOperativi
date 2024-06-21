#!/bin/sh

cd $1
#varibile del numero di righe
NR=
#variabile che contiene il numero di righe contenente un numero
N=
#varibile per memorizzare i file
FILES=

#ciclo per esplorare i file nella directory
for i in *
do 
    if test -f $i -a -r $i
    then 
        NR=`wc -l < $i`
        N=`grep '[0-9]' $i | wc -l`
        if test $NR -eq $2 -a $N -eq $NR
        then 
            FILES="$FILES $i"
        fi
    fi
done
#controllo siano stati trovati dei file
if test "$FILES"
then
    echo "`pwd`"
    echo "$FILES"
fi
#cerco cartelle per la ricorsione
for i in *
do 
    if test -d $i -a -x $i
    then
    echo "ricorsione in `pwd`/$i"
        $0 `pwd`/$i $2 
    fi
done