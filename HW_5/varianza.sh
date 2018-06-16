#!/bin/bash
#
# Esercizio 5
#

if [ $# -eq 0 ]; then
    echo "ERRORE: usa: `basename $0` file"
    exit -1
fi

if [ ! -f $1 ]; then
    echo "ERRORE: $1 non è un file regolare"
    exit 1
fi

if [ ! -f ./media.sh ]; then
    echo "ERROR: non trovo lo script media.sh"
    exit 2
fi


MEDIA=$(./media.sh $1 | grep Media | cut -d: -f2)

sum=0
N=0
while read value; do
    sum=$(echo "scale=3; $sum + ($value-$MEDIA)*($value-$MEDIA)" | bc)
    ((N++))
done < $1
echo "Varianza  : "`echo "scale=2; $sum/$N" | bc`
echo "Dev.Stand.: "`echo "scale=2; sqrt($sum/$N)" | bc`
