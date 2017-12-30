#! /bin/bash

for((i=0; i< $1; i++)); do
	echo 1 > /dev/myled0
	sleep $2
	echo 0 > /dev/myled0
	sleep $2
done
