##########################################################################
# File Name: 12until_mem.sh
#Author: luqi
# Created Time: 2019年07月11日 星期四 15时32分12秒
#########################################################################
#!/bin/zsh

part="/backup"
look_out=`df |grep "$part" |awk '{print $5}' | sed 's/%//g'`
echo $look_out
until [ "$look_out" -gt "90" ]
do
	echo -e "FILE SYSTEM $part IS NERAR FULL" | mail root
	look_out=`df | grep "$part" |awk '{print $5}' |sed 's/%//g'`
	sleep 3600
done
