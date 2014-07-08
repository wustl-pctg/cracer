/*
 * =====================================================================================
 *
 *       Filename:  OM_DS_mixed_test.c
 *
 *    Description:  Tests top and bottom lists are working correctly
 *
 *        Version:  1.0
 *        Created:  07/07/2014 03:26:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (aj), atjalex@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<stdlib.h>
#include	<stdio.h>
#include	"OM_DS.h"
#include	"OM_DS_bender.h"
#include	"OM_sublist.h"

int main ( int argc, char *argv[] )
{
	int num_nodes = atoi(argv[1]);
	int i = 0, j;
	OM_Node ** nodeArray = malloc(num_nodes * sizeof(OM_Node *));
	OM_DS * firstOM_DS =	(OM_DS *)OM_DS_init();
	firstOM_DS->id = 1;
	Top_List * list = init_top_list(); 

	for (;i < num_nodes; i++)
	{
		nodeArray[i]= malloc(sizeof(OM_Node));
		nodeArray[i]->id = i;
		
	}
	
	append_first_list(list, firstOM_DS, ENGLISH_ID);

	OM_DS_add_first_node(firstOM_DS, nodeArray[0], ENGLISH_ID);
	OM_DS_add_first_node(firstOM_DS, nodeArray[0], HEBREW_ID);


	i = 1;
	for (; i < num_nodes; i++)
	{
		 if (OM_DS_insert(nodeArray[i - 1], nodeArray[i], ENGLISH_ID))
		 {
		 	printf("TEST: splitting and adding to top at test index %i\n", i);

		 	Split_and_add_to_top(list, nodeArray[i]->ds_e);
			Rebalance_bottom_lists(list);
		 }

		if (OM_DS_insert(nodeArray[i - 1], nodeArray[i], HEBREW_ID))
		{
			printf("TEST: splitting and adding to top at test index %i\n", i);

			Split_and_add_to_top(list, nodeArray[i]->ds_h);
			Rebalance_bottom_lists(list);
		
		}
		printf("Check for correctness..");
		check_correctness(list);
		print_top_list(list);
		printf("..done %d\n", i);
	}
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */


