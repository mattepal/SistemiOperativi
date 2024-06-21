#!/bin/sh
case $# in
0) #caso di parametri passati non sufficienti
	echo "ERROR: numero di parametri insufficente" #stampo errore 
	exit 2;;
1)
if test ! -f $1 #controllo il parametro passato sia un file
then
 	echo "ERROR: il parametro passato non è un file esistente"  #se non è un file stampo l'errore
	exit 1
fi
echo "inserisci e termina con ctr-d"
cat>>$1
;;
2)
if test ! -f $1 -o ! -f $2 #controllo dei parametri passati siano file
then 
	echo "ERROR:uno dei due file non è esistente" #se non esiste uno dei due file errore 
	exit 1
fi
cat $1>>$2
;;
esac
