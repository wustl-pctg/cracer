#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include "OM_DS.h"
#include "OM_DS_bender.h"
#include "OM_sublist.h"

void print_top_list(Top_List *list){

	OM_DS * current_e = list->head, *current_h = list->head;
	printf("English ID#%u{tag : %lu}->", current_e->id, current_e->tag_e);

	while(current_e != NULL)
	{
		printf("ID#%u{tag : %lu}->", current_e->id, current_e->tag_e);
		current_e = current_e->next_e;
	}
	printf("Tail\n");

	printf("Hebrew ID#%u{tag : %lu}->", current_h->id, current_h->tag_h);

	while(current_h != NULL)
	{
		printf("ID#%u{tag : %lu}->", current_h->id, current_h->tag_h);
		current_h = current_h->next_h;
	}
	printf("Tail\n");



}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  tag_range_relabel
 *  Description:  Relabels the range of nodes from x to y
 * =====================================================================================
 */
void tag_range_relabel (Top_List *list, OM_DS *x, OM_DS *y, const int ID )
{
	switch ( ID ) {
		case ENGLISH_ID:
		
			while (x != y && x != list->tail){
				/// insert x->next after x but with y->tag_e as the end tag
				insert_top_list(list, x, x->next_e, ENGLISH_ID, y->tag_e);

				/// Move along x pointer
				x = x->next_e;
			}
			break;

		case HEBREW_ID:	
			while (x != y && x != list->tail){
				/// insert x->next after x but with y->tag_e as the end tag
				insert_top_list(list, x, x->next_h, HEBREW_ID, y->tag_h);

				/// Move along x pointer
				x = x->next_h;
			}
	
			break;

		default:	
			break;
	}				/* -----  end switch  ----- */
	return ;
}		/* -----  end of function tag_range_relabel  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  top_list_rebalance(Top_List * list, OM_DS *pivot, const int ID)
 *  Description:  Rebalance list according to Bender's algorithm around pivot
 * =====================================================================================
 */
void top_list_rebalance(Top_List * list, OM_DS *pivot, const int ID)
{
	OM_DS *lList = pivot, *rList = pivot;
	float i, overflow_density, overflow_threshold;
	long enclosing_tag_range, num_elements_in_sublist = 1;

	switch ( ID ) {
		case ENGLISH_ID:	
			/// We assume l/rList are not NULL since the list will have at least 3 elements
			do		/// Check if range is in overflow
			{
				/// Move overflow list head and tail outward
				if (lList->prev_e){
					num_elements_in_sublist++;
					lList = lList->prev_e;
				}
				if (rList->next_e){
					num_elements_in_sublist++;
					rList = rList->next_e;
				}
				/// Calculate overflow_density
				enclosing_tag_range = rList->tag_e - lList->tag_e;

				i = log2f(enclosing_tag_range);
				overflow_threshold = pow(list->overflow_constant, -1 * i); 

				overflow_density = (num_elements_in_sublist/ (float)enclosing_tag_range ) ;
			}
			while ( enclosing_tag_range == 0 || overflow_density > overflow_threshold);


			/// rebalance subsection of top_list
			tag_range_relabel(list, lList, rList, ENGLISH_ID);
			
			break;

		case HEBREW_ID:	
			/// We assume l/rList are not NULL since the list will have at least 3 elements
			do		/// Check if range is in overflow
			{
				/// Move overflow list head and tail outward
				if (lList->prev_h){
					num_elements_in_sublist++;
					lList = lList->prev_h;
				}
				if (rList->next_h){
					num_elements_in_sublist++;
					rList = rList->next_h;
				}
				/// Calculate overflow_density
				enclosing_tag_range = rList->tag_h - lList->tag_h;

				i = log2f(enclosing_tag_range);
				overflow_threshold = pow(list->overflow_constant, -1 * i); 
				overflow_density = (num_elements_in_sublist/ (float)enclosing_tag_range ) ;
			}
			while (enclosing_tag_range == 0 || overflow_density > overflow_threshold);
			
			tag_range_relabel(list, lList, rList, HEBREW_ID);
			
			break;
		default:	
			break;
	}				/* -----  end switch  ----- */
	
	return ;
}		/* -----  end of function top_list_rebalance(Top_List * list, OM_DS *pivot, const int ID)  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  insert_top_list(Top_List * list, OM_DS * x, OM_DS *y)
 *  Description:  Insert x after y in the top list
 * =====================================================================================
 */
