#!/bin/sh
if test $# -ne 1  #controllo che il numero di parametro sia corretto
then 
	echo $# non è il numero giusto di parametri #se il numro di paramtri errato stampo errore
	exit 1 #termino esecuzione
fi
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
fi

