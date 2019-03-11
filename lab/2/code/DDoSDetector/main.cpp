#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pcap.h>
#include <cstdlib>
#include "headers.h"
#include "DDoSDetector.h"

#define SFLOW_PORT 6343

DDoSDetector g_DDoSDetector;

void print_bytes(uint8_t *buf, int len);
void parse_packet(uint8_t *packet, int len);

void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
    (*(int *) param)++;
    //printf("\n[%d] packet_handler invoked.", *(int *) param);

    struct udp_header *udp_hdr = UDP_HEADER(pkt_data);

    //printf(" (udp dst_port: %d)", htons(udp_hdr->dport));
    if (htons(udp_hdr->dport) == SFLOW_PORT)
    {
        struct sflow_header *sflow_hdr = SFLOW_HEADER(pkt_data);

        if (htonl(sflow_hdr->addr_type) == SFLOW_AGENT_ADDR_IPV4)
        {
            //in_addr in;
            //in.s_addr = sflow_hdr->addr;
            //printf(" (agent addr: %s)", inet_ntoa(in));

            struct sflow_sample_header *sample_hdr = SFLOW_SAMPLE_HEADER(pkt_data);

            //int flow_samples = 0, counter_samples = 0;
            for (int i = 0; i < htonl(sflow_hdr->nr_samples); i++)
            {
                if (SAMPLE_TYPE(htonl(sample_hdr->sample_type)) == SFLOW_SAMPLE_TYPE_FLOW)
                {
                    //flow_samples++;
                    //printf("\nflow sample[%d]", flow_samples);
                    parse_packet((uint8_t *) &sample_hdr->raw_pkt_hdr.sampled_pkt,
                                 htonl(sample_hdr->raw_pkt_hdr.orig_pkt_len));
                }
                else if (SAMPLE_TYPE(htonl(sample_hdr->sample_type)) == SFLOW_SAMPLE_TYPE_COUNTER)
                {
                    //counter_samples++;
                }
                sample_hdr = NEXT_SAMPLE_HEADER(sample_hdr);
            }
        }
    }
}

int main()
{
    pcap_if_t *alldevs;
    pcap_if_t *d;
    int inum, count;
    int i = 0;
    pcap_t *adhandle;
    u_int netmask;
    struct bpf_program fcode;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        return EXIT_FAILURE;
    }

    for (d = alldevs; d; d = d->next)
    {
        printf("%d. %s", ++i, d->name);
        if (d->description)
            printf(" (%s)\n", d->description);
        else
            printf(" (No description available)\n");
    }

    if (i == 0)
    {
        printf("\nNo interfaces found!\n");
        return EXIT_FAILURE;
    }

    printf("Enter the interface number (1-%d):", i);
    scanf("%d", &inum);

    if (inum < 1 || inum > i)
    {
        printf("\nInterface number out of range.\n");
        pcap_freealldevs(alldevs);
        return EXIT_FAILURE;
    }

    for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);

    if ((adhandle = pcap_open_live(d->name,          // 设备名
                                   65536,            // 65535保证能捕获到不同数据链路层上的每个数据包的全部内容
                                   PCAP_OPENFLAG_PROMISCUOUS,    // 混杂模式
                                   1000,             // 读取超时时间
                                   errbuf            // 错误缓冲池
    )) == NULL)
    {
        fprintf(stderr, "\nUnable to open the adapter.\n");
        pcap_freealldevs(alldevs);
        return EXIT_FAILURE;
    }

    /* No need to care about netmask, it won't be used in this filter */
    netmask = 0xffffffff;

    if (pcap_compile(adhandle, &fcode, "udp", 1, netmask) < 0)
    {
        fprintf(stderr, "\nUnable to compile the packet filter. Check the syntax.\n");
        pcap_freealldevs(alldevs);
        return EXIT_FAILURE;
    }

    if (pcap_setfilter(adhandle, &fcode) < 0)
    {
        fprintf(stderr, "\nError setting filter.\n");
        pcap_freealldevs(alldevs);
        return EXIT_FAILURE;
    }

    printf("\nlistening on %s...\n", d->name);

    pcap_freealldevs(alldevs);

    pcap_loop(adhandle, 0, packet_handler, (u_char *) &count);

    return EXIT_SUCCESS;
}
