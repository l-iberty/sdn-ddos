curl -X POST -d '{"src-ip":"10.0.0.1/32","dst-ip":"10.0.0.2/32","nw-proto":"ICMP","action":"DENY"}' http://localhost:8080/wm/firewall/rules/json
