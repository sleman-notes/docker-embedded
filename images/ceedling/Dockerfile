FROM firmware-build:1.0

ARG CEEDLING_VERSION=1.0.0

RUN apt-get update && apt-get install -y --no-install-recommends \
    ruby-full \
    build-essential \
    gcovr \
 && rm -rf /var/lib/apt/lists/* \
 && gem install ceedling -v ${CEEDLING_VERSION} --no-document

ENV HOME=/tmp

RUN ceedling version

WORKDIR /work
CMD ["bash"]