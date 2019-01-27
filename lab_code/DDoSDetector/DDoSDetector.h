#ifndef DDOS_DETECTOR_H
#define DDOS_DETECTOR_H

#include <vector>
#include <map>
#include "types.h"
#include "CurlHelper.h"

#define ICMP_REQUEST_PACKET     1
#define TCP_SYN_PACKET          2
#define MAX_PKT_WINDOW          50
#define ABNORMAL_WIN_THRESHOLD  5

#define ERROR(msg) fprintf(stderr, msg); \
                   exit(EXIT_FAILURE)


struct packet_info
{
    uint8_t type;
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t tcp_sport;
    uint16_t tcp_dport;

    bool operator<(const packet_info &other) const
    {
        if (src_ip < other.src_ip)              return true;
        else if (dst_ip < other.dst_ip)         return true;
        else if (tcp_sport < other.tcp_sport)   return true;
        else if (tcp_dport < other.tcp_dport)   return true;
        else                                    return false;
    }
};

class DDoSDetector
{
public:
    DDoSDetector();

    virtual ~DDoSDetector();

    void pkt_enqueue(struct packet_info pkt_info);

    void clear_pkt_window();

    int get_pkt_window_size();

    int get_abnormal_win_cnt();

private:
    void check_pkt_window();

    void handle_attack(std::map<struct packet_info, struct packet_info> &pkt_abnormal);

private:
    std::vector<struct packet_info> m_pkt_window;
    int m_abnormal_win_cnt;
    CurlHelper m_CurlHelper;
};

#endif /* DDOS_DETECTOR_H */
