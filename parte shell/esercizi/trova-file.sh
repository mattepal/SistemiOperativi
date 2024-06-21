#!/bin/sh
#file trova-file.sh
case $# in
0)
	echo "ERROR:passati pochi parametri" #errore se il numero di parametri é minore 
	exit 1
;;
1)
	echo "numero di parametri corretto" 
;;
*)
	echo "ERROR:passati troppi parametri" #errore se il numero di parametri è maggiore di 1 
	exit 2
;;
esac
case $1 in 
*/*) #controllo il parametro sia in relativo o assolito
	echo "ERROR:il parametro passato non è relativo semplice" #stampo l'errore
	exit 3
;;
*)
if test -f $1 #controllo l'esistenza del file
then 
	echo "il nome assoluto è:`pwd`/$1" #stampo il nome assoluto del file
fi
;;
esac

