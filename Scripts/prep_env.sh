#!/bin/sh
set -e -x
echo "Script preparing environment to run dms app. Please run with sudo"

echo "deb http://www.linux-projects.org/listing/uv4l_repo/raspbian/stretch stretch main" >> /etc/apt/sources.list

apt-get -y update
apt-get -y upgrade

echo "UV4L"
sudo apt-get install uv4l uv4l-raspicam uv4l-raspicam-extras

apt -y install git
apt -y install cmake

sudo apt-get -y install build-essential

cd /home/pi/opt

echo "##########   Preparing boost ###########"

sudo apt-get -y install libboost-all-dev

echo "##########   Preparing mosquitto ###########"

sudo apt-get -y install libssl-dev
sudo apt-get -y install mosquitto

git clone https://github.com/eclipse/paho.mqtt.c
cd paho.mqtt.c && mkdir -p build
cd build && cmake ..
make -j4 && make install
cd /home/pi/opt

echo "##########   Preparing FANN  ###########"
git clone https://github.com/libfann/fann.git
cd ./fann && mkdir build
cd build && cmake ..
make install
cd /home/pi/opt

echo "##########   Preparing log4c ###########"

wget https://sourceforge.net/projects/log4c/files/log4c/1.2.4/log4c-1.2.4.tar.gz
tar zxvf log4c-1.2.4.tar.gz
cd log4c-1.2.4/
./configure --prefix=/usr/local
make
make install
cd /home/pi/opt

echo "##########   Preparing sqlite3 ###########"
sudo apt-get -y install libsqlite3-dev

git clone https://github.com/aminroosta/sqlite_modern_cpp
cd sqlite_modern_cpp
./configure
make install
cd /home/pi/opt

echo "##########   Preparing opencv ###########"


sudo apt-get -y install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get -y install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev




#git clone https://github.com/opencv/opencv.git
#git clone https://github.com/opencv/opencv_contrib.git

cd ~/opt/opencv
mkdir release
cd release
cmake -D OPENCV_EXTRA_MODULES_PATH=/home/pi/opt/opencv_contrib/modules -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
make -j4
sudo make install

cd /home/pi/opt

#git clone https://github.com/ferot/dms.git

echo "##########   Preparing QT ###########"
sudo apt-get -y install '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev
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
