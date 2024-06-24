#!/bin/bash

while /bin/true; do
date
./mypaping
if [ $? != 0 ]; then
    echo "conn timeout"
    exit
fi
sleep 1
done