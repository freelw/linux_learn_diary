#!/bin/bash

crondir="/var/spool/cron"
crontfile=""

if [[ -d ${crondir}/tabs ]];then
    crontfile=${crondir}/tabs/root
elif [[ -d ${crondir} ]];then
    crontfile=${crondir}/root
else
    crontfile=${crondir}/tabs/root
fi

sed -i "/data\/dbg\/rotate_trace/d" $crontfile
echo "*/20 * * * * cd /data/dbg/rotate_trace/ && ./restart.sh >> /dev/null 2>&1 &" >> $crontfile
echo "*/20 * * * * cd /data/dbg/rotate_trace/ && ./clear.sh >> /dev/null 2>&1 &" >> $crontfile