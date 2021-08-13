FROM alpine:3.14.1
WORKDIR /spice/out

ENV TERM="xterm-256color"
ENV SPICE_DOCKERIZED=1

ARG COMPILER_PATH=linux-amd64

RUN apk update && apk add --no-cache alpine-sdk libncurses5 && rm -rf /var/cache/apk/*
RUN ln -s /lib64/ld-linux-x86-64.so.2 /lib/ld-linux-x86-64.so.2

COPY std/ /usr/lib/spice/std/
COPY spice /usr/bin/spice
COPY bin/spicec-${COMPILER_PATH}/ /usr/lib/spice/

ENTRYPOINT [ "spice" ]