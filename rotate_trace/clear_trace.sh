#!/bin/bash
find /data/dbg/rotate_trace/logs/ -mmin +10 -name "trace*" -exec rm {} \;