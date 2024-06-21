#!/bin/sh

cd $1
trovato=false
for i in *
do 
if test -f $i 
then
    case $i in
    *.$3)
    trovato=true
    ;;
    *)
    ;;
    esac
fi
done

if test $trovato=true
then
    pwd>>$4
fi

for i in *
do 
    if test -d $i -a -x $i 
    then
     FCR-22-04-21.sh `pwd`/$i $2 $3 $4
    fi
done