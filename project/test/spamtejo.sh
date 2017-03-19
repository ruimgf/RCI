#!/bin/bash
while IFS='' read -r line || [[ -n "$line" ]]; do

    echo "$line" | nc -u tejo.tecnico.ulisboa.pt 59000 &
    done < "$1"
pkill -P $$ > /dev/null
