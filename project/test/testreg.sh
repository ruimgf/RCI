#!/bin/bash
nc -u tejo.tecnico.ulisboa.pt 59000 < get_servers &
pkill -P $$ > /dev/null
