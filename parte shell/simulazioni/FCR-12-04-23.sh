#!/bin/sh
#File FCR-12-04-23.sh
#mi sposto nella cartella richiesta
cd $1
#ciclo che scorre tutti gli elementi della cartella corrente
for i in *
do
if test -f $i -a -r $i #controllo che sia un file e che sia leggibile
then
case $i in #controllo che sia con l'estensione cercata
*.$2)
    echo `pwd`/$i >> $4 #salvo il nome assoluto del file nel temporaneo
;;
*.$3)
    echo `pwd`/$i >> $5 #salvo il nome assoluto del file nel temporaneo
;;
*)
;;
esac   
fi
done

#esploro ricorsivamente le sottocartelle
for F in *
do
if test -d $i -a -x $i
then
    $0 `pwd`/$F $2 $3 $4 $5
fi
done