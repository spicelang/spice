---
title: Install on Linux
---

### Install from repository
=== "Debian/Ubuntu/Raspbian"
    To install Spice on Debian, execute the following commands in your terminal:
    ```sh
    curl -fsSL https://server.chillibits.com/files/repo/gpg | sudo apt-key add -
	sudo add-apt-repository "deb https://repo.chillibits.com/$(lsb_release -is | awk '{print tolower($0)}')-$(lsb_release -cs) $(lsb_release -cs) main"
	sudo apt-get update
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

=== "Alpine"
    *Spice will be published for Alpine soon ...*

### Install from package file
You can also install Spice from a Debian package (`.deb`). Simply download the package [here](https://github.com/chillibits/spice/releases/latest).

=== "Debian/Ubuntu/Raspbian"
    To install it, execute the following command:
    ```sh
    dpkg -i <deb-file-name>
    ```

=== "Fedora"
    To install it, execute the following command:
    ```sh
    rpm -U <rpm-file-name>
    ```

=== "CentOS"
    To install it, execute the following command:
    ```sh
    rpm -U <rpm-file-name>
    ```

=== "Alpine"
    To install it, execute the following command:
    ```sh
    apk add --allow-untrusted <apk-file-name>
    ```

### Use
```sh
spice [options] <input>
```