#!/bin/sh

set -x

idn=$(id -u)

PRICER="SwapPricer 0.0.0.0 7922"
swap_pricer/$PRICER &

sleep 1

cd swap_designer_front_end
npm start

# Pricer 1 
pkill  -U $idn -f "$PRICER"
