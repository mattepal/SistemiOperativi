#!/bin/sh
case $# in
0) #passati zero parametri
	echo "lista dei file e sottodirectory in forma paginata"
	 ls -l | more ;; #stampo file e sottodirectory
1) #passato un parametro
echo SONO DIR.sh
echo 'Il valore di $0 ===>' $0
echo 'Il valore di $1 ===>' $1
if test  -f $1 #controllo che il parametro passato sia un file
then 
	echo "DEBUG-Ora eseguo ls-l $1"
	ls -l $1
fi
if test -d $1  #controllo se il parametro è una directory
then
	echo "DEBUG-Ora eseguo ls -d $1" 
	ls -d $1
fi;;
*) #passato un numero  maggiore di uno 
	echo numero di parametri errato #scrivo l'errore 
;;
esac
