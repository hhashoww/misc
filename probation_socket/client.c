
/* This is Client */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <memory.h>
#include "list.h"
#include "my_configure.h"

#define SRV_IP "127.0.0.1"

int main(int argc, char *argv[]){
	data_t *data=NULL;
	struct sockaddr_in si_other;
	int s, slen=sizeof(si_other);

	if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		diep("socket");

	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family=AF_INET;
	si_other.sin_port = htons(PORT);
	if(inet_aton(SRV_IP, &si_other.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	data = (data_t *)malloc(sizeof(data_t));
	data->command = atoi(argv[1]);
	data->id = atoi(argv[2]);
	strncpy(data->name, argv[3], sizeof(argv[3]));

	printf("Sending command : %d\tid: %d\tname: %s\n", data->command, data->id, data->name);
	
	if (sendto(s, data, sizeof(data_t), 0, (struct sockaddr *)&si_other, slen)==-1)
		diep("sendto()");

	close(s);
	return 0;

}
