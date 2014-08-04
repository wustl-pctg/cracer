/*
 * =====================================================================================
 *
 *       Filename:  om_general_test.c
 *
 *    Description:  Tests if the Top_List and Bottom_List implementation of Bender's
 *                  order-maintenance data structure is working correctly. 
 *                  Adds parallel top list rebalances
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

#include	"order-maintenance-par-rebalance.h"
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

	/// Clock to record how long the inserts take
	clock_t start = clock();

	// Create and add the first relevant thing to the ds
	first_insert(list, nodeArray[0]);

	i = 1;
	for (; i < num_nodes; i++)
	{
//			if(list->split_flag == 0) {
//			if(i % 2 == 0)
				/*j = (rand() % i); ///< Case where each insert is random*/
//			else
//				j = i - 1;     ///< Case where each insert is at the back
//			j = 0;        ///< Case where each insert is at the front
//		}
//		else if (list->split_flag == 1) {
			/*j =i - 1;*/
//		}
//		j = 0;
		j = i-1;
//		j = (rand() % i);
		insert(nodeArray[j], nodeArray[i]);

#ifdef RD_DEBUG
		printf("Check for correctness of Bottom_lists..");
		check_sub_correctness(list);
		printf("..done %d\n", i-1);
#endif

	}
	
	// End clock
	printf("Took %f s.\n", ((double)clock() - start ) / CLOCKS_PER_SEC );
	
	/*order_test(nodeArray, num_nodes);*/
	check_sub_correctness(list);

#ifdef RD_STATS
	int num_splits = 0, list_count = 1;
	int num_rebalances = 0;

	Bottom_List * current_bl = list->head;
	ll_node * current_ll_node;
//	printf("Size of bottom lists when split:\n");
	/// Calc size of bottom lists when splits occurred
	while (current_bl != NULL){
//		printf("\tBottom List # %i : ", list_count++);
		current_ll_node = current_bl->list_of_size_of_bottom_list_when_split_head;
		while (	current_ll_node != NULL){
			num_splits++;
//			printf("%i; ", current_ll_node->data);
			current_ll_node = current_ll_node->next;
		}

//		printf ( "\n" );
		current_bl = current_bl->next;
	}

	printf("Total number of bottom list splits: %i\n\n", num_splits);

	list_count = 0;
	current_ll_node = list->list_of_size_of_top_list_when_split_head;
//	printf("Size of top list when split: ");

	/// Calc size of bottom lists when splits occurred
	while (current_ll_node != NULL){
		//printf("%i; ", current_ll_node->data);
		list_count++;
		current_ll_node = current_ll_node->next;
	}
//	printf ( "\n" );

	printf ( "Total number of top list rebalances: %i \n\n",  list_count);
#endif

	/// Free all the allocated memory
	free_tl(list);

	return EXIT_SUCCESS;
}


