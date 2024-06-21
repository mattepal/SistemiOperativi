#!/bin/sh

cd $1

case $1 in 
$2?$2)
    pwd>>$3
;;
*)
;;
esac
for i in *
do 
    if test -d $i -a -x $i
    then
        FCR-3-06-20.sh $i $2 $3
    fi
done