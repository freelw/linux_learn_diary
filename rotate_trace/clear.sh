#!/bin/bash
find /data/dbg/logs/ -mmin +100 -name "*" -exec rm {} \;