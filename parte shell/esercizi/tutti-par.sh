#!/bin/sh
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
	echo "parametro=$i"
done

