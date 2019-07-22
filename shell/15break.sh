##########################################################################
# File Name: 15break.sh
#Author: luqi
# Created Time: 2019年07月11日 星期四 16时00分21秒
#########################################################################
#!/bin/zsh

while : 
do 
	echo -n "input any number:"
	read num
	case $num in
	1|2|3|4|5)
		echo "you enter number between 1 and 5"
		;;
	*)
		echo "wrong number ,bye"
		break;
		;;
	esac
done

