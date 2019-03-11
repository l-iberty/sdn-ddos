#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pcap.h>
#include "headers.h"
#include "DDoSDetector.h"

extern DDoSDetector g_DDoSDetector;

void print_bytes(uint8_t *buf, int len)
{
    int i = 0;
    for (int idx = 0; idx < len; idx++)
    {
        if (i++ % 16 == 0) printf("\n");
        printf("%.2X ", buf[idx]);
    }
    printf("\n");
}

void parse_packet(uint8_t *packet, int len)
{
    in_addr addr;
    PacketInfo pkt;

    struct ether_header *eh = ETHER_HEADER(packet);

    if (eh->proto == htons(ETHPROTO_IPV4))
    {
        struct ip_header *iph = IP_HEADER(packet);

        memset(&pkt, 0, sizeof(pkt));

        switch (iph->proto)
        {
            case IPV4PROTO_ICMP:
            {
                struct icmp_header *icmph = ICMP_HEADER(packet);
                if (icmph->type == ICMP_REQUEST)
                {
                    pkt.type = ICMP_REQUEST_PACKET;
                    memcpy(pkt.src_mac, eh->saddr, MAC_LEN);
                    memcpy(pkt.dst_mac, eh->daddr, MAC_LEN);
                    pkt.src_ip = iph->saddr;
                    pkt.dst_ip = iph->daddr;
                    pkt.tcp_sport = 0;
                    pkt.tcp_dport = 0;
                    g_DDoSDetector.recv_packet(pkt);
                }
                break;
            }
            case IPV4PROTO_TCP:
            {
                struct tcp_header *th = TCP_HEADER(packet);
                pkt.type = (th->flag == FLAG_SYN) ? TCP_SYN_PACKET : 0;
                memcpy(pkt.src_mac, eh->saddr, MAC_LEN);
                memcpy(pkt.dst_mac, eh->daddr, MAC_LEN);
                pkt.src_ip = iph->saddr;
                pkt.dst_ip = iph->daddr;
                pkt.tcp_sport = th->sport;
                pkt.tcp_dport = th->dport;
                g_DDoSDetector.recv_packet(pkt);
                break;
            }
        }
    }
}
