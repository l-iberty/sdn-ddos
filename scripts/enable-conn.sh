URL=http://localhost:8080/wm/firewall/rules/json

curl -X POST -d  '{"src-ip":"*","dst-ip":"*","dl-type":"ARP"}' $URL
curl -X POST -d  '{"src-ip":"*","dst-ip":"*","nw-proto":"ICMP"}' $URL
curl -X POST -d  '{"src-ip":"*","dst-ip":"*","tp-src":"*","tp-dst":"*","nw-proto":"TCP"}' $URL
