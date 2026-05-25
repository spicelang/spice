---
icon: fontawesome/brands/apple
title: Install on macOS
tags:
  - Installation
  - macOS
---

### Install from package manager
The recommended installation method on macOS is via [Homebrew](https://brew.sh). Spice is distributed as a cask from
the official Spice tap.

```sh
brew tap spicelang/tap
brew install --cask spice
```

To upgrade to the latest release later, run:
```sh
brew upgrade --cask spice
```

!!! info "Apple Silicon only"
    The Homebrew cask currently only ships an ARM64 (Apple Silicon) build. On Intel Macs, please use the archive file
    below or build from source.

### Install from archive file
Currently, it is only possible to install spice on macOS via the tar.gz archive, uploaded to the GitHub release.
This archive file contains all resources that Spice needs to run.

[Download ARM64](https://github.com/spicelang/spice/releases/latest/download/spice_darwin_arm64.tar.gz){ .md-button .md-button--primary .md-button--small }

### Use
```sh
spice [options] <input>
```