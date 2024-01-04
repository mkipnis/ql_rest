#!/bin/bash

docker build --no-cache -t ghcr.io/mkipnis/ql_rest:latest  -f docker_build_all .

#docker build -t ql_rest -f docker_build_all .
docker push ghcr.io/mkipnis/ql_rest:latest


# docker run --rm -ti -v "$PWD:/opt/workspace" ql_rest
