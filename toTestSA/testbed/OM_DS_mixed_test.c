/*
 * =====================================================================================
 *
 *       Filename:  Bottom_List_mixed_test.c
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
#include	<time.h>

extern unsigned long MAX_NUMBER;
extern unsigned long MAX_NUMBER_HALF;


void order_test(OM_Node ** nodeArray, int num_nodes) {

	int i = 1, j = 1;
	for(; i < num_nodes-1; ++i) {
		for(j = i+1; j < num_nodes-1; ++j) {
			if(!Bottom_List_order(nodeArray[i], nodeArray[j], ENGLISH_ID))
				printf("English Nodes %i and  %i out of order...................\n", i, j);
//			if(!Bottom_List_order(nodeArray[i], nodeArray[j], HEBREW_ID))
//				printf("Hebrew Nodes %i and  %i out of order...................\n", i, j);
		}
	}
}
int main ( int argc, char *argv[] )
{
	int num_nodes = atoi(argv[1]);
	int i = 0, j;
	/*MAX_NUMBER_HALF = MAX_NUMBER >> 1;*/
	OM_Node ** nodeArray = malloc(num_nodes * sizeof(OM_Node *));
	Bottom_List * firstBottom_List =	(Bottom_List *)Bottom_List_init();
	firstBottom_List->id = 1;
	Top_List * list = init_top_list(); 

	for (;i < num_nodes; i++)
	{
		nodeArray[i]= malloc(sizeof(OM_Node));
		nodeArray[i]->id = i;
		
	}
	clock_t start = clock();
	

	append_first_list(list, firstBottom_List, ENGLISH_ID);

	Bottom_List_add_first_node(firstBottom_List, nodeArray[0], ENGLISH_ID);
	Bottom_List_add_first_node(firstBottom_List, nodeArray[0], HEBREW_ID);


	i = 1;
	for (; i < num_nodes; i++)
	{
		j = (rand() % i); 
		if (Bottom_List_insert(nodeArray[j ], nodeArray[i], ENGLISH_ID))
		 {
			 /*printf("TEST: splitting and adding to top at test index %i\n", i);*/

		 	Split_and_add_to_top(list, nodeArray[i]->ds_e);
			Rebalance_bottom_lists(list);
		 }
		if (Bottom_List_insert(nodeArray[j], nodeArray[i], HEBREW_ID))
		{
			/*printf("TEST: splitting and adding to top at test index %i\n", i);*/

			Split_and_add_to_top(list, nodeArray[i]->ds_h);
			Rebalance_bottom_lists(list);
		
		}
		/*printf("Check for correctness..");*/
		/*check_correctness(list);*/
		/*check_sub_correctness(list);*/
		/*printf("..done %d\n", i);*/

	}

	printf("Took %f ms.\n", ((double)clock() - start ) / CLOCKS_PER_SEC );
	
	/*order_test(nodeArray, num_nodes);*/
	check_correctness(list);
	check_sub_correctness(list);
	/*print_top_list(list);*/
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */


