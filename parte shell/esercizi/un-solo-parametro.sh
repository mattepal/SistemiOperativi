#!/bin/sh
if test $# -ne  1 
then
	echo numero di parametri diverso da 1  #codice di errore per paramtri diverso
	exit 1
fi
case $1 in
/*)
	echo " parametro in forma assoluta"
;;
*/*)
	echo "parametro in forma relativa"
;;
*)
	echo "parametro in forma relativa semplice"
;;
esac
