#!/bin/bash

cat <<FINE > filediinput.txt
riga 1
questa qua
riga 3
lalla bum riga
FINE

cat filediinput.txt | grep "riga" | sort
