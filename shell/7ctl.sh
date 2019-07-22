##########################################################################
# File Name: 7ctl.sh
#Author: luqi
# Created Time: 2019年07月11日 星期四 13时15分52秒
#########################################################################
#!/bin/zsh

echo "test use 1"
test -w tmp.txt
if [ $? -eq "0" ];then
	echo "success\n";
else 
	echo "not sucess"；
fi
echo $?
echo "test use 2 [] begin"
[ -w tmp.txt ]
echo $?
