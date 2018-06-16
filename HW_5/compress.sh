#!/bin/bash
#
# Esercizio 6
#

if [ $# -ne 2 ]; then
    echo "ERRORE: usa: `basename $0` dir size" 1>&2
    exit -1
fi

if [ ! -d $1 ]; then
    echo "ERRORE: $1 non è una directory" 1>&2
    exit 1
fi


r=$(find $1 -type f \( -size +${2}M -atime +5 \) -exec gzip {} \; -print)
if [ $? -ne 0 ]; then
    echo "ERRORE nell'esecuzione del comando find" 1>&2
fi
