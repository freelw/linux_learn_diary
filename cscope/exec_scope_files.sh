#!/bin/bash
find $1 -path $1/scripts* -prune -o -name *.[chxsS] -print > ./cscope.files
cscope -Rbkq