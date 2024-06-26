#!/bin/sh
#file comandi LeggiEMostra.sh 
#CONTROLLO SU PRESENZA DI ALMENO UN PARAMETRO FATTO IN MODO DIVERSO DA PRIMA
case $# in 
0)
echo numero di parametri errato
;;
*)
echo numero di parametri corretto
;;
esac
#in realta' il controllo precedente non e' un controllo stretto ma lasco e quindi in questo caso lo script accetta almeno un parametro, ma non da' errore nel caso di passaggio di piu' parametri e quindi se si vuole un controllo stretto si deve usare il controllo visto prima per if5.sh
#comunque se si arriva qui vuole dire che e' stato passato almeno un parametro ($1)
#controlliamo che il primo parametro sia un file e sia leggibile!
for $i
do
if test ! -f $i -o ! -r $i
then 	echo Errore: $i non file oppure non leggibile
	exit 2
fi
#se arriviamo qui vuol dire che $1 e' un file esistente E leggibile!
#chiediamo all'utente se vuole visualizzare il contenuto del file: N.B. forziamo l'output sul terminale corrente per non avere problemi se si usasse la ridirezione dello standard output
echo "vuoi visualizzare il file $1 (si/no)?" > /dev/tty
#leggiamo la risposta dell'utente
read var1
#se l'utente ha risposto 'si' allora procediamo con visualizzazione delle info e del file
case $var1 in	#ATTENZIONE AD INSERIRE GLI SPAZI PERCHE' E' UNA VALUTAZIONE SE LE DUE STRINGHE SONO UGUALI!
yes | si | SI | Si | Yes | YES)
       	echo info del file $i 	
	ls -la $i	#potrebbe anche essere un nome che inizia con . e quindi usiamo anche -a
	echo contenuto del file $i
	cat $i		#si potrebbe anche usare cat < $1
;;
*) 	echo niente stampa di $i
;;
esac
done

