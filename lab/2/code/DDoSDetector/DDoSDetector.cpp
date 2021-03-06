#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "DDoSDetector.h"


DDoSDetector::DDoSDetector()
{
    m_abnormal_win_cnt = 0;
    m_sliding_times = 0;
}

DDoSDetector::~DDoSDetector()
{
}

void DDoSDetector::recv_packet(PacketInfo pkt_info)
{
    if (m_flow_window.size() < MAX_PKT_WINDOW)
    {
        m_flow_window.push(pkt_info);
    }
    else if (m_flow_window.size() >= MAX_PKT_WINDOW)
    {
        check_flow_window();
    }
}

void DDoSDetector::check_flow_window()
{
    int abnormal_pkt_cnt = 0;

    for (int i = 0; i < m_flow_window.size(); i++)
    {
        PacketInfo pkt = m_flow_window[i];
        switch (pkt.type)
        {
            case ICMP_REQUEST_PACKET:
            case TCP_SYN_PACKET:
                abnormal_pkt_cnt++;
                if (m_abnormal_packets.find(pkt) == m_abnormal_packets.end())
                {
                    m_abnormal_packets[pkt] = 1;
                }
                else
                {
                    m_abnormal_packets[pkt]++;
                }
                break;
        }
    }

    if (abnormal_pkt_cnt >= ABNORMAL_PKT_THRESHOLD)
    {
        /**
         * if the number of abnormal windows exceeds the threshold level,
         * DDoS attack is likely to occur.
         */
        if (++m_abnormal_win_cnt >= ABNORMAL_WIN_THRESHOLD)
        {
            m_abnormal_win_cnt = 0; /* reset counter. */
            handle_attack();
        }

        /* current flow window is abnormal, clear it for next coming packets. */
        printf("[DDoSDetector] current flow window is abnormal, clear it for next coming packets.\n");
        m_flow_window.clear();
    }
    else
    {
        /* current window is normal, slide it forward. */
        printf("[DDoSDetector] current flow window is normal, slide it forward.\n");
        for (int i = 0; i < SLIDING_LEN; i++)
        {
            if (!m_flow_window.empty()) m_flow_window.pop();
        }

        if (++m_sliding_times >= SLIDING_TIMES_THRESHOLD)
        {
            /**
             * flow window has slid forward `m_sliding_times' times, but no
             * abnormal windows are detected, so we can consider that DDoS
             * attack didn't occur, then reset counter.
             */
            printf("[DDoSDetector] flow window has slid %d times, no abnormal windows detected.\n", m_sliding_times);

            m_abnormal_win_cnt = 0;
            m_sliding_times = 0;
            m_flow_window.clear();
            m_abnormal_packets.clear();
        }
    }
}

void DDoSDetector::handle_attack()
{
    in_addr dst;
    char *str_dst, *param;
    uint16_t tp_dst;
    str_dst = new char[32];
    param = new char[256];

    std::map<PacketInfo, int>::iterator it = m_abnormal_packets.begin();
    for (; it != m_abnormal_packets.end(); it++)
    {
        if (it->second < ABNORMAL_PKT_THRESHOLD) continue;
        /* else this packet is abnormal. */

        PacketInfo pkt = it->first;
        dst.s_addr = pkt.dst_ip;
        tp_dst = htons(pkt.tcp_dport);
        strcpy(str_dst, inet_ntoa(dst));

        printf("[DDoSDetector] DDoS attack detected!\n");
        printf("[DDoSDetector] dispatch firewall rule:\n");

        switch (pkt.type)
        {
            case ICMP_REQUEST_PACKET:
                sprintf(param,
                        "{\"src-mac\":\"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\","\
                        "\"dst-mac\":\"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\","\
                        "\"dst-ip\":\"%s/32\","\
                        "\"nw-proto\":\"ICMP\","\
                        "\"action\":\"DENY\","\
                        "\"priority\":\"0\"}", /* highest priority */
                        pkt.src_mac[0], pkt.src_mac[1], pkt.src_mac[2], pkt.src_mac[3], pkt.src_mac[4], pkt.src_mac[5],
                        pkt.dst_mac[0], pkt.dst_mac[1], pkt.dst_mac[2], pkt.dst_mac[3], pkt.dst_mac[4], pkt.dst_mac[5],
                        str_dst);

                printf("%s\n", param);
                m_CurlHelper.do_post(URL_FIREWALL_RULES, param);
                break;
            case TCP_SYN_PACKET:
                sprintf(param,
                        "{\"src-mac\":\"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\","\
                        "\"dst-mac\":\"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\","\
                        "\"dst-ip\":\"%s/32\","\
                        "\"tp-dst\":\"%d\","\
                        "\"nw-proto\":\"TCP\","\
                        "\"action\":\"DENY\","\
                        "\"priority\":\"0\"}", /* highest priority */
                        pkt.src_mac[0], pkt.src_mac[1], pkt.src_mac[2], pkt.src_mac[3], pkt.src_mac[4], pkt.src_mac[5],
                        pkt.dst_mac[0], pkt.dst_mac[1], pkt.dst_mac[2], pkt.dst_mac[3], pkt.dst_mac[4], pkt.dst_mac[5],
                        str_dst, tp_dst);

                printf("%s\n", param);
                m_CurlHelper.do_post(URL_FIREWALL_RULES, param);
                break;
            default:
                fprintf(stderr, "[DDoSDetector] unknown packet type.\n");
                break;
        }
    }
    delete param;
    delete str_dst;
    m_abnormal_packets.clear();
}
