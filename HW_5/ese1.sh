#!/bin/bash

if ! [ $# = 1 ]; then
	echo "usage:" $(basename $0) "filename"
	exit 1
fi

file=$1
if ! [ -e $file ]; then
	echo "file inesistente"
	exit 1
fi

echo "scrivi qualcosa"
while read line;do
	echo $line
	echo $line >> $file
done
