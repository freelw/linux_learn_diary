#!/bin/bash
find /data/dbg/rotate_trace/logs/ -mmin +120 -name "trace*" -exec rm {} \;