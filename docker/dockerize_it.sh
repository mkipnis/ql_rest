#!/bin/bash

docker build -t ghcr.io/mkipnis/ql_rest:latest -f docker_build_all .
#docker push ghcr.io/mkipnis/ql_rest:latest
