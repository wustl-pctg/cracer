#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
typedef struct Sub_List_s{
	struct Sub_List_s *enext,*hnext, *eprev, *hprev;
	unsigned int tag;

}Sub_List;

/// Implementation of Bender's order maintenance data structure.
/// This is the top tier of the list
typedef struct Top_List_s{

	Sub_List *head, *tail; /// TODO: change to OM_DS of the sublist
	const int ID; /// English or Hebrew: do i need this?
	int size;
	float overflow_threshold; /// T in the bender paper
	/// TODO: precompute values of 1 / (T^i)  for much needed values of i
} Top_List;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  tag_range_relabel
 *  Description:  Relabels the range of nodes from x to y
 * =====================================================================================
 */
	void
tag_range_relabel (Top_List *list, Sub_List *x, Sub_list *y, const int ID )
{
	switch ( ID ) {
		case ENGLISH_ID:
			while (x != y){
				/// insert x->next after x but with y->etag as the end tag
			insert_top_list(list, x, x->enext, ENGLISH_ID, y->etag);

			/// Move along x pointer
			x = x->enext;
			}
			break;

		case HEBREW_ID:	
			while (x != y){
				/// insert x->next after x but with y->etag as the end tag
			insert_top_list(list, x, x->hnext, HEBREW_ID, y->htag);

			/// Move along x pointer
			x = x->hnext;
			}
	
			break;

		default:	
			break;
	}				/* -----  end switch  ----- */
	return ;
}		/* -----  end of function tag_range_relabel  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  top_list_rebalance(Top_List * list, Sub_List *pivot, const int ID)
 *  Description:  Rebalance list according to Bender's algorithm around pivot
 * =====================================================================================
 */
void
top_list_rebalance(Top_List * list, Sub_List *pivot, const int ID)
{
	Sub_List *lList, *rList;
	float i, overflow_density;
	switch ( ID ) {
		case ENGLISH_ID:	
			/// Assign initial values to left and right nodes in list (head and tail of sublist)
			lList = pivot->eprev;
			rList = pivot->enext;
			i = log2f(rList - lList);
			overflow_density = pow(list->overflow_threshold, -1 * i); 

			/// We assume l/rList are not NULL since the list will have at least 3 elements
			while ((lList != list->head ) || (rList != list->tail) ||
					/// Check if range is in overflow
					(overflow_density > overflow_threshold))
			{
				/// Move overflow list head and tail outwards
				lList = pivot->eprev;
				rList = pivot->enext;

				/// Calculate overflow_density
				i = log2f(rList - lList);
				overflow_density = pow(list->overflow_threshold, -1 * i); 
			}
			/// rebalance subsection of top_list
			tag_range_relabel(list, lList, rList, ENGLISH_ID);
			
			break;

		case HEBREW_ID:	
			/// Assign initial values to left and right nodes in list (head and tail of sublist)
			lList = pivot->hprev;
			rList = pivot->hnext;
			i = log2f(rList - lList);
			overflow_density = pow(list->overflow_threshold, -1 * i); 

			/// We assume l/rList are not NULL since the list will have at least 3 elements
			while ((lList != list->head ) || (rList != list->tail) ||
					/// Check if range is in overflow
					(overflow_density > overflow_threshold))
			{
				/// Move overflow list head and tail outwards
				lList = pivot->hprev;
				rList = pivot->hnext;

				/// Calculate overflow_density
				i = log2f(rList - lList);
				overflow_density = pow(list->overflow_threshold, -1 * i); 
			}
			/// rebalance subsection of top_list
			tag_range_relabel(list, lList, rList, HEBREW_ID);
			
			break;
		default:	
			break;
	}				/* -----  end switch  ----- */
	
	return ;
}		/* -----  end of function top_list_rebalance(Top_List * list, Sub_List *pivot, const int ID)  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  insert_top_list(Top_List * list, Sub_List * x, Sub_List *y)
 *  Description:  Insert x after y in the top list
 * =====================================================================================
 */
void insert_top_list(Top_List * list, Sub_List * x, Sub_List *y, const int ID, unsigned int end_tag)
{
	///Debug: Double check that all vals are not null
	assert(y != NULL && x != NULL && list != NULL);


	switch ( ID ) {
		case ENGLISH_ID:	
			/// Check if new tag overlaps with x->enext

			if (x->enext->etag  - x->etag == 1) {
				top_list_rebalance(list, x, ID);
			}

			/// Assign new tag, list will be balanced 
			y->etag = (end_tag >> 1) + (x->etag >> 1);
				
			/// Reassign  prev/next pointers
			/// TODO : (fix) ignoring need for atomic inserts as of
			y->eprev = x;
			y->enext  = x->enext;
			x->enext = y;

			break;

		case HEBREW_ID:		
			/// Check if new tag overlaps with x->hnext

			if (x->enext->htag  - x->htag == 1) {
				top_list_rebalance(list, x, ID);
			}
			
			/// Assign new tag, list will be balanced 
			y->etag = (end_tag >> 1) + (x->etag >> 1);
				
			/// Reassign  prev/next pointers
			/// TODO : (fix) ignoring need for atomic inserts as of
			y->hprev = x;
			y->hnext  = x->hnext;
			x->hnext = y;


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
void append_first_list (Top_List * list, Sub_List * first_sub_list, const int ID){

	insert_top_list(list, list->head, first_sub_list, ID, list->tail->etag);
}		/* -----  end of function append_first_list ---- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_top_list
 *  Description:  allocates memory and initializes all values for top list.
 *  			
 * =====================================================================================
 */
Top_List *
init_top_list ()
{
	/// Malloc data for all necessary nodes and for the list itself
	Top_List * list = malloc(sizeof(*list));
	list->head = malloc(sizeof(* (list->head)));
	list->tail = malloc(sizeof(* (list->tail)));

	/// Assign appropriate vals to list
	list->size  				= 	2;

	/// Assign T (in bender's paper), which governs how dense the list can be 
	/// before rebalancing. As of now, we just pick an arbitrary val in [1,2]
	list->overflow_threshold 	=	1.30; 

	/// Assign appropriate vals to head and tail node tags
	list->head->etag = list->head->htag = 0;
	list->tail->etag = list->tail->htag = MY_INT_MAX;

	/// Max unsigned int divided by 2
	list->tail_tag_div_by_2		=	list->tail->tag >> 1;


	/// Assign correct vals to head and tail pointers
	list->head->hprev = list->head->eprev = list->tail->hnext = list->tail->enext = NULL;

	/// Prev node fro tail is head
	list->tail->eprev = list->tail->hprev = list->head;

	/// Next node for head is tail
	list->head->enext = list->head->hnext = list->tail;

	return list;
}		/* -----  end of function init_top_list  ----- */

	void
Free_and_free_nodes ( Top_List * list )
{
	Sub_List * current = list->head, *next = NULL;

	do {
		next = current->enext;
		free(current);
	}
	while (next != NULL);

	free(list);
	return ;
}		/* -----  end of function Free_and_free_nodes  ----- */
int main ( int argc, char *argv[] )
{
	Top_List *list =  init_top_list();


	Free_and_free_nodes(elist);
	Free_and_free_nodes(hlist);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
