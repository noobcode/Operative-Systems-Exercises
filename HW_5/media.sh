#!/bin/bash
#
# Esercizio 4
#

if [ $# -eq 0 ]; then
    echo "ERRORE: usa: `basename $0` file"
    exit -1
fi

if [ ! -f $1 ]; then
    echo "ERRORE: $1 non è un file regolare"
    exit 1
fi

function max() {
    if [ $1 -gt $MAX ]; then 
	MAX=$1
    fi
}
function min() {
    if [ $1 -lt $MIN ]; then
	MIN=$1
    fi
}


n=0
sum=0
# setto il valore iniziale per MIN e MAX
MIN=$(head -1 $1)
MAX=$MIN

while read line; do   
    ((n++))
    sum=$((sum + ((line)) ))
    max $line
    min $line
done < $1

MEDIA=$(echo "scale=2; $sum/$n" | bc)

echo "Elementi:$n"
echo "Min     :$MIN"
echo "Max     :$MAX"
echo "Media   :$MEDIA"