void insert_top_list(Top_List * list, OM_DS * x, OM_DS *y, const int ID, int RELABELING_END_TAG)
{
	///Debug: Double check that all vals are not null
	assert(y != NULL && x != NULL && list != NULL);

	switch ( ID ) {
		case ENGLISH_ID:
			if (RELABELING_END_TAG != 0)
			{
				y->tag_e = (RELABELING_END_TAG >> 1) + (x->tag_e >> 1);

				if (y->tag_e == x->tag_e || y->tag_e == RELABELING_END_TAG)
				{
					/// We have an issue, collision during rebalancing
					printf("We have an issue: collision during reabalance");
					exit(1); 
				}

			}
			else //We are inserting a new element into the list
			{
				y->tag_e = ((x->next_e->tag_e >> 1) + (x->tag_e >> 1));
			
				
				/// Reassign  prev/next pointers
				y->prev_e = x;
				y->next_e  = x->next_e;
				x->next_e = y;

				/// Update the next node after y's prev_e reference (change it to y)
				y->next_e->prev_e = y;
	
				if ((y->tag_e == x->tag_e) ||  ( y->tag_e == y->next_e->tag_e ))
					top_list_rebalance(list, x, ID);
		
			}
			break;
			
		case HEBREW_ID:		
			if (RELABELING_END_TAG != 0)
			{
				y->tag_h = (RELABELING_END_TAG >> 1) + (x->tag_h >> 1);

				if (y->tag_h == x->tag_h || y->tag_h == RELABELING_END_TAG)
				{
					/// We have an issue, collision during rebalancing
					printf("We have an issue: collision during reabalance");
					exit(1); 
				}

			}
			else //We are inserting a new element into the list
			{
				y->tag_h = ((x->next_h->tag_h >> 1) + (x->tag_h >> 1));
				y->tag_h = y->tag_h;

				/// Reassign  prev/next pointers
				y->prev_h = x;
				y->next_h  = x->next_h;
				x->next_h = y;

				/// Update the next node after y's prev_h reference (change it to y)
				y->next_h->prev_h = y;
	
				if ((y->tag_h == x->tag_h) ||  ( y->tag_h == y->next_h->tag_h ))
					top_list_rebalance(list, x, ID);
		
	
				/// IMPORTANT NOTE: size is only updated once since the list referes to the same nodes
				(list->size)++;
			}
			break;
			

		default:	
			break;
	}				/* -----  end switch  ----- */

	return ;

}		/* -----  end of function insert_top_list  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  append_first_list
 *  Description:  Appends the sublist passed into this function to the top list
 * =====================================================================================
 */
void append_first_list (Top_List * list, OM_DS * first_sub_list, const int ID){

	insert_top_list(list, list->head, first_sub_list, ID, 0);
}		/* -----  end of function append_first_list ---- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_top_list
 *  Description:  allocates memory and initializes all values for top list.
 *  			
 * =====================================================================================
 */
Top_List * init_top_list ()
{
	/// Malloc data for all necessary nodes and for the list itself
	Top_List * list = malloc(sizeof(*list));
	list->head = malloc(sizeof(* (list->head)));
	list->tail = malloc(sizeof(* (list->tail)));

	/// Assign appropriate vals to list
	list->size  				= 	2;

	/// Assign T (in bender's paper), which governs how dense the list can be 
	/// before rebalancing. As of now, we just pick an arbitrary val in [1,2]
	list->overflow_constant 	=	1.2; 

	/// Assign appropriate vals to head and tail node tags
	list->head->tag_e = list->head->tag_h = 0;
	list->tail->tag_e = list->tail->tag_h = ~0;



	/// Assign correct vals to head and tail pointers
	list->head->prev_h = list->head->prev_e = list->tail->next_h = list->tail->next_e = NULL;

	/// Prev node fro tail is head
	list->tail->prev_e = list->tail->prev_h = list->head;

	/// Next node for head is tail
	list->head->next_e = list->head->next_h = list->tail;

	return list;
}		/* -----  end of function init_top_list  ----- */

void Top_List_free_and_free_nodes ( Top_List * list )
{
	/// Keep track pf current and next node
	OM_DS * current = list->head, *next = NULL;

	do {
		/// Assign next pointer
		next = current->next_e;

		/// Free current sublist
		// TODO: free_and_free_nodes(current);
		//printf("Debug: Free nodes currently does not call the OM_DS routine\n");
		free(current);

		/// Move current along
		current = next;
	}
	while (next != NULL);

	/// Free this list
	free(list);
	return ;
}		/* -----  end of function Free_and_free_nodes  ----- */
int main ( int argc, char *argv[] )
{
	Top_List *list =  init_top_list();
	int num_inserts = atoi(argv[1]), i = 0;

	OM_DS ** arrayToInsert = malloc(sizeof(OM_DS * ) * num_inserts);

	for (i = 0; i < num_inserts; i++ )
	{
		arrayToInsert[i] = (OM_DS *)malloc(sizeof(OM_DS));
		arrayToInsert[i]->id = i;

	}

	clock_t start = clock();

	append_first_list(list, arrayToInsert[0], ENGLISH_ID);
	append_first_list(list, arrayToInsert[0], HEBREW_ID);
	//print_top_list(list);

	for (i = 1; i < num_inserts; i++)
	{
		insert_top_list(list, list->head , arrayToInsert[i ], ENGLISH_ID, 0);
		insert_top_list(list, list->head, arrayToInsert[i], HEBREW_ID, 0);
		//print_top_list(list);
	}


	printf("Took %f ms.\n", ((double)clock() - start ) / CLOCKS_PER_SEC );
	
	Top_List_free_and_free_nodes(list);
	free(arrayToInsert);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
