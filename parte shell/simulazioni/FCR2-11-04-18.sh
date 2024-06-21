#!/bin/sh
#File FCR2-11-04-18.sh
cd $1 #ci posizioniamo nella directory giusta
NR= #variabile che salva il numero di righe dei file leggibili presenti nella cartella $1 

#controllo che la cartella corrente abbia il nome cercato dall'utente
case $1 in
*/$2)
    for i in * #caso sia così scorro tutti i file e cartellle
    do
        if test -f $i -a -r $i #controllo che sia un file e sia leggibile
        then
            NR=`wc -l < $i` #assegno il valore a NR contando le righe di un file
            if test $NR -gt 5 #controllo che il numero di righe sia maggiore o uguale a 5
            then
               
                tail -5 $i | head -1 >$i.quintultima #creo e salvo il file .quintultima richiesto
                echo `pwd`/$i.quintultima >> $3  
            else
                >$i.NOquintultima #in caso non abbia 5 righe creo e salvo il file .NOquintultima
                echo `pwd`/$i.NOquintultima >>$3
            fi   
        fi
    done
;;
*);;
esac

#creo il for che ricerchi le cartelle per esplorare le sottocartelle tramite ricorsione
for d in *
do
if test -d $d -a -x $d
then
    $0 `pwd`/$d $2 $3
fi
done
