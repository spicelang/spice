---
title: Use with Docker
---

### Download
You don't have to pull the image first. You also can skip this step.
=== "Docker Hub"
    ```sh
    docker pull chillibits/spice
    ```
=== "GitHub Container Registry"
    ```sh
    docker pull ghcr.io/spicelang/spice
    ```

### Use
=== "Linux - Docker Hub"
    ```sh
    docker run --rm -it -v $(pwd):/spice/out chillibits/spice
    ```
=== "Linux - GHCR"
    ```sh
    docker run --rm -it -v $(pwd):/spice/out ghcr.io/spicelang/spice
    ```
=== "Windows - Docker Hub"
    ```sh
    docker run --rm -it -v ${pwd}:/spice/out chillibits/spice
    ```
=== "Windows - GHCR"
    ```sh
    docker run --rm -it -v ${pwd}:/spice/out ghcr.io/spicelang/spice
    ```

### Customize
#### Custom output path
You can use another output path by replacing `$(pwd)`/`${pwd}` with a custom path.

!!! example
    ```sh
    docker run --rm -it -v ./project:/spice/out chillibits/spice
    ```