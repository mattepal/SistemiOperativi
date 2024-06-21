#!/bin/sh
#File FCP2-11-04-18.sh
#controllo il numero dei parametri
case $# in
0|1|2)
    echo ERROR:$# numero di parametri insufficiente
    exit 1
;;
*)
    echo DEBUG-OK:$# numero di parametri corretto
;;
esac
case $1 in
*/*)	
    echo Errore: $1 non deve contenere il carattere \/
    exit 2
;;
*)
;;
esac
D=$1 # nome specificato nel testo
shift
for i 
do 
case $i in
/*)
    if test ! -d $i -a ! -x $i
    then
        echo ERROR: $i è una cartella non esistente o non attraversabile
        exit 3  
    fi
    ;;
*)
    echo ERROR: $i non nome assoluto
    exit 4
    ;;
esac
done   
#aggiugo la directory corrente al path e la esporto
PATH=`pwd`:$PATH
export PATH
# creo un file temporaneo
> /tmp/tmp$$

#chiamo il file FCR per ogni parametro G passato dall'utente
for G
do 
    echo DEBUG: esploro la directory $G
    FCR2-11-04-18.sh $G $D /tmp/tmp$$
done

#stampo il numero di file trovati e creati
echo DEBUG: il numero di file creati è `wc -l </tmp/tmp$$`
#esploro il file temporaneo per far visualizzare in modo chiaro il suo contenuto
for i in `cat /tmp/tmp$$`
do 
    case $i in
    *.NO*) #caso in cui non sia presente la quintulitma
    echo il file non conteneva una quintultima riga, creato $i 
    ;;
    *) #caso opposto
    echo il file conteneva la quintultima riga, creato $i che contiene:
    cat <$i    
    esac
done





#cancello il file temporaneo
rm /tmp/tmp$$