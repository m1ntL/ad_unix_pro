##########################################################################
# File Name: cleanipc.sh
#Author: luqi
# Created Time: 2019年07月11日 星期四 19时23分23秒
#########################################################################
#!/bin/zsh

if [ "$#" != "2" ];then
	echo "usage:"$0" user shm|sem|msq|all"
	exit 1
elif [ "$2" != "shm" -a "$2" != "sem" -a "$2" != "msq" -a "$2" != "all" ];then
	echo "usage:"$0" user shm|sem|msq|all"
	exit 1
fi

#delete shm
if [ "$2" = "shm" -o "$2" = "all" ];then
	START = `ipcs | sed -n '/Shared/='`
	END = `ipcs | sed -n '/Semaphore/='`
	for i in `ipcs|sed -n ${START}, ${END}p|cut -d ' ' -f 2`
		do
			ipcrm shm $i
			echo -e $i
		done
fi

#delete sems
if [ "$2" = "sem" -o "$2" = "all" ];then
	START=`ipcs|sed -n '/Semaphore/='`
	END=`ipcs|sed -n '/Message/='`
	for i in `ipcs|sed -n ${START}, ${END}p | cut -d ' ' -f 2`
	do 
		ipcrm sem $i
		echo -e $i
	done
fi

#delete msg
if [ "$2" = "msg" -o "$2" = "all" ];then
	START=`ipcs|sed -n '/Message/='`
	for i in `ipcs| sed -n ${START}, ${END}p |cut -d ' ' -f 2`
	do 
		ipcrm msg $i
		echo -e $i
	done
fi
