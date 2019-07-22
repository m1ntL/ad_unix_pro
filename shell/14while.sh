##########################################################################
# File Name: 14while.sh
#Author: luqi
# Created Time: 2019年07月11日 星期四 15时51分35秒
#########################################################################
#!/bin/zsh


echo "ctrl+d quit"
while read line
do
	echo $line
done < myfile.txt
