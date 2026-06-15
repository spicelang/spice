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

The cask ships builds for both Apple Silicon (ARM64) and Intel (x86_64) Macs, and Homebrew automatically selects the
one matching your machine.

### Install from archive file
Alternatively, you can install Spice on macOS via the tar.gz archive, uploaded to the GitHub release.
This archive file contains all resources that Spice needs to run.

[Download ARM64](https://github.com/spicelang/spice/releases/latest/download/spice_darwin_arm64.tar.gz){ .md-button .md-button--primary .md-button--small }
[Download x86_64](https://github.com/spicelang/spice/releases/latest/download/spice_darwin_amd64.tar.gz){ .md-button .md-button--primary .md-button--small }

### Use
```sh
spice [options] <input>
```