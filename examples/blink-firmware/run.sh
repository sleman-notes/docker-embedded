exec docker run --rm -it -v "$PWD":/work -w /work --user "$(id -u):$(id -g)" \
    -v /etc/passwd:/etc/passwd:ro \
    -v /etc/group:/etc/group:ro \
    firmware-build:1.0