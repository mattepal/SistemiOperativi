#!/bin/sh

cd $1

NR=

if test -f $2 -a -r $2
then
    NR=`wc -l < $2`
    if test $NR -ge 4
    then
        echo `pwd`/$2 $NR >> $3
    fi
fi
for i in *
do
    if test -d $i -a -x $i
    then
        $0 `pwd`/$i $2 $3
    fi
done