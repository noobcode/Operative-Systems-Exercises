#!/bin/bash

if ! [ $# = 3 ]; then
	echo "usage "`basename $0` "dir1 dir2 dir3"
	exit 1
fi

for dir in $@; do
	if ! [ -d $dir ]; then
		echo "$dir non è una directory"
		exit 1
	fi
done

dir1=$1
dir2=$2
dir3=$3

#for file1 in $dir1; do
#	for file2 in $dir2; do
#		if ! [ $file1 = $file2 ]; then
#			cp $file1 $dir3
#		fi
#		if [ $file1 = $file2 ]; then
#			if ! diff $file1 $file2; then
#			cp $file1 $dir3
#			fi
#		fi
#	done
#done	 

for line in $(diff -r -q -N $1 $2); do
    if [[ $line =~ dir1/.* ]]; then
	cp $line $3
    fi
done

