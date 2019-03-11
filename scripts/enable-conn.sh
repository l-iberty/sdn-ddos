URL_FIREWALL_ENABLE=http://localhost:8080/wm/firewall/module/enable/json
URL_FIREWALL_RULES=http://localhost:8080/wm/firewall/rules/json

curl -X PUT ${URL_FIREWALL_ENABLE}
curl -X POST -d  '{"src-ip":"*","dst-ip":"*","dl-type":"ARP","priority":"10"}' ${URL_FIREWALL_RULES}
curl -X POST -d  '{"src-ip":"*","dst-ip":"*","nw-proto":"ICMP","priority":"10"}' ${URL_FIREWALL_RULES}
curl -X POST -d  '{"src-ip":"*","dst-ip":"*","tp-src":"*","tp-dst":"*","nw-proto":"TCP","priority":"10"}' ${URL_FIREWALL_RULES}
