if [ $# -eq 0 ]
then
	echo "usage-1 $0 <controller-IP>"
	echo "usage-2 $0 <controller-IP> <topoFile> <topoName>"
elif [ $# -eq 1 ]
then
	mn --controller=remote,ip=$1,port=6653
elif [ $# -eq 3 ]
then
	mn --controller=remote,ip=$1,port=6653 --custom=$2 --topo $3
fi
