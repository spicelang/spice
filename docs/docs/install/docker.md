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
    docker pull ghcr.io/chillibits/spice
    ```

### Use
=== "Docker Hub"
    ```sh
    docker run --rm -it -v $(pwd):/spice/out chillibits/spice
    ```
=== "GitHub Container Registry"
    ```sh
    docker run --rm -it -v $(pwd):/spice/out ghcr.io/chillibits/spice
    ```

### Customize
#### Custom output path
You can use another output path by replacing `$(pwd)` with a custom path.

!!! example
    ```sh
    docker run --rm -it -v ./project:/spice/out chillibits/spice
    ```