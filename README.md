# Docker Embedded

Command reference:

## The three words

- **Image:** a frozen filesystem. Your toolchain: gcc, make, libraries. Built
once, never changes. Like an ISO file.

- **Container:** a running instance of an image. Starts, runs one command,
exits, and you throw it away. Like booting that ISO in a VM you delete after.

- **Bind mount:** a directory on your machine, lent to a container while it
runs. The container sees it, edits it, and the changes are on your disk because
it *is* your disk.

Note that your source code does not go inside the image. You do not clone your
repo into the image. The image is the compiler, your repo is the input. You
mount the repo into a throwaway container, run `make`, and the .elf appears in
your local folder.

## Run a container, delete it

The lifecycles of a container, follows:

```bash
docker run --rm -it ubuntu:24.04 bash
```

- run = create a container from an image and start it
- --rm = delete the container when the command exits
- -it = interactive + terminal (needed for a shell)
- ubuntu:24.04 = the image
- bash = the command to run inside

After this, you will be in the root of a fresh Ubuntu. Try:

```bash
cd home
touch iwashere
ls
arm-none-eabi-gcc --version    # command not found (plain ubuntu has no toolchain)
exit
```

Run the exact same command again and look for "iwashere". It is gone. The
containeir was destroyed; the image never changed. **Containers are disposable,
images are not.**

## Give the container your files

The previous container could not see your code. It can be fixed with `-v`:

```bash
cd examples/blink-firmware
docker run --rm -it -v "$PWD":/work ubuntu:24.04 bash
ls /work            # your firmware sources, inside the container
exit
```

The "-v HOST_PATH:CONTAINER_PATH" is the bind mount. "/work" inside the
container **is** "examples/blink-firmware", on your machine. Create a file in
"/work" and it appears in your editor immediately. 

## Write the Dockerfile

Plain Ubuntu has no toolchain. Instead of installing one by hand every time,
describe the environment in a file. Read
[images/firmware-build/Dockerfile](images/firmware-build/Dockerfile).

The instructions most used:

| Instruction | What it does | When |
|---|---|---|
| FROM      | The starting filesystem                           | First line, always |
| ENV       | Environment variable                              | Persists into containers |
| RUN       | Execute a command, save the result as a layer     | build time |
| WORKDIR   | Default directory (creates it)                    | build time |
| CMD       | Default command if you give none                  | run time |
| COPY      | Host file into the image                          | build time |
| ARG       | Build-time variable                               | gone after build |

Build it:

```bash
docker build -t firmware-build:1.0 images/firmware-build
```

- "-t name:tag" names the image
- "images/firmware-build" is the **build context**, the folder sent to Docker,
and the folder COPY reads from. The Dockerfile is expected inside it.

After, check it exists, running "docker images".

## Build your firmware

```bash
cd examples/blink-firmware
docker run --rm -v "$PWD":/work -w /work firmware-build:1.0 make
ls build/
```

blink.elf, blink.bin, blink.map will be in your machine. Flash them with your
own host tools.

- "-w /work" sets the working directory so "make" finds the Makefile. In this
case its unecessary because the image already sets /work as WORKDIR

No shell, no "-it"; the container existed for the duration of one "make" and was
destroyed. Run it again and it rebuilds, the object files are on your disk, not
in the container.

Also, we can build it interactively:

```bash
docker run --rm -it -v "$PWD":/work firmware-build:1.0 bash
arm-none-eabi-gcc --version
make clean && make
arm-none-eabi-size build/blink.elf
exit
```

## Understanding layers

Every RUN, COPY and ADD creates a layer. Docker caches layers and reuses them
until one instruction changes, after which **that layer and every layer below it
rebuilds**.

See with:

```bash
docker history firmware-build:1.0
```

Now if we add a package at the end of the RUN in the Dockerfile and rebuild. The
whole apt-get layer re-runs, because we change it. But "FROM ubuntu:24.04" was
cached, so it did not re-download Ubuntu.

Beacause of this, it is important to follow two rules:

1. **Put what rarely changes first.** Toolchain at the top, tweaks at the
bottom. Reverse that will invalidate everything in each small change.

2. **Chain related commands into one RUN.**

```Dockerfile
# bad
RUN apt-get update
RUN apt-get install -y make
RUN rm -rf /var/lib/apt/lists/*

# good
RUN apt-get update && apt-get install -y --no-install-recommends make \
 && rm -rf /var/lib/apt/lists/*
```

The "--no-install-recommends" and "rm -rf" saves a few hundred MB.

## Build a second image on top of the first

Now, for run unit tests, we will need Ruby and Ceedling (but we already have
make, python and the toolchain in an image). We do not need install them twice.

[images/ceedling/Dockerfile](images/ceedling/Dockerfile)

```dockerfile
FROM firmware-build:1.0
```

also could be (with the repo already exists in dockerhub)

```dockerfile
FROM user/firmware-build:1.0
```

FROM inherits **everything**: installed packages, ENV, WORKDIR, all of it. We
only add what is missing.

```bash
docker build -t firmware-ceedling:1.0 images/ceedling

cd examples/blink-firmare
docker run --rm -v "$PWD":/work -w /work firmware-ceedling:1.0 ceedling test:all
```

the firmware-ceedling shares every layer with firmware-build, only the Ruby
layer is new. We can confirm that with "docker system df".

That sharing is why we should have several small purpose-build images rather
than one giant image that does everything.

## Build a third image from scratch

The [images/cpputest/Dockerfile](images/cpputest/Dockerfile) does the same job
as image 2 but starts "FROM ubuntu:24.04", so it repeats the entire toolchain
install.

```bash
docker build -t firmware-cpputest:1.0 images/cpputest

cd examples/blink-firmware
docker run --rm -it -v "$PWD":/work -w /work/tests/cpputest firmware-cpputest:1.0 make
```

Note the mount, the **parent** folder is mounted, and the working directory is
the subfolder.

## Tags mean something

firmware-build:1.0 is fine while we have one toolchain. Now, once we have two,
it would be nice to put the contents in the tag, like:

```
firmware-build:gcc13     firmware-build:gcc10
```

We must never rely on ":lastest". Because it can silently moves from GCC 13 to
GCC 14, a firmware that compile last month may not compile today and you will
have no idea why.

For publishing in docker hub, we can use:

```bash
docker login -u USER
docker tag firmware-build:1.0 USER/firmware-build:1.0
docker push USER/firmware-build:1.0
```

Elsewhere: docker pull USER/firmware-build:1.0. "docker run" pulls automatically
if the image is missing locally.

## Fix the root-owned files
