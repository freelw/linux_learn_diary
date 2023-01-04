#!/bin/bash
find /data/dbg/rotate_trace/logs/ -mtime +1 -name "*.*" -exec rm {} \;