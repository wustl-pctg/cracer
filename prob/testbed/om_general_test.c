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
 *         Author:  Alex Jones (aj), atjalex@gmail.com, and Shane
 *        Company:  
 *
 * =====================================================================================
 */

#include	"order-maintenance-general.h"
#include	<stdlib.h>
#include	<stdio.h>
#include	<time.h>


void order_test(OM_Node ** nodeArray, int num_nodes) {

	int i = 1, j = 1;
	for(; i < num_nodes-1; ++i) {
		for(j = i+1; j < num_nodes-1; ++j) {
			if(!order(nodeArray[i], nodeArray[j]))
				printf("Nodes %i and  %i out of order.\n", i, j);
		}
	}
}
int main ( int argc, char *argv[] )
{
	int num_nodes = atoi(argv[1]);
	int i = 0, j;

	OM_Node ** nodeArray = malloc(num_nodes * sizeof(OM_Node *));
	Bottom_List * firstBottom_List = create_bl();
 	Top_List * list = create_tl(); 

	for (;i < num_nodes; i++)
	{
		nodeArray[i]= malloc(sizeof(OM_Node));
		nodeArray[i]->ID = i;
	}

	clock_t start = clock();

	// Create and add the first relevant thing to ds
	insert_tl(list, NULL, firstBottom_List);
	insert_bl(NULL, nodeArray[0], firstBottom_List);

	i = 1;
	for (; i < num_nodes; i++)
	{
		/*j = (rand() % i);*/
		j = i -1;
		if (insert_bl(nodeArray[j], nodeArray[i], nodeArray[j]->ds))
		 {
			 /*printf("TEST: splitting and adding to top at test index %i\n", i);*/

			rebalance_bls(list);
		 }
#ifdef RD_DEBUG
		printf("Check for correctness..");
		check_sub_correctness(list);
		printf("..done %d\n", i);
#endif

	}

	// End clock
	printf("Took %f ms.\n", ((double)clock() - start ) / CLOCKS_PER_SEC );
	
	order_test(nodeArray, num_nodes);
	check_sub_correctness(list);

	return EXIT_SUCCESS;
}


