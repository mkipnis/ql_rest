#!/bin/bash

cd /ql_rest_deps_build/ql_rest/Examples/options_monitor/options_pricer_server
./OptionsPricer 0.0.0.0 17921 &

cd /ql_rest_deps_build/ql_rest/Examples/options_monitor/options_pricer_proxy
python3 manage.py runserver 0.0.0.0:8000
