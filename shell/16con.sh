##########################################################################
# File Name: 16con.sh
#Author: luqi
# Created Time: 2019年07月11日 星期四 16时06分44秒
#########################################################################
#!/bin/zsh

while :
do 
	echo -n "input a number:"
	read num
	case $num in
		1|2|3|4|5)
			echo "you enter number between 1 and 5"
			;;
		*)
			echo -n " wrong,continue?(y|n)"
			read con
			case $con in
				y)
					continue;
					;;
				n)
					break;
					;;
			esac
	esac
done
