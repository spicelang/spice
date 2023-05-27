sudo apt install build-essential software-properties-common
sudo add-apt-repository ppa:ubuntu-toolchain-r/ppa
sudo apt update && sudo apt install gcc-13 g++-13
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 110 --slave /usr/bin/g++ g++ /usr/bin/g++-13 --slave /usr/bin/gcov gcov /usr/bin/gcov-13 --slave /usr/bin/gcc-ar gcc-ar /usr/bin/gcc-ar-13 --slave /usr/bin/gcc-ranlib gcc-ranlib /usr/bin/gcc-ranlib-13