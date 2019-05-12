FROM ubuntu:19.04

RUN \
  apt-get update && \
  apt-get -y --no-install-recommends install \
      g++ \
      libavcodec-dev \
      libavformat-dev \
      libavutil-dev \
      libboost-program-options-dev \
      libfmt-dev \
      libmsgsl-dev \
      libswscale-dev \
      scons

ENV SCONSFLAGS "-u -j4"
WORKDIR "/tmp/medianizer"
