#include "common.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>


#define SVR_IP                          "127.0.0.1"
int main(int argc, char**argv)
{
    int sockfd,n;
    struct sockaddr_in servaddr,cliaddr;
    char sendline[1500];
    gox_syscp_agent_data_t data;

    if (argc != 2)
    {
        printf("usage:  udpcli <IP address>\n");
        return -1;
    }

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(SVR_IP);
    servaddr.sin_port=htons(5678);

    /*
    while (fgets(sendline, 10000,stdin) != NULL)
    {
        sendto(sockfd,sendline,strlen(sendline),0,
                (struct sockaddr *)&servaddr,sizeof(servaddr));
    }
    */
    /*
    scanf("%s", sendline);
    int cnt=0;
    int rlt = 0;
    while (cnt < 10000)
    {
        n = sendto(sockfd,sendline,strlen(sendline),MSG_DONTWAIT,
                (struct sockaddr *)&servaddr,sizeof(servaddr));
        rlt += n;
        cnt++;
        //usleep(1000);
    }
    printf("rlt = %d\n", rlt);
    printf("cnt = %d\n", cnt);
    */
    int cnt=0;
    int rlt = 0;
    int type;
    srand(time(NULL));
    while (cnt < 10)
    {
        memset(&data, 0x00, sizeof(gox_syscp_agent_data_t));
        type = rand() % unknown;
        printf("type = %d\n", type);

        switch(type) {
            case flow :
                printf("flow\n");
                data.data_type = flow;
                break;
            case pkt_in:
                printf("pkt_in\n");
                data.data_type = pkt_in;
                break;
            default:
                printf("unknown\n");
                data.data_type = unknown;
        }


        n = sendto(sockfd, &data, sizeof(gox_syscp_agent_data_t), 0,
                (struct sockaddr *)&servaddr,sizeof(servaddr));

        cnt++;
        usleep(1000);
    }
    return 0;
}
