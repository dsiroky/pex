#!/bin/bash
set -e

docker run -v $(pwd):/tmp/medianizer -it dsiroky/pex sh -c "scons"
