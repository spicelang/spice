#!/bin/sh

sudo dnf -y install dnf-plugins-core
sudo dnf config-manager --add-repo https://server.chillibits.com/files/repo/fedora.repo
sudo dnf install spice