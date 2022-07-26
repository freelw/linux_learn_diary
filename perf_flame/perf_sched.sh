#!/bin/bash

uname -a
perf sched record -- sleep 100
perf sched latency -p --sort max | grep soft