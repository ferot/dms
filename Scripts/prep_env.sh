#!/bin/sh
set -e -x
echo "Script preparing environment to run dms app. Please run with sudo"

if [ "$1" = "nc" ] 
then

apt-get -y update
apt-get -y upgrade

apt -y install git
apt -y install cmake

sudo apt-get -y install build-essential

echo "##########   Preparing boost ###########"

sudo apt-get -y install libboost-all-dev

echo "##########   Preparing mosquitto ###########"

sudo apt-get -y install libssl-dev
sudo apt-get -y install mosquitto

git clone https://github.com/eclipse/paho.mqtt.c
cd paho.mqtt.c && mkdir -p build
cd build && cmake ..
make -j4 && make install
cd ../..

echo "##########   Preparing FANN  ###########"
git clone https://github.com/libfann/fann.git
cd ./fann && mkdir build
cd build && cmake ..
make install
cd ../../

echo "##########   Preparing libjson-rpc-cpp ###########"

sudo apt-get -y install libcurl-ocaml-dev libmicrohttpd-dev libjsoncpp-dev libargtable2-dev libhiredis-dev

git clone git://github.com/cinemast/libjson-rpc-cpp.git
mkdir -p libjson-rpc-cpp/build
cd libjson-rpc-cpp/build
cmake .. && make
sudo make install
sudo ldconfig      
cd ../..

echo "##########   Preparing log4c ###########"

wget https://sourceforge.net/projects/log4c/files/log4c/1.2.3/log4c-1.2.3.tar.gz
tar zxvf log4c-1.2.3.tar.gz
cd log4c-1.2.3/
./configure --prefix=/usr/local
make
make install
cd ..

echo "##########   Preparing sqlite3 ###########"
sudo apt-get -y install libsqlite3-dev

git clone https://github.com/aminroosta/sqlite_modern_cpp
cd sqlite_modern_cpp
./configure
make install
cd ..

echo "##########   Preparing opencv ###########"


sudo apt-get -y install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get -y install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev

git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git
fi

if [ -d "opencv" && -d "opencv_contrib" ]; then
cd opencv
mkdir release
cd release
cmake -D OPENCV_EXTRA_MODULES_PATH=/home/tf/mgr/opencv_contrib/modules -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
make -j5
sudo make install

else 
	echo "There is neither opencv nor opencv_contrib repo!"
fi
#git clone https://github.com/ferot/dms.git

echo "##########   Preparing QT ###########"
sudo apt-get install '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev
git clone git://code.qt.io/qt/qt5.git
cd qt5
./configure -developer-build -opensource -nomake examples -nomake tests
make -j4
make install

if test -d "dms";
then
cd dms/App
mkdir -p build && cd build
cmake ..
make -j8
fi
