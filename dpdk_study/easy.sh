#!/bin/bash
DPDK_FOLDER='/home/wangli/dpdk_learn/dpdk-22.03'
pushd ${DPDK_FOLDER}
ifconfig enp2s0 down
./usertools/dpdk-devbind.py --bind=vfio-pci enp2s0
echo 1024 > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages
pushd ./examples/helloworld/build
./helloworld -l 0-3 -n 4
popd
popd