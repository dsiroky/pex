#!/bin/bash
set -e

docker run -v $(pwd):/tmp/medianizer -it dsiroky/pex ./build/medianizer $@
