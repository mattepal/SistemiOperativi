#!/bin/sh
 cd $1
#variabile che conta il numero di righe 
NR=0
#variabile che controlla la presenaza di soli file
OF=true
#parametro che controlli la presenza di file con righe giuste
ML=true
#controllo della directory
for i in *
do
    if test -f $i
    then 
        NR=`wc -l <$i`
        if test $NR -le $2 
        then 
            MR=false
        fi
    else 
        if test -d $i
        then 
            OF=false
        fi
    fi
done
if test $OF=true -a $ML=true -a $NR -ne 0
then
    pwd>>$3
fi
for i in *
do
if test -d $i -a -x $i
then
FCR-15-03-16.sh `pwd`/$i $2 $3
fi
done