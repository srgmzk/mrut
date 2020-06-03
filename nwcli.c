/*
 * =====================================================================================
 *
 *       Filename:  nwcli.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/01/2020 01:06:50 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "CommandParser/libcli.h"
#include "CommandParser/cmdtlv.h"
#include "cmdcodes.h"
#include "graph.h"
#include <stdio.h>

extern graph_t *topo;

/*Generic Topology Commands*/
static int
show_nw_topology_handler(param_t *param, ser_buff_t *tlv_buf, op_mode enable_or_disable) {
	int CMDCODE = -1;
	CMDCODE = EXTRACT_CMD_CODE(tlv_buf);
	
	switch(CMDCODE) {
		case CMDCODE_SHOW_NW_TOPOLOGY:
			dump_nw_graph(topo);
			break;
		default:
			;	
	}

}

int
node_callback_handler(param_t *param, ser_buff_t *tlv_buf, op_mode enable_or_disable){
    printf("%s() is called ...\n", __FUNCTION__);
    return 0;
}

int
validate_node_name(char *value){

    printf("%s() is called with value = %s\n", __FUNCTION__, value);
    return VALIDATION_SUCCESS; /*else return VALIDATION_FAILED*/
}

static int
arp_handler(param_t *param, ser_buff_t *tlv_buf, op_mode enable_or_disable) { 
	int CMDCODE = -1;

	tlv_struct_t *tlv = NULL;
	char *node_name = NULL;
	char *ip_addr = NULL;

	CMDCODE = EXTRACT_CMD_CODE(tlv_buf);

	TLV_LOOP_BEGIN(tlv_buf, tlv){
    	if(strncmp(tlv->leaf_id, "node_name", strlen("node_name")) == 0) { 
            node_name = tlv->value;  
        } else if(strncmp(tlv->leaf_id, "ip_address", strlen("ip-address")) == 0) { 
          	ip_addr = tlv->value;                                              
        }   
    } TLV_LOOP_END;


	switch(CMDCODE) {
		case CMDCODE_RUN_NODE_ARP_RESOLVE:
			printf("node_name = %s. ip-address = %s", node_name, ip_addr); 
			break;
		default:
			;	
	}

	return 0;
}



void
nw_init_cli() {

	init_libcli();
	
	param_t *show 		= libcli_get_show_hook();
	param_t *debug 		= libcli_get_debug_hook();
	param_t *config		= libcli_get_config_hook();
	param_t *run 		= libcli_get_run_hook();
	param_t *debug_show = libcli_get_debug_show_hook();
	param_t *root 		= libcli_get_root();

	{
		/*show topology*/
		static param_t topology;
		init_param(&topology, CMD, "topology", show_nw_topology_handler, 0, INVALID, 0, "Dump Complete Network Topology");
		libcli_register_param(show, &topology);
		set_param_cmd_code(&topology, CMDCODE_SHOW_NW_TOPOLOGY);
		
	}

	/*
	 *	RUN COMMANDs
	 *	
	*/	

	{
        /*run node*/
        static param_t node;    /* Get the param_t variable, either a static memory or heap memory, not stack */ 

        init_param(&node, CMD, "node", 0, 0, INVALID, 0, "Help : node");
        libcli_register_param(run, &node); /*Add node param as suboption of run param*/
		
		{ 
		
			/* run node <node name> resolve-arp <ip-address> */
			static param_t node_name;

			init_param(&node_name, LEAF, 0, node_callback_handler, validate_node_name, STRING, "node_name", \
						"Help: Node name" );
			libcli_register_param(&node, &node_name);
			
			{ 
				static param_t run_cmd;
				init_param(&run_cmd, CMD, "arp-resolve", 0, 0, INVALID, 0, "Help: arp resolve");
        		libcli_register_param(&node_name, &run_cmd); /*Add node param as suboption of  param*/
				{
					/* resolve arp */
					static param_t ip_addr;	
					init_param(&ip_addr, LEAF, 0, arp_handler, 0, IPV4, "ip_address", "Help: ip address A.B.C.D");
					libcli_register_param(&run_cmd, &ip_addr);

					set_param_cmd_code(&ip_addr, CMDCODE_RUN_NODE_ARP_RESOLVE);								
				}
			
			}	
		}

	}
	{

        static param_t node; 
	}
	support_cmd_negation(config);


	
}

