#!/bin/bash
veth=xxx
uuid='uuu'
tcpdump -i ${veth} tcp and port 8445 -nn -v -s 120 -C250 -W100 -Z root -w /data/dbg/_${veth}_${uuid}_20220523.pcap
