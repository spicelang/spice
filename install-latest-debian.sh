#!/bin/sh

sudo apt-get install ca-certificates software-properties-common
curl -fsSL https://server.chillibits.com/files/repo/gpg | sudo apt-key add -
sudo add-apt-repository "deb https://repo.chillibits.com/$(lsb_release -is | awk '{print tolower($0)}')-$(lsb_release -cs) $(lsb_release -cs) main"
sudo apt-get install spice