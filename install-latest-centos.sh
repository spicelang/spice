#!/bin/sh

sudo yum install -y yum-utils
sudo yum-config-manager --add-repo https://server.chillibits.com/files/repo/centos.repo
sudo yum install spice