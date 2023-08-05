#!/bin/bash

options_pricer_server/OptionsPricer 0.0.0.0 17921 &

cd options_pricer_proxy
python3 manage.py runserver 0.0.0.0:8000
