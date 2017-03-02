
/* This is Server */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <memory.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <errno.h>
#include <sys/stat.h>
#include "list.h"
#include "my_configure.h"

void init_my_entries();
entry_t *entry_alloc();
int insert_entry(entry_t *myentry);
void entry_dump(entry_t *ent);
void entries_dump();
int entry_search(int id);
int delete_entry(int id);
void entry_free(entry_t *ent);
void insert_to_queue(data_t *myentry);	

entries_t my_entries;
recv_queues_t recv_queues;

void init_my_entries(){
	int i;
	memset(&my_entries, 0x00, sizeof(entries_t));
	spin_lock_init(&my_entries.pool_lock);
	INIT_HLIST_HEAD(&my_entries.free_head);
	for(i=0 ; i<MAX_ENTRY ; i++){
		my_entries.entries[i].index=i;
		INIT_HLIST_NODE(&my_entries.entries[i].hlist_node);
		hlist_add_head(&my_entries.entries[i].hlist_node, &my_entries.free_head);
	}
	my_entries.free_count = MAX_ENTRY;

	for(i=0 ; i<HASH_LEN ; i++){
		spin_lock_init(&my_entries.hash[i].lock);
		INIT_HLIST_HEAD(&my_entries.hash[i].head);
		my_entries.hash[i].num=0;
	}
}

void init_queues(){
	int i;
	memset(&recv_queues, 0x00, sizeof(recv_queues_t));
	recv_queues.index=0;
	for(i=0 ; i<QUEUE_NUM ; i++){
		recv_queues.queues[i].read_index = 0;
		recv_queues.queues[i].write_index = 0;

	}

}

entry_t *entry_alloc(){
	hlist_node_t *node;
	int index;
	entry_t *ent;
	if(my_entries.free_count == 0)
		return NULL;

	spin_lock(&my_entries.pool_lock);
	node = my_entries.free_head.first;
	hlist_del(node);
	my_entries.free_count --;
	spin_unlock(&my_entries.pool_lock);
	ent = container_of(node, entry_t, hlist_node);
	index = ent->index;
	memset((char *)ent, 0x00, sizeof(entry_t));
	ent->index = index ;
	return ent;
}

int entry_search(int id){
	int hash_val;
	entries_hash_list_t *hash_entry;
	hlist_node_t *pos;
	entry_t *ent;
	
	hash_val = id % HASH_LEN;
	hash_entry = &my_entries.hash[hash_val];
	if(hash_entry->num == 0)
		return 0;

	spin_lock(&hash_entry->lock);
	hlist_for_each(pos, &hash_entry->head){
		ent = container_of(pos, entry_t, hlist_node);
		if(id == ent->id){
			spin_unlock(&hash_entry->lock);
			return 1;
		}
	}
	spin_unlock(&hash_entry->lock);
	return 0;
}

int insert_entry(entry_t *myentry){
	entry_t *ent;
	entries_hash_list_t *hash_entry;
	int hash_val;
	int i;

	i = entry_search(myentry->id);
	if(i == 1){
		printf("entry %d exists \n", myentry->id);
		return -1;
	}

	ent = entry_alloc();
	if(ent == NULL)
		return -1;
	printf("entry index = %d\n", ent->index);
	ent->id = myentry->id;
	strncpy(ent->name, myentry->name, sizeof(myentry->name));
	hash_val = myentry->id % HASH_LEN;
	hash_entry = &my_entries.hash[hash_val];
	
	spin_lock(&hash_entry->lock);
	hlist_add_head(&ent->hlist_node, &hash_entry->head);
	hash_entry->num ++;
	spin_unlock(&hash_entry->lock);
	
	return 0;
}

void entry_dump(entry_t *ent){
	printf("Data id : %d \n", ent->id);
	printf("Data name : %s \n", ent->name);
}


