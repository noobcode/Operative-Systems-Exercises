#!/bin/bash
#
# Esercizio 2
#
# test:
#  shell$> ./signal.sh > out.txt 2> err.txt &
#  shell$> for ((i=0;i<5;++i)); do kill -USR1 $! ; kill -USR2 $! ;  sleep; done ; kill -9 $!

function errecho() { echo "$@" 1>&2; }

trap 'echo $(date +"%T") ricevuto SIGUSR1' USR1
trap 'errecho $(date +"%T") ricevuto SIGUSR2' USR2

# ciclo infinito che processa i segnali ogni decimo di secondo 
while true; do sleep 0.1; done
