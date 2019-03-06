if [ $# -ne 4 ]; then
	echo "usage: $0 <controller-IP> <topFile> <topName> <k>"
	echo "(k is the number of pods in the fat-tree)"
else
	mn --controller=remote,ip=$1,port=6653 --custom=$2 --topo $3,$4
fi
