#!/bin/sh

pip install yfinance --upgrade --no-cache-dir
python save_demo_data.py
python save_risk_free_rates.py