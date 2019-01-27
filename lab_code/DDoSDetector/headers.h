#ifndef HEADERS_H
#define HEADERS_H

#include "types.h"

#define MAC_LEN 6
#define ETHPROTO_IPV4        0x0800 // 以太网上层协议类型: IPv4
#define ETHPROTO_ARP         0x0806 // 以太网上层协议类型: ARP

struct ether_header
{
    uint8_t daddr[MAC_LEN];         // 目的 MAC 地址
    uint8_t saddr[MAC_LEN];         // 源 MAC 地址
    uint16_t proto;                 // 上层协议类型 (0x0800->IPv4, 0x0806->ARP)
}__attribute__((packed));

#define HARD_ETHERNET   0x0001
#define ARP_REQUEST     0x0001
#define ARP_RESPONCE    0x0002

struct arp_header
{
    uint16_t arp_hrd;               // 硬件类型
    uint16_t arp_pro;               // 上层协议类型
    uint8_t arp_hln;                // 硬件地址长度
    uint8_t arp_pln;                // 协议地址长度
    uint16_t arp_op;                // 操作选项 (请求 or 应答)
    uint8_t arp_shaddr[MAC_LEN];    // 发送者硬件地址 (MAC)
    uint32_t arp_spaddr;            // 发送者协议地址 (IPv4)
    uint8_t arp_thaddr[MAC_LEN];    // 目标硬件地址 (MAC)
    uint32_t arp_tpaddr;            // 目标协议地址 (IPv4)
}__attribute__((packed));

#define IPV4PROTO_ICMP  1
#define IPV4PROTO_TCP   6
#define IPV4PROTO_UDP   17

struct ip_header
{
    uint8_t ver_ihl;                // 版本 (4 bits) + 首部长度 (4 bits)
    uint8_t tos;                    // 服务类型(Type of service)
    uint16_t tlen;                  // 总长(Total length)
    uint16_t id;                    // 标识(Identification)
    uint16_t flags_fo;              // 标志位(Flags) (3 bits) + 段偏移量(Fragment offset) (13 bits)
    uint8_t ttl;                    // 存活时间(Time to live)
    uint8_t proto;                  // 上层协议(Protocol)
    uint16_t cksum;                 // 首部校验和(Header checksum)
    uint32_t saddr;                 // 源地址(Source address)
    uint32_t daddr;                 // 目的地址(Destination address)
}__attribute__((packed));

#define ICMP_REQUEST    8
#define ICMP_REPLY      0

struct icmp_header
{
    uint8_t type;                // ICMP数据报类型
    uint8_t code;                // 编码
    uint16_t cksum;              // 校验和
    uint16_t id;                 // 标识(通常为当前进程pid)
    uint16_t seq;                // 序号
}__attribute__((packed));

struct tcp_header
{
    uint16_t sport;              // 源端口号
    uint16_t dport;              // 目的端口号
    uint32_t seq;                // 序号
    uint32_t ack;                // 确认号
    uint8_t lenres;              // 4 bits 的数据偏移和 4 bits 的保留字段
    uint8_t flag;                // 标志
    uint16_t win;                // 窗口长度
    uint16_t cksum;              // 校验和
    uint16_t urp;                // 紧急指针
}__attribute__((packed));

#define FLAG_SYN    0x02 // 0000_0010

struct udp_header
{
    uint16_t sport;            // 源端口(Source port)
    uint16_t dport;            // 目的端口(Destination port)
    uint16_t len;              // UDP数据包长度(Datagram length)
    uint16_t cksum;            // 校验和(Checksum)
}__attribute__((packed));

#define SFLOW_AGENT_ADDR_IPV4     1

struct sflow_header
{
    uint32_t ver;               // Datagram version
    uint32_t addr_type;         // Agent address type
    uint32_t addr;              // Agent address
    uint32_t sub_agent;         // Sub-agent
    uint32_t nr_seq;            // Sequence number
    uint32_t sys_uptime;        // System updated time
    uint32_t nr_samples;        // Number of samples
    /* Here follows `nr_samples' samples, each of which has (`sample_len' + 8) bytes. */
}__attribute__((packed));

#define SFLOW_SAMPLE_TYPE_FLOW      1
#define SFLOW_SAMPLE_TYPE_COUNTER   2
#define EX_DATA_LEN                 24
#define SAMPLE_TYPE(x) (x & 0xfff)

struct sflow_sample_header
{
    uint32_t sample_type;       // [0:19] = Enterprise, [20:31] = sFlow sample type
    uint32_t sample_len;        // sample length
    uint32_t nr_seq;            // Sequence number
    uint32_t unused_padding;
    uint32_t rate;              // Sampling rate
    uint32_t pool;              // Sample pool
    uint32_t dropped_pkts;      // Dropped packets
    uint32_t in_if;             // Input interface
    uint32_t out_if;            // Output interface
    uint32_t record;            // Flow record
    uint8_t ex[EX_DATA_LEN];    // Extended switch data
    struct raw_packet_header
    {
        uint32_t unused_paddding;
        uint32_t flow_len;      // Flow data length
        uint32_t proto;         // Header protocol
        uint32_t frame_len;     // Frame length
        uint32_t payload_rem;   // Payload removed
        uint32_t orig_pkt_len;  // Original packet length
        void *sampled_pkt;      // Beginning of sampled packet, whose lengths depends on `orig_pkt_len'
    } __attribute__((packed)) raw_pkt_hdr;
}__attribute__((packed));

#define ETHER_HEADER(x)         (struct ether_header *)((uint8_t *)x)
#define IP_HEADER(x)            (struct ip_header *)((uint8_t *)x + sizeof(struct ether_header))
#define ICMP_HEADER(x)          (struct icmp_header *)((uint8_t *)x + sizeof(struct ether_header) + sizeof(struct ip_header))
#define TCP_HEADER(x)           (struct tcp_header *)(ICMP_HEADER(x))
#define UDP_HEADER(x)           (struct udp_header *)(ICMP_HEADER(x))
#define SFLOW_HEADER(x)         (struct sflow_header *)((uint8_t *)UDP_HEADER(x) + sizeof(struct udp_header))
#define SFLOW_SAMPLE_HEADER(x)  (struct sflow_sample_header *)((uint8_t *)SFLOW_HEADER(x) + sizeof(struct sflow_header))
#define NEXT_SAMPLE_HEADER(x)   (struct sflow_sample_header *)((uint8_t *)x + htonl(x->sample_len) + 8)

#endif /* HEADERS_H */
