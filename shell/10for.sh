##########################################################################
# File Name: 10for.sh
#Author: luqi
# Created Time: 2019年07月11日 星期四 15时08分21秒
#########################################################################
#!/bin/zsh

#for i in aaa bbb ccc
#do 
#	echo $i
#done


for i in `cat myfile.txt`
do 
	echo $i
done
