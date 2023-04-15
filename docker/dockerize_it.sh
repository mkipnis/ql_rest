#!/bin/bash

docker build -t ql_rest -f docker_build_all .

# docker run --rm -ti -v "$PWD:/opt/workspace" ql_rest
