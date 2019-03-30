#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define ECHO_PORT 9999
#define BUF_SIZE 4096

int g_conn_num; // 连接数量

typedef struct {
    int sockfd;
    struct sockaddr_in addr;
} thread_param, *pthread_param;

int close_socket(int sock) {
    if (close(sock)) {
        fprintf(stderr, "Failed closing socket.\n");
        return 1;
    }
    return 0;
}

void *recv_thread(void *param) {
    pthread_param p = (pthread_param) param;
    int client_sock = p->sockfd;
    struct sockaddr_in cli_addr = p->addr;
    ssize_t nr_recv;
    char recv_buf[BUF_SIZE] = {0};

    // 从连接套接口接收客户端发送来的数据
    // recv 会一直等待, 直到有数据可读或客户端断开连接
    while ((nr_recv = recv(client_sock, recv_buf, BUF_SIZE, 0)) > 1) {
        //printf("\nmessage from client [%s]: %s",
               //inet_ntoa(cli_addr.sin_addr), recv_buf);

        //将接收的数据直接发送给客户端
        recv_buf[nr_recv] = 0; // end of string
        if (send(client_sock, recv_buf, sizeof(recv_buf), 0) == -1) {
            close_socket(client_sock);
            fprintf(stderr, "Error sending message to client.\n");
            return NULL;
        }
        memset(recv_buf, 0, BUF_SIZE);
    }

    if (nr_recv == -1) {
        close_socket(client_sock);
        fprintf(stderr, "Error reading from client socket.\n");
        return NULL;
    }

    if (close_socket(client_sock)) {
        fprintf(stderr, "Error closing client socket.\n");
        return NULL;
    } else {
        printf("connection was closed by client [%s]",
               inet_ntoa(cli_addr.sin_addr));
        printf(" (conn_num = %d)", --g_conn_num);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int sock; // 服务端监听套接字
    int client_sock; // 客户端连接套接字
    pthread_t tid;
    socklen_t cli_size;
    struct sockaddr_in addr; // 服务端地址结构
    struct sockaddr_in cli_addr; // 客户端地址结构

    if (argc != 3)
    {
        printf("usage: %s <server-ip> <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* 创建监听套接字 */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Failed creating socket.\n");
        return EXIT_FAILURE;
    }

    addr.sin_family = AF_INET; // IPV4
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    /* 绑定监听套接字*/
    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr))) {
        close_socket(sock);
        perror("bind");
        return EXIT_FAILURE;
    }

    /* 监听 */
    if (listen(sock, 5)) // 监听队列的长度为5
    {
        close_socket(sock);
        perror("listen");
        return EXIT_FAILURE;
    }

    printf("----- Server -----\n");
    printf("Listening at %d ...\n", atoi(argv[2]));

    /* 循环检测是否有连接请求，若有则创建连接套接字并做相应处理 */
    g_conn_num = 0;
    while (1) {
        cli_size = sizeof(cli_addr);
        if ((client_sock = accept(sock, (struct sockaddr *) &cli_addr,
                                  &cli_size)) == -1) {
            close_socket(sock);
            fprintf(stderr, "Error accepting connection.\n");
            return EXIT_FAILURE;
        }
        printf("\nreceived a connection from [%s]",
               inet_ntoa(cli_addr.sin_addr));
        printf(" (conn_num = %d)", ++g_conn_num);

        // 线程参数结构体
        thread_param param;
        param.sockfd = client_sock;
        param.addr = cli_addr;

        int ret = pthread_create(&tid, NULL, recv_thread, (void *) &param);
        if (ret < 0) {
            close_socket(client_sock);
            close_socket(sock);
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }

    close_socket(sock);
    return EXIT_SUCCESS;
}
