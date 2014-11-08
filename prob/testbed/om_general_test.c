/*
 * =====================================================================================
 *
 *       Filename:  om_parallel_test
 *      
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
#include    "util/omOptions.h"

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

//extern int getOptions(int, char**, BenchOptions*);

int main ( int argc, char *argv[] )
{
	OmOptions opt;
	/// User specifies the number of nodes to insert
	int num_nodes;
	int i = 1, j, split_flag = 0;
	clock_t start;
	OM_Node ** nodeArray; 
	int * intArray;

	opt.inserts = 0;
	opt.rand = 0;
	opt.beg = 0;
	opt.end = 0;
	opt.printSplits = 0;

	/// Create the list
 	Top_List * list = create_tl();

	getOptions(argc,argv,&opt);

	num_nodes = opt.inserts;

	nodeArray = malloc(num_nodes * sizeof(OM_Node *));
	intArray = malloc( num_nodes * sizeof(int));

 	nodeArray[0]= malloc(sizeof(OM_Node));
	nodeArray[0]->ID = 0;

	if (opt.rand)
	{
		srand(5);
		/// Assign memory to all the nodes to be inserted
		for (;i < num_nodes; i++)
		{
			intArray[i] = (rand() % i);
			nodeArray[i]= malloc(sizeof(OM_Node));
			nodeArray[i]->ID = i;
		}
	} else if (opt.end) {
		/// Assign memory to all the nodes to be inserted
		for (;i < num_nodes; i++)
		{
			intArray[i] = i-1;
			nodeArray[i]= malloc(sizeof(OM_Node));
			nodeArray[i]->ID = i;
		}
	} else if (opt.beg) {
		/// Assign memory to all the nodes to be inserted
		for (;i < num_nodes; i++)
		{
			intArray[i] = 0;
			nodeArray[i]= malloc(sizeof(OM_Node));
			nodeArray[i]->ID = i;
		}
	} else {
		/// Assign memory to all the nodes to be inserted
		for (;i < num_nodes; i++)
		{
			intArray[i] = (rand() % i);
			nodeArray[i]= malloc(sizeof(OM_Node));
			nodeArray[i]->ID = i;
		}
	}
		

	/// Clock to record how long the inserts take
	start = clock();

	// Create and add the first relevant thing to the ds
	first_insert(list, nodeArray[0]);

	i = 1;
	for (; i < num_nodes; i++)
	{

		insert(nodeArray[intArray[i]], nodeArray[i]);

#ifdef RD_DEBUG
		printf("Check for correctness..");
		check_sub_correctness(list);
		printf("..done %d\n", i);
#endif

	}
	
	// End clock
	printf("Took %f s.\n", ((double)clock() - start ) / CLOCKS_PER_SEC );
	
	/*order_test(nodeArray, num_nodes);*/
	/*check_sub_correctness(list);*/

	if( opt.printSplits){
		print_split_count();
		print_rebalance_count();
	}

	i = 0;

#ifdef RD_STATS
	int num_splits = 0, list_count = 1;
	int num_rebalances = 0, before_split_size_total = 0, split_size_total = 0, rebalance_size_total = 0;
	int expected_splits = 0, expected_rebalances = 0, expected_split_size_total = 0, size = 0, count = 0;

	Bottom_List * current_bl = list->head;
	ll_node * current_ll_node;
	int list_num = 1;
//	printf("Size of bottom lists when split:\n");

	if( opt.printSplits ) {
		printf("\nThe Top_List and sizes of it's bottom_list's nodes:\n");
	}
	while (current_bl != NULL) {
		if( opt.printSplits ) {
			printf("List %i has %i nodes.\n", list_num++, current_bl->size);
		}
		size += current_bl->size;
		current_bl = current_bl->next;
	}
	if( opt.printSplits ) {
		printf("\n\n");
	}

	// Reset current_bl
	current_bl = list->head;
	/// Calc size of bottom lists when splits occurred
	while (current_bl != NULL){
		
//		printf("\tBottom List # %i : ", list_count++);
		current_ll_node = current_bl->list_of_size_of_bottom_list_when_split_head;
		while (	current_ll_node != NULL){
			num_splits++;
//			printf("%i; ", current_ll_node->data);
			
			/// Add up the size of the splits **********************
			if( opt.printSplits ) {
				printf("Size before split of %p: %i \t\t", current_ll_node, current_ll_node->size_before_split);
				printf("Data for %p: %i\n", current_ll_node, current_ll_node->data);
			}
			before_split_size_total += current_ll_node->size_before_split;
			split_size_total += current_ll_node->data;
			current_ll_node = current_ll_node->next;
		}

//		printf ( "\n" );
		current_bl = current_bl->next;
	}

//	printf("Total number of bottom list splits: %i\n\n", num_splits);

	list_count = 0;
	current_ll_node = list->list_of_size_of_top_list_when_split_head;
//	printf("Size of top list when rebalanced: \n");

	/// Calc size of bottom lists when splits occurred
	while (current_ll_node != NULL){
//		printf("%i\n", current_ll_node->data);
		list_count++;

		/// Add up the size of the rebalances ********************
		rebalance_size_total += current_ll_node->data;

		current_ll_node = current_ll_node->next;
	}
//	printf ( "\n" );

//	printf ( "Total number of top list rebalances: %i \n\n",  list_count);


	/* =========================================================================
	========================== UNIT TESTS FOR SPLITS ===========================
	========================================================================= */
	/// Only for the end-insert case
	if ( opt.end )
	{
		// Splits happen starting at the 66th insert every 64 inserts
		expected_splits = ((int)(num_nodes-2)/64) * 3;

		// Check expected number of splits
		if ( expected_splits != num_splits)
			printf("\n\n\n INCORRECT NUMBER OF SPLITS - ERROR IN CODE\n Expected Splits: %i \t Number of Splits: %i\n\n", expected_splits, num_splits);

		while ( count < num_nodes - 64) {
			expected_split_size_total += 65;
			count += 64;
		}

		if ( expected_split_size_total != split_size_total || expected_split_size_total != before_split_size_total)
			printf("\n\n\n INCORRECT TOTAL SPLIT COUNT - ERROR IN CODE\n Expected total: %i \t Split Total: %i \n\n", expected_split_size_total, split_size_total);

	}

	// Check Node count
	if (size != num_nodes)
		printf("\n\n\n ERROR - Nodes Counted in Bottom Lists does not match Number of Nodes passed in.\n Counted %i and passed %i.\n\n", size, num_nodes);


	

#endif

	/// Free all the allocated memory
	free_tl(list);
	free(intArray);
	free(nodeArray);
	return EXIT_SUCCESS;
}


