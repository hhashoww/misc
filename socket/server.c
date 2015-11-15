#include "common.h"
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SVR_IP                          "127.0.0.1"
#define SVR_PORT                        5678
#define BUF_SIZE                        2000

int main (int argc, char **argv) {
    struct sockaddr_in  server_addr;
    struct sockaddr_in  client_addr;
    socklen_t           len;
    fd_set              active_fd_set;
    int                 sock_fd;
    int                 max_fd;
    int                 n;
    int                 ret;
    int                 flag = 1;
    char                buff[BUF_SIZE];
    gox_syscp_agent_data_t data;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SVR_IP);
    server_addr.sin_port = htons(SVR_PORT);
    len = sizeof(struct sockaddr_in);

    /* Create endpoint */
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket()");
        return -1;
    } else {
        printf("sock_fd=[%d]\n", sock_fd);
    }

    /* Set socket option */
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) < 0) {
        perror("setsockopt()");
        return -1;
    }

    /* Bind */
    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind()");
        return -1;
    } else {
        printf("bind [%s:%u] success\n",
                inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
    }


    int cnt= 0 ;
#if 1
    FD_ZERO(&active_fd_set);
    FD_SET(sock_fd, &active_fd_set);
    max_fd = sock_fd;
    while(1) {

        //printf("waiting.....\n");
        struct timeval  tv;
        fd_set          read_fds;

        /* Set timeout */
        tv.tv_sec = 0;
        tv.tv_usec = 1000;

        /* Copy fd set */
        read_fds = active_fd_set;
        ret = select(max_fd + 1, &read_fds, NULL, NULL, &tv);

        if (ret == -1) {
            //perror("select()");
            return -1;
        } else if (ret == 0) {
            //printf("select timeout\n");
            //printf("cnt = %d\n", cnt);
            continue;
        } else {
            int i;

            /* Service all sockets */
            for (i = 0; i < FD_SETSIZE; i++) {
                if (FD_ISSET(i, &read_fds)) {
                    if (i == sock_fd) {
                        /*
                        n = recvfrom(sock_fd, buff, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &len);
                        buff[n] = 0;
                        cnt++;
                        printf("recv %s\n", buff);
                        */
                        memset(&data, 0x00, sizeof(gox_syscp_agent_data_t));
                        n = recvfrom(sock_fd, &data, sizeof(gox_syscp_agent_data_t), 0, (struct sockaddr *)&client_addr, &len);
                        if (n > 0) {
                            printf("type = %d\n", data.data_type);
                            switch(data.data_type) {
                                case flow :
                                    printf("flow\n");
                                    break;
                                case pkt_in:
                                    printf("pkt_in\n");
                                    break;
                                default:
                                    printf("unknown\n");
                            }
                        }
                    }
                } // end of if
            } //end of for
        } // end of if
#else
        while(1) {
            n = recvfrom(sock_fd, buff, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &len);
            buff[n]=0;
            cnt += n;
            printf("recv %s, strlen = %ld, cnt  = %d\n", buff, strlen(buff), cnt);
#endif
        }

    return 0;
}
