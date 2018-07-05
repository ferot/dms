#!/bin/bash
#Script prepares environment to run multiple system nodes instances on single machine.
#Prepares folders (if needed), changes configuration to avoid conflicts with other instances
#Runs applications
set -x

trap 'echo "killing apps..."; pkill cam_app; pkill calib_tool; pkill model_app' INT

#this should be changed(if needed) or get as input param
REPO_DIR="/home/tf/repos"
REPO_BUILD_DIR=$REPO_DIR/"dms/build"
DST_ENV_DIR="/home/tf/dms_run"

FIRST_ENV_DIR=$DST_ENV_DIR/bin
SECOND_ENV_DIR=$DST_ENV_DIR/bin2

function run_app() {
run_dirs=( "$SECOND_ENV_DIR" "$FIRST_ENV_DIR" )
INDEX=0
for i in ${run_dirs[*]}; do
let INDEX=${INDEX}+1
    echo $i
    cd $i
    ./cam_app & > log_${INDEX}
    pids[${INDEX}]=$!
done
}

function wait_for_jobs() {
# wait for all pids
for pid in ${pids[*]}; do
    echo "waiting for $pid"
    wait $pid
done
}

function change_config() {
#change config(to avoid conflicts in mqtt communication and cameras acquisition) in second's cam dir
sed -i 's/"cam_id": 0,/"cam_id": 1,/' $SECOND_ENV_DIR/config.ini
sed -i 's/"client_id": 1/"client_id": 2/' $SECOND_ENV_DIR/config.ini
}

function create_dirs() {
#prepare environment
mkdir -p $DST_ENV_DIR
mkdir -p $FIRST_ENV_DIR
mkdir -p $SECOND_ENV_DIR

cp -rf $REPO_BUILD_DIR/bin/* $FIRST_ENV_DIR
cp -rf $REPO_BUILD_DIR/bin/* $SECOND_ENV_DIR
}


create_dirs
change_config
run_app
./model_app
./calib_tool
wait_for_jobs



