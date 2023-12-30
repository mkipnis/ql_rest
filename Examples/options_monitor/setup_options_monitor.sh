#!/bin/bash

pip3 install django --break-system-packages
pip3 install yfinance --break-system-packages
pip3 install django-cors-headers --break-system-packages
pip3 install djangorestframework --break-system-packages

cd options_monitor_frontend
npm install
npm run build

cd ../options_pricer_proxy/
ln -s ../options_monitor_frontend/build front_end
ln -s front_end/static static
