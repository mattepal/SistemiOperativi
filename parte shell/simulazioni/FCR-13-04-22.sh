#!/bin/sh

cd $1
NR=
for i in *
do
if test -f $i 
then 
    NR=`wc -l <$i`
    if test $NR -eq $2
    then
        echo `pwd`/$i >> $3 
    fi 
fi
done
for i in *
do 
    if test -d $i -a -w $i 
    then 
        $0 `pwd`/$i $2 $3
    fi
done