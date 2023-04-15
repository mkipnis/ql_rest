#!/bin/bash

pip3 install django
pip3 install yfinance
pip3 install corsheaders
pip3 install django-cors-headers
pip3 install djangorestframework

cd options_monitor_frontend
npm install
npm run build

cd ../options_pricer_proxy/
ln -s ../options_monitor_frontend/build front_end
ln -s front_end/static static
