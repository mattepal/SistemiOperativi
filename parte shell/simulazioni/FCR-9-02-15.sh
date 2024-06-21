#!/bin/sh

cd $1

NR=
for i in *
do 
if -f $i -a -r $i
then
    NR= `wc -l<$i` 
    if NR -eq $2
    then

    fi
fi
done