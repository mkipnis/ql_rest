#!/bin/bash

docker build -t options_pricer_cpp . -f Dockerfile.server_cpp
docker build -t options_pricer_py . -f Dockerfile.server_py

docker build -t options_pricer_client . -f Dockerfile.client
