ù#!/bin/sh
cont=0
case $# in
0)
	echo "numero di parametro insufficente"
	exit 1
;;
*)
	echo "numero parametri corretto "
;;
esac
echo "il numero di parametri è $#"
for i
do	
	cont=`expr $cont + 1`
	echo "parametro \$$cont=$i"
done

