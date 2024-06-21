#!/bin/sh
if test $# -ne 1
then 
	echo "ERROR: numero di parametri non corretti"
	exit 1
fi
case $1 in
/*)
	echo "parametro corretto"
	if test ! -d $1 -o ! -w $1 
	then 
		echo "ERROR:il parametro non è una directory"
	exit 4
	fi
	if test ! -x $1
	then 
		echo "ERROR:il parametro non è una directory attraversabile"
	exit 5
	fi
	echo "la directory è attravrsabile"
;;
*)
	echo "il parametro passatto non è un nome assoluto"
exit 3
;;
esac

sh file-dir.sh $1
