#! /bin/bash 
while :
do
	echo 1 > /dev/myled0
	sleep $1
	echo 0 > /dev/myled0
	sleep $1
done
