#!/bin/bash

while /bin/true; do
date
ifconfig ${ETH0NAME}
ifconfig ${ETH1NAME}
sleep 1
done