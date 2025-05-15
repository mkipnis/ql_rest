#!/bin/sh

pip3 install yfinance --upgrade --no-cache-dir --break-system-packages
python3 save_demo_data.py
python3 save_risk_free_rates.py
