##########################################################################
# File Name: 6fd.sh
#Author: luqi
# Created Time: 2019年07月10日 星期三 22时58分11秒
#########################################################################
#!/bin/zsh

exec 3<&0 0<name.txt

read line1
read line2

exec 0<&3

echo $line1
echo $line2






























