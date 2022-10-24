#!/bin/bash

source ./monitorrc
while /bin/true; do
date
echo 'bonding stat:'
cat /sys/class/net/bond1/bonding/slaves 
echo "${ETH0NAME} stat:"
cat /sys/class/net/${ETH0NAME}/bonding_slave/state
echo "${ETH1NAME} stat:"
cat /sys/class/net/${ETH1NAME}/bonding_slave/state
sleep 1
done