# Setup Guide for IceCC

## Install

```sh
apt-get install icecc icecream-sundae
```

## Start daemons (on all machines)

```sh
iceccd -d
```

## Start scheduler (on machine that triggers the compile process)

```sh
icecc-scheduler -d
```