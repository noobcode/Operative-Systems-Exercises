#!/bin/bash
#
# Esercizio 3
#
#

if [ $# -lt 3 ]; then
    echo "ERRORE: usa: `basename $0` dir1 dir2 dir3"
    exit -1
fi

if [[ ! -d $1 || ! -d $2 || ! -d $3 ]]; then
    echo "ERRORE: \"$1\" e/o \"$2\" e/o \"$3\" non sono directory"
    exit 1
fi

for line in $(diff -r -q -N $1 $2); do
    if [[ $line =~ dir1/.* ]]; then
	cp $line $3
    fi
done