void entries_dump(){
	int i;
	entries_hash_list_t *hash_entry;
	hlist_node_t *pos;
	entry_t *ent;
	int count = 0;

	printf("entries dump\n");

	for(i=0 ; i<HASH_LEN ; i++){
		hash_entry=&my_entries.hash[i];
		if(hash_entry->num==0)
			continue;
		printf("hash[%d],has %d entries: \n",i,hash_entry->num);
		spin_lock(&hash_entry->lock);
		hlist_for_each(pos, &hash_entry->head){
			ent=container_of(pos, entry_t, hlist_node);
			entry_dump(ent);
			count++;
		}
		spin_unlock(&hash_entry->lock);
	}
	printf("entries free conut = %d \n", my_entries.free_count);

	if((count + my_entries.free_count) == MAX_ENTRY)
		printf("entries status correct \n");
	else
		printf("entries status wrong \n");

}

void entry_free(entry_t *ent){
	hlist_node_t *node;
	node = (hlist_node_t *)&ent->hlist_node;
	spin_lock(&my_entries.pool_lock);
	hlist_add_head(node, &my_entries.free_head);
	my_entries.free_count ++;
	spin_unlock(&my_entries.pool_lock);

}


int delete_entry(int id){
	int hash_val;
	entries_hash_list_t *hash_entry;
	entry_t *ent;
	hlist_node_t *pos;
	hlist_node_t *node;

	hash_val = id % HASH_LEN;
	hash_entry = &my_entries.hash[hash_val];

	if(hlist_empty(&hash_entry->head))
		return -1;
	
	node = NULL;
	spin_lock(&hash_entry->lock);
	hlist_for_each(pos, &hash_entry->head){
		ent = container_of(pos, entry_t, hlist_node);
		if(id == ent->id){
			hlist_del(pos);
			node = pos;
			hash_entry->num --;
			break;
		}
	}

	spin_unlock(&hash_entry->lock);
	if(node != NULL){
		ent=container_of(node, entry_t, hlist_node);
		entry_free(ent);
		return 0;
	}
	return -1;
}





void *recive(){
	data_t *data;
	struct sockaddr_in si_me, si_other;
	int s;
	unsigned int slen=sizeof(si_other);

	if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		diep("socket");

	memset((char *)&si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(s, (struct sockaddr *) &si_me, sizeof(si_me))==-1)
		diep("bind");

	while(1){
		if(recvfrom(s, data, sizeof(data_t), 0, (struct sockaddr *) &si_other, &slen)==-1)
			diep("recvfrom()");
		printf("Received packet from %s:%d\nCommand: %d\tData id: %d\tData name:%s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), data->command, data->id, data->name);
			

		insert_to_queue(data);	






	/*	
		if(myentry->command == 1){ // insert
			insert_entry(myentry);
		}else if(myentry->command == 2){ //delete
			delete_entry(myentry->id);
		}else{
			printf("command %d not exist! \n", myentry->command);
		}

		entries_dump();
	*/	
	}

	close(s);


}


void insert_to_queue(data_t *data){
	int write_index;
	int read_index;
	data_t *q_data; //queue data

	write_index = recv_queues.queues[recv_queues.index].write_index;
	read_index = recv_queues.queues[recv_queues.index].read_index;
	
	write_index = (write_index+1) % QUEUE_LEN;
	while(write_index == read_index){
		printf("recv queues %d is full \n",recv_queues.index);
		sleep(1);
		recv_queues.index ++;
		write_index = recv_queues.queues[recv_queues.index].write_index;
		read_index = recv_queues.queues[recv_queues.index].read_index;
	}


	q_data = &(recv_queues.queues[recv_queues.index].queue[write_index]);
	q_data->id = data->id;
	//printf("q_data id : %d \n", q_data->id);
	strncpy(q_data->name, data->name, sizeof(data->name));
	q_data->command = data->command;

	printf("insert data to recv queue %d :\n", recv_queues.index);
	printf("data command : %d \tdata id : %d \tdata name : %s \n", q_data->command, q_data->id, q_data->name);

	recv_queues.index = (recv_queues.index+1) % QUEUE_NUM;
	recv_queues.queues[recv_queues.index].write_index = write_index;
}


int main(void){
	pthread_t pthread_recive;
	init_my_entries();
	init_queues();
	pthread_create(&pthread_recive, NULL, recive, NULL);
	while(1){
		sleep(1);
	}
	return 0;
}
