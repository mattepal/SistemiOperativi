#!/bin/sh
#File FCP.sh
#controllo il numero dei parametri
#necessita almeno 3 parametri
case $# in
0|1|2)
    echo ERROR:$# numero di parametri insufficiente
    exit 1
;;
*)
    echo DEBUG-OK:$# numero di parametri corretto
;;
esac
# stringa1 directory1 directory2 ... directoryK
#controllo del primo parametro che sia stringa e salvataggio nella variabile K
case $1 in
*/*)	
    echo Errore: $1 non è un nome relativo semplice, non deve contenere il carattere \/
    exit 2;;
esac
echo DEBUG-OK: $1 è un nome relativo semplice
w=$1 # nome specificato nel testo
shift #shifto i prametri di ingresso in modo di avere in $* solo i probaili nomi di directory assolute

#controllo il restante dei parametri tramite un ciclo che li scorre 
for K
do
  case $K in
  /*)
    if test ! -d $K -a ! -x $K #controllo che esista come directory e sia attraversabile
    then
        echo ERROR: $K è un nome di una directory inesistente o non attraversabile
        exit 3
    fi
    ;;
  *)
    echo ERROR: $K non nome assoluto #stampa di errore in caso sia sbagliato il parametro
    exit 4
    ;;
  esac  
done
echo DEBUG-OK: resto dei parametri ok, nomi assoluti

#aggiugo la directory corrente al path e la esporto
PATH=`pwd`:$PATH
export PATH
# creo un file temporaneo
> /tmp/tmp$$

#ciclo che scorre tutte le directory assolute e chiama il file ricorsivo FCR
for H
do 
    echo fase per $H
    FCR.sh $H $w /tmp/tmp$$
done

sum=`wc -l < /tmp/tmp$$` #assegno il valore  dei file trovati a sum 
if test $sum -eq 0 #controllo che sum non sia zero quindi il file tmp non sia vuoto
then 
    echo ERROR: nessun file è stato trovato in 4arta posizione
    exit 5
fi
#stampo il numero di file trovati in caso l'if non vada a buon fine
echo "\nDEBUG: sono stati trovati $sum file in 4arta posizione \n"
#creo la cartella
mkdir -p $w
cd $w #mi sposto nella cartella creata 
for i in `cat /tmp/tmp$$`
do
    ln -s $i `pwd` #creo il link software 
done


#cancello il file temporaneo
rm /tmp/tmp$$