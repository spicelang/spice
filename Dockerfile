FROM alpine:3.24.2
WORKDIR /spice/out

ARG TARGETPLATFORM

ENV TERM="xterm-256color"
ENV SPICE_DOCKERIZED=1

RUN apk update && apk add --repository=https://dl-cdn.alpinelinux.org/alpine/edge/main --no-cache \
    build-base ncurses-dev libc6-compat gcompat zstd-dev gcc libgcc clang lld && rm -rf /var/cache/apk/*
RUN ln -sf /usr/lib/libncursesw.so.6 /usr/lib/libtinfo.so.6

COPY std/ /usr/lib/spice/std/
# The vendored libbacktrace, which Spice programs link as '-lbacktrace'. Staged per architecture by the release
# workflow, and selected here the same way the compiler binary below is.
COPY docker-libs/$TARGETPLATFORM/libbacktrace.a /usr/lib/spice/std/runtime/lib/
COPY $TARGETPLATFORM/spice /usr/bin

ENTRYPOINT [ "spice" ]