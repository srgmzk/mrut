/*
* graph lib
* graph is collection of nodes
*/

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "gluethread/glthread.h"

#define NODE_NAME_SIZE		16
#define IF_NAME_SIZE		16
#define MAX_INTF_PER_NODE 	10


//Forward declaration
typedef struct node_ node_t;
typedef struct link_ link_t;


typedef struct interface_ {
	char if_name[IF_NAME_SIZE];
	struct node_ *att_node;
	struct link_ *link;
} interface_t;

struct link_ {
	interface_t intf1;
	interface_t intf2;
	unsigned int cost;
}

stuct node_ {
	char node_name[NODE_NAME_SIZE];
	interface_t *intf[MAX_INTF_PER_NODE];
	glthread_t graph_glue;
}

typedef struct graph_{
	char topology_name[32]; //uniq name to every graph (network topo)
	glthread_t node_list;
	
}



#endif
