#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "DDoSDetector.h"


DDoSDetector::DDoSDetector()
{
    m_abnormal_win_cnt = 0;
}

DDoSDetector::~DDoSDetector()
{
}

void DDoSDetector::pkt_enqueue(struct packet_info pkt_info)
{
    if (m_pkt_window.size() < MAX_PKT_WINDOW)
    {
        m_pkt_window.push_back(pkt_info);
    }
    else if (m_pkt_window.size() == MAX_PKT_WINDOW)
    {
        check_pkt_window();
    }
}

void DDoSDetector::clear_pkt_window()
{
    m_pkt_window.clear();
    m_abnormal_win_cnt = 0;
}

int DDoSDetector::get_pkt_window_size()
{
    return m_pkt_window.size();
}

int DDoSDetector::get_abnormal_win_cnt()
{
    return m_abnormal_win_cnt;
}

void DDoSDetector::check_pkt_window()
{
    int abnormal_pkt_cnt = 0;
    std::map<struct packet_info, struct packet_info> pkt_abnormal;
    std::map<struct packet_info, int> pkt_stat;

    for (int i = 0; i < m_pkt_window.size(); i++)
    {
        switch (m_pkt_window[i].type)
        {
            case ICMP_REQUEST_PACKET:
            case TCP_SYN_PACKET:
                abnormal_pkt_cnt++;
                if (pkt_stat.count(m_pkt_window[i]) == 0)
                {
                    pkt_stat[m_pkt_window[i]] = 1;
                }
                else
                {
                    pkt_stat[m_pkt_window[i]]++;
                    if (pkt_stat[m_pkt_window[i]] > MAX_PKT_WINDOW / 3)
                    {
                        pkt_abnormal[m_pkt_window[i]] = m_pkt_window[i];
                    }
                }
                break;
        }
    }
    m_pkt_window.clear();

    if (abnormal_pkt_cnt > MAX_PKT_WINDOW / 3)
    {
        m_abnormal_win_cnt++; /* current window is abnormal, increase counter. */
        /**
         * if the number of abnormal windows exceeds the threshold level,
         * DDoS attack is likely to occur.
         */
        if (m_abnormal_win_cnt == ABNORMAL_WIN_THRESHOLD)
        {
            printf("[DDoSDetector] DDoS attack detected!\n");
            m_abnormal_win_cnt = 0;

            handle_attack(pkt_abnormal);
        }
    }
    else
    {
        m_abnormal_win_cnt = 0; /* current window is NOT abnormal, reset counter. */
    }
}

void DDoSDetector::handle_attack(std::map<struct packet_info, struct packet_info> &pkt_abnormal)
{
    char *url = "http://localhost:8080/wm/firewall/rules/json";

    for (std::map<struct packet_info, struct packet_info>::iterator it = pkt_abnormal.begin();
         it != pkt_abnormal.end(); it++)
    {
        in_addr src, dst;
        char *str_src, *str_dst, *param;
        uint16_t tp_src, tp_dst;

        struct packet_info pi = it->second;
        src.s_addr = pi.src_ip;
        dst.s_addr = pi.dst_ip;
        tp_src = htons(pi.tcp_sport);
        tp_dst = htons(pi.tcp_dport);

        param = (char *) malloc(sizeof(char) * 256);
        memset(param, 0, sizeof(param));

        str_src = (char *) malloc(strlen(inet_ntoa(src)) + 1);
        strcpy(str_src, inet_ntoa(src));
        str_dst = (char *) malloc(strlen(inet_ntoa(dst)) + 1);
        strcpy(str_dst, inet_ntoa(dst));

        switch (pi.type)
        {
            case ICMP_REQUEST_PACKET:
                printf("[DDoSDetector] dispatch firewall rule:\n");
                printf("\tsrc-ip: %s, dst-ip: %s\n", str_src, str_dst);
                printf("\tnw-proto: ICMP\n");
                printf("\taction: DENY\n");

                sprintf(param,
                        "{\"src-ip\":\"%s/32\",\"dst-ip\":\"%s/32\",\"nw-proto\":\"ICMP\",\"action\":\"DENY\"}",
                        str_src, str_dst);
                m_CurlHelper.do_post(url, param);
                break;
            case TCP_SYN_PACKET:
                printf("[DDoSDetector] dispatch firewall rule:\n");
                printf("\tsrc-ip: %s, dst-ip: %s\n", str_src, str_dst);
                printf("\ttp-src: %d, tp_dst: %d\n", tp_src, tp_dst);
                printf("\tnw-proto: TCP\n");
                printf("\taction: DENY\n");

                sprintf(param,
                        "{\"src-ip\":\"%s/32\",\"dst-ip\":\"%s/32\",\"tp-src\":\"%d\",\"tp-dst\":\"%d\",\"nw-proto\":\"TCP\",\"action\":\"DENY\"}",
                        str_src, str_dst, tp_src, tp_dst);
                m_CurlHelper.do_post(url, param);
                break;
        }
        free(str_src);
        free(str_dst);
        free(param);
    }
}
