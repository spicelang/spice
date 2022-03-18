FROM alpine:3.15.1
WORKDIR /spice/out

ENV TERM="xterm-256color"
ENV SPICE_DOCKERIZED=1

ARG COMPILER_PATH=linux-amd64

RUN apk update && apk add --no-cache build-base ncurses-dev libc6-compat gcompat && rm -rf /var/cache/apk/*
RUN ln -sf /usr/lib/libncursesw.so.6 /usr/lib/libtinfo.so.6

COPY std/ /usr/lib/spice/std/
COPY spice /usr/bin/spice
COPY bin/spicec-${COMPILER_PATH}/ /usr/lib/spice/

ENTRYPOINT [ "spice" ]