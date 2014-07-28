/*
 * =====================================================================================
 *
 *       Filename:  om_print_test.c
 *
 *    j
 *
 *        Version:  1.0
 *        Created:  07/28/2014 11:39:08 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (aj), atjalex@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#include	"order-maintenance-par-rebalance.h"
#include	<stdlib.h>
#include	<stdio.h>
#include	<time.h>


int main ( int argc, char *argv[] )
{
	/// User specifies the number of nodes to insert
	int num_nodes = atoi(argv[1]);
	int i = 0, j, split_flag = 0;

	OM_Node ** nodeArray = malloc(num_nodes * sizeof(OM_Node *));
	
	/// Create the list
 	Top_List * list = create_tl(); 


	/// Assign memory to all the nodes to be inserted
	for (;i < num_nodes; i++)
	{
		nodeArray[i]= malloc(sizeof(OM_Node));
		nodeArray[i]->ID = i;
	}


	// Create and add the first relevant thing to the ds
	first_insert(list, nodeArray[0]);

	i = 1;
	for (; i < num_nodes; i++)
	{
		j = i - 1;

		insert(nodeArray[j], nodeArray[i]);
	}

	print_tree(list);

	free_tl(list);

	return EXIT_SUCCESS;
}


