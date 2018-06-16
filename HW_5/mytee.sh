#!/bin/bash
#
# Esercizio 1
#


if [ $# -eq 0 ]; then
    echo "ERRORE: usa: `basename $0` file"
    exit -1
fi

if [ -e $1 ]; then
    if [ ! -f $1 ]; then
	echo "ERRORE: $1 non è un file regolare"
	exit 1
    fi
fi

while read line; do   
    echo $line
    echo $line >> $1
    if [ ! $0 ]; then
	echo "ERRORE: durante la scrittura in $1"
	exit 2
    fi
done
