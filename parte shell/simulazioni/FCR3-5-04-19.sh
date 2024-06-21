#!/bin/sh

cd $1
NC=
if test -f $2.txt -a -r $2.txt -a -w $2.txt
then
    NC=`wc -m <$2.txt`
    echo $NC `pwd`/$2.txt > $3

fi
for i in *
do 
if test -d $i -a -x $i 
then
    FCR-5-04-19.sh `pwd`/$i $2 $3
fi
done