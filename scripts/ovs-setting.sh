if [ $# -ne 2 ]
then echo "usage: $0 <switch-Datapath, eg. s1> <sFlow-collector-IP>"
else
	ovs-vsctl -- --id=@sflow create sflow agent=$1 target=\"$2:6343\" header=128 sampling=10 polling=1 -- set bridge $1 sflow=@sflow
fi
