# Cheatsheet

## Build the images

```bash
docker build -t img:tag folder

docker build --no-cache -t img:tag folder      # ignore the layer cache
```

## Run

Example with firmware-build:1.0

```bash
# compile the firmware in the current directory
docker run --rm -v "$PWD":/work -w /work --user "$(id -u):$(id -g)" \
  firmware-build:1.0 make

# interactive shell inside the toolchain
docker run --rm -it -v "$PWD":/work -w /work firmware-build:1.0 bash
```

| Flag | Meaning |
|---|---|
| --rm | delete the container when it exits |
| -v HOST:CONTAINER | bind mount |
| -w DIR | working directory inside the container |
| -it | interactive + TTY (needed for a shell) |
| --user UID:GID | run as you, so files are not root-owned |
| -e VAR=value | set an environment variable |
| --name NAME | name the container instead of a random one |

## Dockerfile instructions

| Instruction | Runs at | Notes |
|---|---|---|
| FROM img:tag | build | first line; pin the tag |
| RUN cmd | build | creates a layer; chain with `&&` |
| ENV K=V | build+run | persists into containers |
| ARG K=V | build only | overridden with `--build-arg K=V` |
| WORKDIR /d | build+run | creates the dir |
| COPY src dst | build | host -> image |
| CMD ["x"] | run | default command, easily overridden |
| ENTRYPOINT ["x"] | run | fixed executable, args appended |

## Docker Hub

```bash
docker login -u YOURUSER
docker tag firmware-build:1.0 YOURUSER/firmware-build:1.0
docker push YOURUSER/firmware-build:1.0
docker pull YOURUSER/firmware-build:1.0
```

```bash
docker images                    # local images and sizes
docker ps                        # running containers
docker ps -a                     # including stopped ones
docker history img:tag           # the layers and what created them
docker inspect img:tag           # full metadata
docker exec -it CONTAINER bash   # shell into a running container
docker system df                 # real disk usage, accounting for shared layers
```

## Clean up

```bash
docker rmi img:tag        # remove an image
docker image prune        # dangling images
docker builder prune      # build cache - usually the real disk hog
docker system prune -a    # everything unused; destructive, read the prompt
docker rm container_name_or_id # remove the container
```