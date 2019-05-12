FROM ubuntu:19.04

RUN \
  apt-get update && \
  apt-get -y --no-install-recommends install \
      g++ \
      libboost-dev \
      libavcodec-dev \
      libavformat-dev \
      libavutil-dev \
      libswscale-dev \
      libfmt-dev \
      scons

ENV SCONSFLAGS "-u -j4"
WORKDIR "/tmp/medianizer"
