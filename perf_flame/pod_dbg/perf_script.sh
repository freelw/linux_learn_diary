#!/bin/bash

pid=`pidof svr`
PLAYGROUND=/data/dbg
mkdir -p ${PLAYGROUND}
FILENAME=perf_${POD_NAME}.data
pushd ${PLAYGROUND}
perf record -F 99 -g -p ${pid} -o ${FILENAME} -- sleep 30
popd
echo "sz ${PLAYGROUND}/${FILENAME}"
