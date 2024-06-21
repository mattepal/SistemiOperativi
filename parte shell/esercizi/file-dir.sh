#!/bin/sh

cd $1
f=0
d=0
for i in *
do
	if test -f $i
	then
		echo "[F] `pwd`/$i"
		f=`expr $f + 1`
	fi
	if test -d $i
	then
		echo "[D] `pwd`/$i"
		d=`expr $d + 1`
	fi
done

if test $f -eq 0 -a $d -eq 0
then 
	echo "ATTENTION:la cartella aperta è vuota"
	exit 6
fi
echo numero di file=$f
echo numero di directory= $d
