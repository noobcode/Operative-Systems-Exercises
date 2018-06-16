#!/bin/bash
# 
# Esercizio 5 
#

if [ $# -lt 2 ]; then
    echo usa: `basename $0` dir key
    exit 1
fi

#controllo che il primo parametro sia una directory
if [ ! -d $1 ]; then
    echo $1 is not a directory
    exit 1
fi

# utilizzo find, stampa sullo standard output
find $1 -type f -exec grep "$2" -l {} \;
