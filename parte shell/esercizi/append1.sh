#!/bin/sh
if test $# -ne 2
then 
	echo "numero di paramtri errati"
	exit 1
fi
for i
do
case $i in
*/*)
	echo " paramtero $i non è corretto"
	exit 2
;;
*)
	echo "parametro corretti"
;;
esac
done
	if test ! -f $1 -o ! -r $1 
	then
	echo "il file $1 non esiste o non è leggibile" 
	exit 3
	fi
	if test ! -f $2 
	then
		if test ! -w .
		then
			echo "la cartella non è scrivibile"
			exit 4
		fi 
	else
	if test ! -w $2
	then 
		echo "il file $2 non è scrivibile"
		exit 5 
	fi
	fi
cat<$1>>$2
