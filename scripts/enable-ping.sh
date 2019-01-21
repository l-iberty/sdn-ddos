curl -X POST -d  '{"src-ip":"10.0.0.1/32","dst-ip":"10.0.0.2/32","dl-type":"ARP"}' http://localhost:8080/wm/firewall/rules/json
curl -X POST -d  '{"src-ip":"10.0.0.2/32","dst-ip":"10.0.0.1/32","dl-type":"ARP"}' http://localhost:8080/wm/firewall/rules/json
curl -X POST -d  '{"src-ip":"10.0.0.1/32","dst-ip":"10.0.0.2/32","nw-proto":"ICMP"}' http://localhost:8080/wm/firewall/rules/json
curl -X POST -d  '{"src-ip":"10.0.0.2/32","dst-ip":"10.0.0.1/32","nw-proto":"ICMP"}' http://localhost:8080/wm/firewall/rules/json
