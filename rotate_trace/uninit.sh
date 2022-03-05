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