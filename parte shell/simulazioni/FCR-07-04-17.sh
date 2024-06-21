#!/bin/sh

cd $1

if test -f $2 -a -r $2
then
    sort -f $2 > sorted
    echo `pwd`/sorted >>$3
fi

for i in *
do 
if test -d $i -a -x $i
then
    $0 `pwd`/$i $2 $3
fi
done