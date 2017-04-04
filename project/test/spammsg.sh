#!/bin/bash
while IFS='' read -r line || [[ -n "$line" ]]; do

    echo "$line" | nc -u 193.136.128.103 17001 &
    done < "$1"
pkill -P $$ > /dev/null
