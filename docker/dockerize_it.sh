#!/bin/bash

docker build --no-cache -t ghcr.io/mkipnis/ql_rest:latest -f docker_build_all .
#docker push ghcr.io/mkipnis/ql_rest:latest
