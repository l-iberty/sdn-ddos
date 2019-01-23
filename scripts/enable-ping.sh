curl -X POST -d  '{"src-ip":"*","dst-ip":"*","dl-type":"ARP"}' http://localhost:8080/wm/firewall/rules/json
curl -X POST -d  '{"src-ip":"*","dst-ip":"*","nw-proto":"ICMP"}' http://localhost:8080/wm/firewall/rules/json
