FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    vim \
    wget \
    ca-certificates \
    xz-utils \
    make \
    python3 \
    git \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /work
CMD ["bash"]