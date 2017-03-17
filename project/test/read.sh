#!/bin/bash
while IFS='' read -r line || [[ -n "$line" ]]; do

    echo "$line" | nc -u 127.0.0.1 5000 &
    done < "$1"
pkill -P $$ > /dev/null
