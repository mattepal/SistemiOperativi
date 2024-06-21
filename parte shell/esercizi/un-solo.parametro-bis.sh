#!/bin/sh
if test $# -ne  1 #controllo il numero di parametri passati
then
	echo numero di parametri diverso da 1  #codice di errore per paramtri diverso
	exit 1
fi
case $1 in
/*) #controllo  che il parametro sia in nome assoluto
	echo " parametro in forma assoluta"
	if test -f $1 #controllo che il parametro sia un file
	then 
		echo "il paramtro è un file"
	fi
	if test -d $1 -a -x $1 #controllo che il parametro sia una directory traversabile
	then
		echo "il parametro è una directory traversabile"
	fi
	if test ! -d $1 -a ! -f $1 #controll che il parametro non sia ne file ne directory
	then
		echo "non è una direcotry ne file"
	fi
;;
*/*) #controllo che il paramtro si ain forma relativa
	echo "parametro in forma relativa"
;;
*) #controllo che il file sia in forma relativa semplice
	echo "parametro in forma relativa semplice"
;;
esac
