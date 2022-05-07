#!/bin/sh

idn=$(id -u)

# Pricer 1
PRICER_1="BondPricer 0.0.0.0 15555"
bond_pricer_server/$PRICER_1 &

sleep 1

# Pricer 2 
PRICER_2="BondPricer 0.0.0.0 16666"
bond_pricer_server/$PRICER_2 &

sleep 1

# Client
cd  bond_pricer_client
python3 main.py

# Pricer 1 
pkill  -U $idn -f "$PRICER_1"

# Pricer 2 
pkill -U $idn -f "$PRICER_2"
