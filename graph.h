/*
* graph lib
* graph is collection of nodes
*/

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "gluethread/glthread.h"
#include <stddef.h>

#define NODE_NAME_SIZE		16
#define IF_NAME_SIZE		16
#define MAX_INTF_PER_NODE 	10

#define container_of(ptr, type, member) ({ \
	const typeof( ((type *)0)->member ) *__mptr = (ptr); \
	(type *)( (char *)__mptr - offsetof(type,member) );})

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
};

struct node_ {
	char node_name[NODE_NAME_SIZE];
	interface_t *intf[MAX_INTF_PER_NODE];
	glthread_t graph_glue;
};

typedef struct graph_ {
	char topology_name[32]; //uniq name to every graph (network topo)
	glthread_t node_list;
} graph_t;


graph_t *create_new_graph(char *);
node_t *create_graph_node(graph_t *, char *);
void insert_link_between_two_nodes(node_t *, node_t *, char *, char *, int); 

static inline node_t *
get_nbr_node(interface_t *interface) {
	node_t *selfnode = interface->att_node;
	link_t *selflink = interface->link;
	
	link_t *l1 = container_of(interface, link_t, intf1);
	link_t *l2 = container_of(interface, link_t, intf2);

/*	printf("%s\n", l1->intf1.if_name);
	printf("%s\n", l2->intf1.if_name);
	printf("%s\n", interface->if_name);

	printf("%p\n", &(l1->intf1));
	printf("%p\n", &(l2->intf1));
	printf("%p\n", interface);
*/	
	return ( &(l1->intf1) == interface ) ? selflink->intf2.att_node : selflink->intf1.att_node; 
		
};

static inline int 
get_node_intf_available_slot(node_t *node) {
	int i = 0;
	while ( i < MAX_INTF_PER_NODE )
		if ( node->intf[i] ) i++;
		else return i;
	return -1;
};
	

#endif
