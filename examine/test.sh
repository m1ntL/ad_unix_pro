##########################################################################
# File Name: test.sh
#Author: luqi
# Created Time: 2019年07月14日 星期日 21时05分59秒
#########################################################################
#!/bin/zsh

test -e "$HOME/test" || mkdir $HOME/test

i=0
while [ "$i" != "50" ]
do
	i=$(($i+1))
	mkdir $HOME/test/user"$i"
	chmod 734 $HOME/test/user"$i"
	echo user"$i"
done

