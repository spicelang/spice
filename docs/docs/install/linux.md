---
title: Install on Linux
---

### Install from repository
=== "Debian/Ubuntu"
    To install Spice on Debian, execute the following commands in your terminal:
    ```sh
    sudo apt-get install ca-certificates
    curl -fsSL https://server.chillibits.com/files/repo/gpg | sudo apt-key add -
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

=== "Raspbian"
    To install Spice on Debian, execute the following commands in your terminal:
    ```sh
    sudo apt-get install ca-certificates
    curl -fsSL https://server.chillibits.com/files/repo/gpg | sudo apt-key add -
    sudo echo "deb [arch=armhf] https://repo.chillibits.com/$(lsb_release -is | awk '{print tolower($0)}')-$(lsb_release -cs) $(lsb_release -cs) main" > /etc/apt/sources.list.d/chillibits.list
    sudo apt-get update
    sudo apt-get install spice
    ```

    !!! warning
        The support for Raspbian is at the experimental stage. Please file a bug ticket on <a href="https://github.com/chillibits/spice/issues/new/choose" target="_blank">GitHub</a> if there occurs a bug for you while installing.

### Install from package file
You can also install Spice from a Linux package.

=== "Debian/Ubuntu/Raspbian"
    [Download amd64](https://github.com/chillibits/spice/releases/latest/download/spice_amd64.deb){ .md-button .md-button--primary .md-button--small }
    [Download arm64](https://github.com/chillibits/spice/releases/latest/download/spice_arm64.deb){ .md-button .md-button--primary .md-button--small }
    [Download armv5](https://github.com/chillibits/spice/releases/latest/download/spice_armv5.deb){ .md-button .md-button--primary .md-button--small }
    [Download armv6](https://github.com/chillibits/spice/releases/latest/download/spice_armv6.deb){ .md-button .md-button--primary .md-button--small }
    [Download armv7](https://github.com/chillibits/spice/releases/latest/download/spice_armv7.deb){ .md-button .md-button--primary .md-button--small }

    To install it, execute the following command:
    ```sh
    dpkg -i <deb-file-name>
    ```

=== "Fedora/CentOS"
    [Download amd64](https://github.com/chillibits/spice/releases/latest/download/spice_amd64.rpm){ .md-button .md-button--primary .md-button--small }
    [Download arm64](https://github.com/chillibits/spice/releases/latest/download/spice_arm64.rpm){ .md-button .md-button--primary .md-button--small }
    [Download armv5](https://github.com/chillibits/spice/releases/latest/download/spice_armv5.rpm){ .md-button .md-button--primary .md-button--small }
    [Download armv6](https://github.com/chillibits/spice/releases/latest/download/spice_armv6.rpm){ .md-button .md-button--primary .md-button--small }
    [Download armv7](https://github.com/chillibits/spice/releases/latest/download/spice_armv7.rpm){ .md-button .md-button--primary .md-button--small }

    To install it, execute the following command:
    ```sh
    rpm -U <rpm-file-name>
    ```

=== "Alpine"
    [Download amd64](https://github.com/chillibits/spice/releases/latest/download/spice_amd64.apk){ .md-button .md-button--primary .md-button--small }
    [Download arm64](https://github.com/chillibits/spice/releases/latest/download/spice_arm64.apk){ .md-button .md-button--primary .md-button--small }
    [Download armv5](https://github.com/chillibits/spice/releases/latest/download/spice_armv5.apk){ .md-button .md-button--primary .md-button--small }
    [Download armv6](https://github.com/chillibits/spice/releases/latest/download/spice_armv6.apk){ .md-button .md-button--primary .md-button--small }
    [Download armv7](https://github.com/chillibits/spice/releases/latest/download/spice_armv7.apk){ .md-button .md-button--primary .md-button--small }

    To install it, execute the following command:
    ```sh
    apk add --allow-untrusted <apk-file-name>
    ```

### Use
```sh
spice [options] <input>
```