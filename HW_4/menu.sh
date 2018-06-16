#!/bin/bash

if [ $# -le 1 ]; then
    echo usa: `basename $0` file1 file2
    exit 1
fi

f1=$1
f2=$2

clear
echo "Scegli tra le seguenti opzioni:"
echo "  1 rimuovere $f1 e $f2"
echo "  2 archiviare $f1 e $f2"
echo "  3 appendere $f1 ad $f2"
echo "  4 quit"
echo ""
echo -n "Scelta: "
read x

case $x in 
    1)
	echo -n "Rimuovo il file $f1 ed $f2 (S/N)? "
	read yn
	if [ $yn == "S" ]; then
	    rm -f $f1 $f2
	fi
	;;
    2)
	bf1=`basename $f1`	
	bf2=`basename $f2`
	name=`echo $bf1 | cut -d. -f1`
	name+=`echo $bf2 | cut -d. -f1`
	echo "creo l'archivio $name.tar.gz"
	tar cf $name.tar $f1 $f2 2> /dev/null
	if [ $? -ne 0 ]; then
	    echo 
	    echo "Errore nella creazione dell'archivio"
	    exit 1;
	fi
	gzip $name.tar 2> /dev/null
	if [ $? -ne 0 ]; then
	    echo
	    echo "Errore nella compressione dell'archivio"
	    exit 1;
	fi
	;;
    3)
	echo "Appendo il file $f1 al file $f2"
	cat $f1 >> $f2
	if [ $? -ne 0 ]; then
	    echo 
	    echo "Errore nella concatenazione dei file"
	    exit 1;
	fi
	;;
    *)
	;;
esac
