# PEX task

## Prerequisites
  * docker

## Steps

For a "stable" environment in a docker image:
  1. Run `./prepare_env.sh` to set up a docker image.
  1. Run `./build.sh` to build the program.
  1. Run `./run.sh -i video.mp4 -o output.csv -w 3 -h 3`.

For a local build (see `Dockerfile` for other prerequisites):
  1. Run `scons -u -j4`.
  1. Run `./build/medianizer -i video.mp4 -o output.csv -w 3 -h 3`.
