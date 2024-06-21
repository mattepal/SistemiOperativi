#!/bin/sh
case $# in
0)
echo numero di parametri insufficente
exit 1
;;
*)
;;
esac
first=$1
shift
echo il primo parametro=$first
echo il resto dei parametri= $*
