#!/bin/bash
 if ! [ $# = 1 ]; then
 	echo "fuck"
 	exit 1
 fi
 
file=$1
ele=0
max=0
min=0
somma=0
while read num; do
	somma=$(($somma+$num))
	ele=$(($ele+1))
	if [ $num -gt $max ]; then
		max=$num
	fi
	if [ $num -lt $min ]; then
		min=$num
	fi
done < $file

media=$(echo "scale=2; $somma/$ele" | bc) 
echo "ele $ele"
echo "max $max"
echo "min $min"
echo "somma $somma"
echo "media $media"
