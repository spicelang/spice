---
icon: fontawesome/brands/docker
title: Use with Docker
tags:
  - Installation
  - Docker
---

### Download
You don't have to pull the image first. You also can skip this step.
=== ":fontawesome-brands-docker: Docker Hub"
    ```sh
    docker pull chillibits/spice
    ```
=== ":fontawesome-brands-github: GitHub Container Registry"
    ```sh
    docker pull ghcr.io/spicelang/spice
    ```

### Use
=== ":fontawesome-brands-docker: Docker Hub"
    ```sh
    # Linux/macOS
    docker run --rm -it -v $(pwd):/spice/out chillibits/spice
    # Windows
    docker run --rm -it -v ${pwd}:/spice/out chillibits/spice
    ```
=== ":fontawesome-brands-github: GitHub Container Registry"
    ```sh
    # Linux
    docker run --rm -it -v $(pwd):/spice/out ghcr.io/spicelang/spice
    # Windows
    docker run --rm -it -v ${pwd}:/spice/out ghcr.io/spicelang/spice
    ```

`spice` is the self-hosted compiler, which is written in Spice itself. The image also contains the host compiler, which
is written in C++ and builds the self-hosted compiler, as `spice-host`. To fall back to it, override the entrypoint:
```sh
docker run --rm -it --entrypoint spice-host -v $(pwd):/spice/out chillibits/spice
```

The `Compiler` line of the `--version` output tells which of the two compilers you are running (`host` or `self-hosted`).

### Customize
#### Custom output path
You can use another output path by replacing `$(pwd)`/`${pwd}` with a custom path.

!!! example
    ```sh
    docker run --rm -it -v ./project:/spice/out chillibits/spice
    ```