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
extern sem_t g_Mutex;

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

void parse_packet(uint8_t *buf, int len)
{
    in_addr addr;
    struct packet_info pkt_info;

    //print_bytes(buf, len);

    struct ether_header *eh = ETHER_HEADER(buf);

    if (eh->proto == htons(ETHPROTO_IPV4))
    {
        struct ip_header *iph = IP_HEADER(buf);
        //addr.s_addr = iph->saddr;
        //printf(" SrcIp: %s\n", inet_ntoa(addr));
        //addr.s_addr = iph->daddr;
        //printf(" DstIp: %s\n", inet_ntoa(addr));

        memset(&pkt_info, 0, sizeof(pkt_info));

        switch (iph->proto)
        {
            case IPV4PROTO_ICMP:
            {
                struct icmp_header *icmph = ICMP_HEADER(buf);
                if (icmph->type == ICMP_REQUEST)
                {
                    //printf(" ICMP Request\n");

                    pkt_info.type = ICMP_REQUEST_PACKET;
                    pkt_info.src_ip = iph->saddr;
                    pkt_info.dst_ip = iph->daddr;
                    sem_wait(&g_Mutex);
                    g_DDoSDetector.pkt_enqueue(pkt_info);
                    sem_post(&g_Mutex);
                }
                else if (icmph->type == ICMP_REPLY)
                {
                    //printf(" ICMP Reply\n");
                }
                break;
            }
            case IPV4PROTO_TCP:
            {
                struct tcp_header *th = TCP_HEADER(buf);
                //printf(" TCP SrcPort: %d, DstPost: %d, Flags: 0x%.2X\n",
                //htons(th->sport), htons(th->dport), th->flag);
                pkt_info.type = (th->flag == FLAG_SYN) ? TCP_SYN_PACKET : 0;
                pkt_info.src_ip = iph->saddr;
                pkt_info.dst_ip = iph->daddr;
                pkt_info.tcp_sport = th->sport;
                pkt_info.tcp_dport = th->dport;
                sem_wait(&g_Mutex);
                g_DDoSDetector.pkt_enqueue(pkt_info);
                sem_post(&g_Mutex);
                break;
            }
        }
    }
}

void *timer_thread(void *param)
{
    for (;;)
    {
        sleep(60);
        sem_wait(&g_Mutex);
        printf("[timer_thread] current size of pkt window is %d, reset it.\n",
               g_DDoSDetector.get_pkt_window_size());
        printf("[timer_thread] current value of abnormal window counter is %d, reset it.\n",
               g_DDoSDetector.get_abnormal_win_cnt());
        g_DDoSDetector.clear_pkt_window();
        sem_post(&g_Mutex);
    }
    return NULL;
}
