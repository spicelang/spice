FROM alpine:3.14.0
WORKDIR /spice/out

ENV TERM="xterm-256color"
ENV SPICE_DOCKERIZED=1

ARG COMPILER_PATH=linux-amd64

RUN apk update && apk add --no-cache libc6-compat libstdc++ && rm -rf /var/cache/apk/*

COPY spice /usr/bin/spice
COPY bin/spicec-${COMPILER_PATH}/ /usr/lib/spice/

ENTRYPOINT [ "spice" ]