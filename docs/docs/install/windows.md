---
title: Install on Windows
---

### Install via winget cli
Spice gets distributed for Windows via the new Windows package manager called [winget](https://github.com/microsoft/winget-cli). <br>
In the future, winget will be available for download in the Microsoft Store. Currently, the easiest way to install winget is to download it manually from GitHub. Visit the [installation instructions](https://github.com/microsoft/winget-cli#installing-the-client) from Microsoft.

As soon as the Windows package manager is installed on your Windows machine, you can open up powershell and execute the following installation command: <br>
```sh
winget install ChilliBits.Spice
```
After installing Spice, you should be able to run it. If not, please restart your powershell instance to make it reload the available commands.

### Install manually via installer

[Download 64bit](https://github.com/spicelang/spice/releases/latest/download/spice_x64_setup.msi){ .md-button .md-button--primary .md-button--small }

It is possible that you see an error notification by your anti-virus software complaining about Spice being a potentially dangerous application. This can happen if the latest version was released very recently and too few people downloaded it yet. If you want to validate the file signature, you can visit [this GitHub repo](https://github.com/microsoft/winget-pkgs/tree/master/manifests/c/ChilliBits/Spice), select your version and open the file `ChilliBits.Spice.installer.yaml`. There you can find the SHA256 signatures for the `x64` exe installer.

### Install from archive file
If neither of the two installation approaches matched your desire, you also can download an archive file, that contains
all resources that Spice needs to run.

[Download 64bit](https://github.com/spicelang/spice/releases/latest/download/spice_windows_amd64.zip){ .md-button .md-button--primary .md-button--small }

### Use
```sh
spice [options] <input>
```