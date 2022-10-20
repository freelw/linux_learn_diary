#!/bin/bash

nohup taskset -c 0 ./bench &
nohup taskset -c 7 ./bench &