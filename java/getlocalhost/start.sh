#!/bin/bash

java -Dsun.net.inetaddr.ttl=0 JavaGetLocalHostExample $1 &
javaid=$!

while /bin/true; do
date
jstack ${javaid}|grep BLOCK -A 2
sleep 1
done
