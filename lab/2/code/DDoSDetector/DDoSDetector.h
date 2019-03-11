#ifndef DDOS_DETECTOR_H
#define DDOS_DETECTOR_H

#include <vector>
#include <map>
#include <queue>
#include <set>
#include "RandomQueue.h"
#include "types.h"
#include "headers.h"
#include "CurlHelper.h"

#define ICMP_REQUEST_PACKET     1
#define TCP_SYN_PACKET          2
#define MAX_PKT_WINDOW          50
#define ABNORMAL_WIN_THRESHOLD  5
#define SLIDING_TIMES_THRESHOLD 5
#define SLIDING_LEN             (MAX_PKT_WINDOW / SLIDING_TIMES_THRESHOLD)
#define URL_FIREWALL_RULES      "http://localhost:8080/wm/firewall/rules/json"

#define ERROR(msg) fprintf(stderr, msg); \
                   exit(EXIT_FAILURE)


struct PacketInfo
{
    uint8_t type;
    uint8_t src_mac[MAC_LEN];
    uint8_t dst_mac[MAC_LEN];
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t tcp_sport;
    uint16_t tcp_dport;

    bool operator<(const PacketInfo &other) const
    {
        bool b1 = type < other.type;
        bool b2 = dst_ip < other.dst_ip;
        bool b3 = tcp_dport < other.tcp_dport;
        bool b4 = memcmp(src_mac, other.src_mac, MAC_LEN) < 0;
        return b1 || b2 || b3 || b4;
    }
};

class DDoSDetector
{
public:
    DDoSDetector();
    virtual ~DDoSDetector();
    void recv_packet(PacketInfo pkt_info);
private:
    void check_flow_window();
    void handle_attack();
private:
    RandomQueue<PacketInfo> m_flow_window; // sliding window
    std::set<PacketInfo> m_abnormal_packets;
    int m_abnormal_win_cnt;
    int m_sliding_times;
    CurlHelper m_CurlHelper;
};

#endif /* DDOS_DETECTOR_H */
