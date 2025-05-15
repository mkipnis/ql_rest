#!/bin/sh

pip3 install yfinance --upgrade --no-cache-dir --break-system-packages
python3 save_demo_data.py

echo "Options Monitor URL: http://localhost:8000"
