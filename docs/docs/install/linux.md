---
title: Install on Linux
---

### Install from repository
The recommended installation method on Linux is to set up the ChilliBits repository on your system and install Spice via
that.

=== "Debian/Ubuntu"
    To install Spice on Debian, execute the following commands in your terminal:
    ```sh
    sudo apt-get install ca-certificates software-properties-common
    curl -fsSL https://server.chillibits.com/files/repo/gpg | sudo tee /etc/apt/trusted.gpg.d/chillibits.asc
	sudo add-apt-repository "deb https://repo.chillibits.com/$(lsb_release -is | awk '{print tolower($0)}')-$(lsb_release -cs) $(lsb_release -cs) main"
	sudo apt-get install spice
    ```

=== "Fedora"
    To install Spice on Fedora, execute the following commands in your terminal:
    ```sh
    sudo dnf -y install dnf-plugins-core
	sudo dnf config-manager --add-repo https://server.chillibits.com/files/repo/fedora.repo
	sudo dnf install spice
    ```

=== "CentOS"
    To install Spice on CentOS, execute the following commands in your terminal:
    ```sh
    sudo yum install -y yum-utils
	sudo yum-config-manager --add-repo https://server.chillibits.com/files/repo/centos.repo
	sudo yum install spice
    ```

### Install from package file
You can also install Spice from a Linux package.

=== "Debian/Ubuntu"
    [Download amd64](https://github.com/spicelang/spice/releases/latest/download/spice_amd64.deb){ .md-button .md-button--primary .md-button--small }
    [Download arm64](https://github.com/spicelang/spice/releases/latest/download/spice_arm64.deb){ .md-button .md-button--primary .md-button--small }

    To install it, execute the following command:
    ```sh
    dpkg -i <deb-file-name>
    ```

=== "Fedora/CentOS"
    [Download amd64](https://github.com/spicelang/spice/releases/latest/download/spice_amd64.rpm){ .md-button .md-button--primary .md-button--small }
    [Download arm64](https://github.com/spicelang/spice/releases/latest/download/spice_arm64.rpm){ .md-button .md-button--primary .md-button--small }

    To install it, execute the following command:
    ```sh
    rpm -U <rpm-file-name>
    ```

=== "Alpine"
    [Download amd64](https://github.com/spicelang/spice/releases/latest/download/spice_amd64.apk){ .md-button .md-button--primary .md-button--small }
    [Download arm64](https://github.com/spicelang/spice/releases/latest/download/spice_arm64.apk){ .md-button .md-button--primary .md-button--small }

    To install it, execute the following command:
    ```sh
    apk add --allow-untrusted <apk-file-name>
    ```

### Install from archive file
If neither of the two installation approaches matched your desire, you also can download an archive file, that contains
all resources that Spice needs to run.

[Download amd64](https://github.com/spicelang/spice/releases/latest/download/spice_linux_amd64.tar.gz){ .md-button .md-button--primary .md-button--small }
[Download arm64](https://github.com/spicelang/spice/releases/latest/download/spice_linux_arm64.tar.gz){ .md-button .md-button--primary .md-button--small }

### Use
```sh
spice [options] <input>
```