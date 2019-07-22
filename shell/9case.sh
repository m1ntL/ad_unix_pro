##########################################################################
# File Name: 9case.sh
#Author: luqi
# Created Time: 2019年07月11日 星期四 14时32分23秒
#########################################################################
#!/bin/zsh

echo -n "enter a number between 1 and 3"
read num

case $num in
1)
	echo "you input 1"
	;;
2)
	echo "you input 2"
	;;
3)
	echo "you input 3"
	;;
y|Y)
	echo "you input y"
	;;
*)
	echo "`basename $0`:this is not between 1 and 3" >&2
	exit
	;;
esac
