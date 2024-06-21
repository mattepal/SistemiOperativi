#!/bin/sh

cd $1

NR=

case $1 in
*/$2)
    for i in *
    do 
        if test -f $i -a -r $i
        then
            NR=`wc -l< $i`
            if test $NR -gt 4
            then
                echo `pwd`/$i $NR >> $3 
            fi
        fi
    done
;;
*)
;;
esac

for i in *
do 
    if test -d $i -a -x $i
    then
        $0 `pwd`/$i $2 $3
    fi
done

