/*
 * =====================================================================================
 *
 *       Filename:  Bottom_List_mixed_test.c
 *
 *    Description:  Tests if the Top_List and Bottom_List implementation of Bender's
 *                  order-maintenance data structure is working correctly.
 *
 *        Version:  1.0
 *        Created:  07/07/2014 03:26:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (aj), atjalex@gmail.com; Shane Deiley, shanedeiley@wustl.edu
 *        Company:  Washington University in St. Louis Summer REU 2014
 *
 * =====================================================================================
 */

#include	"order-maintenance-general.h"
#include	<stdlib.h>
#include	<stdio.h>
#include	<time.h>

/// Tests if each node is in order in the back-insert case
void order_test (OM_Node ** nodeArray, int num_nodes)
{
	int i = 1, j = 1;
	for(; i < num_nodes-1; ++i)
	{
		for(j = i+1; j < num_nodes-1; ++j)
		{
			if( !order(nodeArray[i], nodeArray[j]) )
				printf("Nodes %i and  %i out of order.\n", i, j);
		}
	}
}


int main ( int argc, char *argv[] )
{
	/// User specifies the number of nodes to insert
	int num_nodes = atoi(argv[1]);
	int i = 0, j;

	OM_Node ** nodeArray = malloc(num_nodes * sizeof(OM_Node *));
	
	/// Create the lists
	Bottom_List * firstBottom_List = create_bl();
 	Top_List * list = create_tl(); 

	/// Assign memory to all the nodes to be inserted
	for (;i < num_nodes; i++)
	{
		nodeArray[i]= malloc(sizeof(OM_Node));
		nodeArray[i]->ID = i;
	}

	/// Clock to record how long the inserts take
	clock_t start = clock();

	// Create and add the first relevant thing to ds
	insert_tl(list, NULL, firstBottom_List);
	insert_bl(NULL, nodeArray[0], firstBottom_List);

	i = 1;
	for (; i < num_nodes; i++)
	{
		j = (rand() % i); ///< Case where each insert is random
		/*j = i -1;*/     ///< Case where each insert is at the back
		/*j = 0;*/        ///< Case where each insert is at the front
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
	
	/*order_test(nodeArray, num_nodes);*/
	check_sub_correctness(list);

	/// Free all the allocated memory
	free_tl(list);

	return EXIT_SUCCESS;
}


