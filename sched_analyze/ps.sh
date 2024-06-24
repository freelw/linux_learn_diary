#!/bin/bash


while /bin/true; do

date

ps aux -L | awk '$10 == "Rl"'
sleep 1

done