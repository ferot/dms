#!/bin/bash
#Script prepares environment to run multiple system nodes instances on single machine.
#Prepares folders (if needed), changes configuration to avoid conflicts with other instances
#Runs applications
set -ex

#this should be changed(if needed) or get as input param

REPO_DIR="/home/tf/repos"
REPO_BUILD_DIR=$REPO_DIR/"dms/build"
DST_ENV_DIR="/home/tf/dms_run"

FIRST_ENV_DIR=$DST_ENV_DIR/bin
SECOND_ENV_DIR=$DST_ENV_DIR/bin2

function run_app() {
dir_to_run=$FIRST_ENV_DIR

cd $dir_to_run
./cam_app&

cd $SECOND_ENV_DIR
./cam_app&
}

function change_config() {
#change config(to avoid conflicts in mqtt communication and cameras acquisition) in second's cam dir
sed -i 's/"cam_id": 0,/"cam_id": 1,/' $SECOND_ENV_DIR/config.ini
sed -i 's/"client_id": 1/"client_id": 2/' $SECOND_ENV_DIR/config.ini
}

function create_dirs() {
#prepare environment
mkdir -p $DST_ENV_DIR

cp -rf $REPO_BUILD_DIR/bin $FIRST_ENV_DIR
cp -rf $REPO_BUILD_DIR/bin $SECOND_ENV_DIR
}


create_dirs
change_config
run_app





