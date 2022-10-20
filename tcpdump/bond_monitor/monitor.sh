#!/bin/bash

source ./monitorrc
nohup tcpdump -i ${ETH0NAME} ether proto 0x8809 -w eth0.pcap &
nohup tcpdump -i ${ETH1NAME} ether proto 0x8809 -w eth1.pcap &

nohup ./log_bond_stat.sh 2>&1 > ./bond_stat.log &
nohup ./log_iface_stat.sh 2>&1 > ./iface_stat.log &
