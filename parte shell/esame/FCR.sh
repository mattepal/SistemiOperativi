#!/bin/sh
#File FCR.sh
#DirecotryH stringa1 file-Tmp
cd $1 #ci posizioniamo nella directory giusta

FP=0 #parametro che viene implementato ogni volta che si incontra un file per il controllo della posizione di esso
found=false  #parametro inizialmente false che diventa true in caso si trovi il file in 4 posizione 

#ciclo che scorre tutti gli elementi della cartella corrente
for item in *
do
    #controllo che l'elemento selezionato sia un file
    if test -f $itme
    then
        FP=`expr $FP + 1` #incremento il valore che rappresenta la posizione di un file
        if test $FP -eq 4 #controllo che sia il file in 4arta posizione
        then
            echo DEBUG: trovato file in 4arta posizione nella directory $1
            echo `pwd`/$item >> $3 #salvo il nome assoluto del file dentra al temporaneo
            found=true #cambio il valore a found dato che è stato trovato il file in 4arta posizione
        fi
    fi
done
#controllo sia cambiato il valore di found e quindi sia stato trovato il file in 4arta posizione
if test $found = false
then
    echo ATTENTION: nella directory $1 nessun file è stato trovato in 4arta posizione # se non è stato cambiato stampo come richiesto il nome assoluto con spiegazione
fi

for i in *
do 
    if test -d $i -a -x $i
    then
        $0 `pwd`/$i $2 $3
    fi
done