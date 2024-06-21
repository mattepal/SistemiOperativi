#!/bin/sh
#File FCR-12-04-13.sh
cd $1 #ci posizioniamo nella directory giusta
trovato=0
for i in *
do 
   if test -f $i -a -r $i
   then 
        case $i in
        *.$2)
            trovato=`expr $trovato + 1`
        ;; 
        esac
   fi
done
if test $trovato -ne 0
then
    pwd>>$4
fi

for i in * 
do 
    if test -d $i -a -x $i
    then
    echo ricorsione in $i
        FCR-12-04-13.sh `pwd`/$i $2 $3 $4
    fi
done