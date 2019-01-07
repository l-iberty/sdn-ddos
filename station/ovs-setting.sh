if [ $# -ne 1 ]
then echo "usage: $0 <sFlow-collector-IP>"
else
	ovs-vsctl -- --id=@sflow create sflow agent=s1 target=\"$1:6343\" header=128 sampling=10 polling=1 -- set bridge s1 sflow=@sflow
fi
