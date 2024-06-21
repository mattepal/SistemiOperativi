#!/bin/sh
cd $1
if test -d $2
then 
echo "esiste la cartella $2"
fi
for i in *
do
if test -d $1 -a -x $1
then 
	echo esplorazione di $1
	sh $0 `pwd`/$1 $2
fi
done
