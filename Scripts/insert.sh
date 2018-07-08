#!/bin/bash
#Script used to fill manually database for debug purposes (f.e checking heatmap feature behaviour)

coord_1=$1
coord_2=$2
cnt=$3

for ((i=1;i<=$cnt;i++))
do
	sqlite3 database.db "insert into events (timestamp, objectid, position) values (\"2018-07-07 18:45:53\", 0, \"($coord_1,$coord_2)\")"
done

