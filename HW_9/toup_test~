#!/bin/bash

OUT="/dev/null"

./toup_client "ciao!" "Hello" "Hi"    >&      $OUT & 
./toup_client "prova1" "prova2"       2>&1 >> $OUT & 
./toup_client "altra prova"           2>&1 >> $OUT & 
./toup_client "penultimo MESSAGGIO!"  2>&1 >> $OUT & 
./toup_client "Arrivederci" "goodbye" "au Revoir" "hasta luego"  2>&1 >> $OUT
./toup_client "altra prova"           2>&1 >> $OUT & 
./toup_client "altra prova"           2>&1 >> $OUT & 
./toup_client "altra prova"           2>&1 >> $OUT & 
./toup_client "Messaggio lungo aaaaaaaaaaa" "bbbb  bbbbbbbbbbbbb     bbbbbbbbbbbbbbbbbbbbbbb bbbbbbbbbbbbbbbbbb" "au Revoir" "hasta luego"  " cccc ccccc " " dddddd d d d d d d ddddddd "  "eeeee eeee eeee eeeeeeeeeee e e e e" 2>&1 >> $OUT
./toup_client "altra prova"           2>&1 >> $OUT & 
./toup_client "altra prova"           2>&1 >> $OUT & 
./toup_client "altra prova"           2>&1 >> $OUT & 
