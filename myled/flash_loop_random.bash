#! /bin/bash
while :
do
	echo 1 > /dev/myled0
	sleep $(($RANDOM % 5))
	echo 0 > /dev/myled0
	sleep $(($RANDOM % 5))
done
