#!/bin/sh
if test $# -ne 1  #controllo che il numero di parametro sia corretto
then 
	echo $# non è il numero giusto di parametri #se il numro di paramtri errato stampo errore
	exit 1 #termino esecuzione
fi
echo SONO DIR.sh
echo 'Il valore di $0 ===>' $0
echo 'Il valore di $1 ===>' $1
echo "DEBUD-Ora eseguo ls -l $1"
ls -l $1
