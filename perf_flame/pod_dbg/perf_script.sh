#!/bin/bash

pid=`pidof svr`
DATE=`date "+%Y-%m-%d_%H_%M_%S"`
PLAYGROUND=/data/dbg
mkdir -p ${PLAYGROUND}
FILENAME=perf_${POD_NAME}_${pid}_${DATE}.data
pushd ${PLAYGROUND}
perf record -F 99 -g -p ${pid} -o ${FILENAME} -- sleep 30
popd
echo "sz ${PLAYGROUND}/${FILENAME}"