#!/bin/sh
#File FCP-12-04-23.sh
# controllo il numero dei parametri 
# stringa1 stringa2 directory1 directory2 ...
#caso di parametri sbagliati stampo errore e termino
case $# in
0|1|2|3)
    echo ERROR: numero di paramtri non accettabile
    exit 1
;;
*)
    echo DEBUG-OK: numero di parametri corretto
;;
esac
#salvo il valore del primo parametro nella variabile S1
#controllo che il primo parametro sia passato correttamente (stringa)
case $1 in
*/*)	
    echo Errore: $1 non deve contenere il carattere \/
    exit 1;;
esac
S1=$1 # nome specificato nel testo
shift #eseguo shift per passare al secondo parametro dato che il primo è salvato in S1

#salvo il valore del secondo parametro nella variabile S2
#controllo che il secondo parametro sia passato correttamente (stringa)
case $1 in
*/*)	
    echo Errore: $1 non deve contenere il carattere \/
    exit 1;;
esac
S2=$1 # nome specificato nel testo
shift #eseguo shift per passare al terzo parametro dato che il secondo è salvato in S2
#in $* ora contengo solo i nomi delle directory

#ciclo che esplora i parametri restatni e controlla che siano nomi assoluti 
for i 
do
case $i in
/*) 
    if test ! -d $i -a ! -x $i #controllo che la directory passata sia esistente e attraversabile
    then
        echo ERROR: la directory $i non esiste o non è attraversabile
        exit 1
    fi;;
*)
    echo ERROR: il parametro $i non è un nome assoluto
    exit 1
 ;;
esac
done
# aggiugo la directory corrente al path e la esporto
PATH=`pwd`:$PATH
export PATH
# creo i file temporanei
> /tmp/nomiassoluti1-$$
>/tmp/nomiassoluti2-$$

#invoco per Q fasi il file FCR in modo da esplorare le gerarchie G
for G 
do
    FCR-12-04-23.sh $G $S1 $S2 /tmp/nomiassoluti1-$$ /tmp/nomiassoluti2-$$
done

#stampo il numero di file con estensione richiesta trovati
echo numero totali di file trovati che terminano con $S1 è `wc -l </tmp/nomiassoluti1-$$`
echo numero totali di file trovati che terminano con $S2 è `wc -l </tmp/nomiassoluti2-$$`

#salvo il valore dei file trovati tramite il numero di righe dei file temporanei nelle variabili TOTn
TOT1=`wc -l </tmp/nomiassoluti1-$$`
TOT2=`wc -l </tmp/nomiassoluti2-$$`
#fase di richiesta e ricerca del nome assoluto
#controllo richiesto nella traccia su TOT1 e TOT2
if test $TOT1 -le $TOT2 -a $TOT1 -ne 0
then
    echo salve Matteo inserisca un numero compreso tra 1 e $TOT1 
    read X #lettura del valore inserito dall'utente
    case $X in #controllo del valore inserito dell'utente
    *[!0-9]*)
        echo ERROR: non hai inserito un numero
        #rimozione dei file temporanei
        rm /tmp/nomiassoluti1-$$
        rm /tmp/nomiassoluti2-$$
        exit 1
    ;;
    *)
        if test $X -eq 0
        then
        echo ATTENTION: il parametro passato equivale a 0 
        #rimozione dei file temporanei
        rm /tmp/nomiassoluti1-$$
        rm /tmp/nomiassoluti2-$$
        exit 1
        fi 
    esac
    #caso in cui tutti i controlli vanno a buon fine ricerco il numero di riga richiesto
    #per entrambe le estensioni dei file
    echo il file leggibile con estensione $S1 che si trova alla linea $X è:
    head -$X </tmp/nomiassoluti1-$$ | tail -1
#   prova=1
#    for i in `cat /tmp/nomiassoluti1-$$`
#    do
#        if test $prova -eq $X 
#        then
#            echo file: $i
#        fi
#        prova=`expr $prova + 1`
#    done
    echo il file leggibile con estensione $S2 che si trova alla linea $X è:
    head -$X </tmp/nomiassoluti2-$$ | tail -1

fi
#cancello il file temporaneo
rm /tmp/nomiassoluti1-$$
rm /tmp/nomiassoluti2-$$
