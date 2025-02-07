sudo apt install build-essential software-properties-common
sudo add-apt-repository ppa:ubuntu-toolchain-r/ppa
sudo apt update && sudo apt install gcc-14 g++-14
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 110 --slave /usr/bin/g++ g++ /usr/bin/g++-14 --slave /usr/bin/gcov gcov /usr/bin/gcov-14 --slave /usr/bin/gcc-ar gcc-ar /usr/bin/gcc-ar-14 --slave /usr/bin/gcc-ranlib gcc-ranlib /usr/bin/gcc-ranlib-14