#!/bin/bash

motion&

i=100000
while(($i > 5))
do
    /home/hujia/workspace/udp_find_me/send_once
    sleep 15
done
