#ifndef MY_CONFIGURE_H
#define MY_CONFIGURE_H

#include <stdlib.h>
#include "list.h"

#define PORT 9930
#define HASH_LEN 10
#define MAX_ENTRY 100
#define QUEUE_LEN 25
#define QUEUE_NUM 4

typedef struct data_s {
	int command; // 1:insert 2:delete
	int id;
	char name[10];
}data_t;

typedef struct entry_s {
	int command; // 1:insert 2:delete
	int id;
	char name[10];
	hlist_node_t hlist_node;
	int index;
}entry_t;

typedef struct entry_hash_list_s {
	hlist_head_t head;
	int num;
	spinlock_t lock;
}entries_hash_list_t;


typedef struct entries_s {
	int free_count;
	spinlock_t pool_lock;
	hlist_head_t free_head;
	entries_hash_list_t hash[HASH_LEN];
	entry_t entries[MAX_ENTRY];
}entries_t;


typedef struct recv_queue_s {
	int write_index;
	int read_index;
	data_t queue[QUEUE_LEN];
}recv_queue_t;


typedef struct recv_queues_s{
	int index;
	recv_queue_t queues[QUEUE_NUM];
}recv_queues_t;


void diep(char *s) {
        perror(s);
        exit(1);
}

#endif
