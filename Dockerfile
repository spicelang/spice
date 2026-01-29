FROM alpine:3.23.3
WORKDIR /spice/out

ARG TARGETPLATFORM

ENV TERM="xterm-256color"
ENV SPICE_DOCKERIZED=1

RUN apk update && apk add --repository=https://dl-cdn.alpinelinux.org/alpine/edge/main --no-cache \
    build-base ncurses-dev libc6-compat gcompat zstd-dev gcc libgcc clang lld && rm -rf /var/cache/apk/*
RUN ln -sf /usr/lib/libncursesw.so.6 /usr/lib/libtinfo.so.6

COPY std/ /usr/lib/spice/std/
COPY $TARGETPLATFORM/spice /usr/bin

ENTRYPOINT [ "spice" ]