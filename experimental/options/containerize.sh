#!/bin/bash

docker build --no-cache -t ghcr.io/mkipnis/ql_rest_opt_pricer_cpp:latest . -f Dockerfile.server_cpp
docker build --no-cache -t ghcr.io/mkipnis/ql_rest_opt_pricer_py:latest . -f Dockerfile.server_py
docker build --no-cache -t ghcr.io/mkipnis/ql_rest_opt_pricer_client:latest . -f Dockerfile.client

docker push ghcr.io/mkipnis/ql_rest_opt_pricer_cpp:latest
docker push ghcr.io/mkipnis/ql_rest_opt_pricer_py:latest
docker push ghcr.io/mkipnis/ql_rest_opt_pricer_client:latest
