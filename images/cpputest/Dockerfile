FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    vim \
    make \
    python3 \
    git \
    ca-certificates \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    libnewlib-arm-none-eabi \
    build-essential \
    libcpputest-dev \
    gcovr \
 && rm -rf /var/lib/apt/lists/*

# libcpputest-dev installs into the normal system paths, so CPPUTEST_HOME - the
# variable every CppUTest Makefile expects - is simply /usr here.
ENV CPPUTEST_HOME=/usr
ENV HOME=/tmp

WORKDIR /work
CMD ["bash"]