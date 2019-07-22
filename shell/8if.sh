##########################################################################
# File Name: 8if.sh
#Author: luqi
# Created Time: 2019年07月11日 星期四 14时24分05秒
#########################################################################
#!/bin/zsh

echo "input file name:"
read myfile


if [ -e $myfile ];then
	if [ -s $myfile ];then
		echo "$myfile exist and size greater than zero"
	else
		echo "$myfile exist but size is zero"
	fi
else 
	echo "file not exist"
fi
