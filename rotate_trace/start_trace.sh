#!/bin/bash
source ./trace_rc
Datetime=`date +"%Y/%m/%d %H:%m:%S"|sed 's/ //g'|sed 's/\///g'|sed 's/://g'`
mkdir -p ./logs
TRACE_NAME="./logs/trace_${Datetime}"
ERR_NAME="./logs/ERR_${Datetime}.log"
dpdk_trace_vpcid=1238165 dpdk_trace_addr='169.254.0.47:80' dpdk_trace_write=${TRACE_NAME} nohup dpdk-ptrace >> ${ERR_NAME} 2>&1 &