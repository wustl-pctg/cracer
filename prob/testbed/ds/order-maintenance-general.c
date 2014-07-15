/*
 * =====================================================================================
 *
 *       Filename:  order-maintenance-general.c
 *
 *    Description:  Combined Top_List and Bottom_List for order maintenance
 *
 *        Version:  1.0
 *        Created:  07/15/2014 11:32:30 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (aj), atjalex@gmail.com, Shane Deiley shanedeiley@wustl.edu
 *        Company: 
 *
 * =====================================================================================
 */
#define IS_SOURCE_FILE
#include "order-maintenance-general.h"

static unsigned long MAX_NUMBER = ~0;
static int INT_BIT_SIZE = 64;
static double OVERFLOW_CONSTANT = 1.5;



/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  create_bl
 *  Description:  Creates a new list and allocates memory for it
 * =====================================================================================
 */
Bottom_List * create_bl(){

	/// Pointer to new list
	Bottom_List * list;

	/// Allocate memory for list
	list = (Bottom_List*)malloc(sizeof(Bottom_List));
	list->head = NULL; 
	list->tail = NULL; 

	/// Reset size and reorder flag
	list->size = list->reorder_flag = 0;

	return list;
}

/*!
 * ===  FUNCTION  ======================================================================
 *         Name:  create_tl
 *  Description:  Allocate memory and initialize top list
 * =====================================================================================
 */
Top_List * create_tl(){
	/// Allocate memory for top list
	Top_List * list = malloc(sizeof(*list));

	/// Assign appropriate vals to list
	list->size  				= 	0;

	/// Assign correct vals to head and tail pointers
	list->head = list->tail = NULL;

	return list;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  insert_bl
 *  Description:  Insert node y after node x in the Bottom List specified in x.
 *  How to Call:  Always pass in ds, if this is the first call, then x is null.
 * =====================================================================================
 */
int insert_bl(OM_Node * x, OM_Node *y, Bottom_List * ds){

#ifdef RD_DEBUG
		//if x is null
		if (!(y && ds) ){
			printf("Some node or ds is null, skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n", x->ID, y->ID);
			assert(0);
		}
#endif
		/// Update the ds y is in 
		y->ds = ds;

		/// This is the first insert called on the ds
		if (ds->size == 0){
			ds->head = ds->tail = y;
			y->next = y->prev = NULL;
			ds->size = 1;
			return 0;
		}

		/// Reassign prev/next pointers
		y->prev = x;
		y->next = x->next;
		x->next = y;
		if ( y->next != NULL) y->next->prev = y;

		/// Assign value to y->tag
		if (x == ds->tail)
		{
			y->tag = (x->tag >> 1) + (MAX_NUMBER >> 1);

			/// Correct for adding two odd numbers (MAX_NUMBER is always odd)
			if (x->tag & 0x1 == 0x1) y->tag++;

			ds->tail = y;
		}
		else
		{
			y->tag = (x->tag >> 1) + (x->next->tag >> 1);

			/// Correct for adding two odd numbers (MAX_NUMBER is always odd)
			if (x->next->tag & x->tag & 0x1 == 0x1) y->tag++;
		}

		ds->size += 1;
		if (ds->size > INT_BIT_SIZE >> 1)
			ds->reorder_flag = 1;

		if(ds->size == INT_BIT_SIZE) 
		    return 1; ///< Needs to be split
		else
			return 0; ///< Doesn't needs immediately split
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  insert_tl
 *  Description:  Insert list y after list x in top list
 * =====================================================================================
 */
void insert_tl(Top_List * list, Bottom_List *x, Bottom_List *y)
{
	#ifdef RD_DEBUG
		assert((list->size == 0 || x != NULL) && y != NULL && list != NULL);
	#endif

		/// Insert the first element y into the list
		if (list->size == 0){
			/// Assign head and tail of list
			list->head = list->tail = y;	
		
			/// Put y in the middle of the tag range
			/// NOTE: This might want to put as MAX_NUMBER >> 1 if this doesnt work out
			y->tag = 0;

			y->reorder_flag = 0;
			y->next = y->prev = NULL;

			(list->size)++;
		}
		/// If there exist other elements in the top list
		else {
				/// Assign tags
			if (x == list->tail)
			{
				y->tag = (x->tag >> 1) + (MAX_NUMBER >> 1);

				/// Correct for adding two odd numbers (MAX_NUMBER is always odd)
				if (x->tag & 0x1 == 0x1) y->tag++;

				list->tail = y;
			}
			else
			{
				y->tag = (x->tag >> 1) + (x->next->tag >> 1);

				/// Correct for adding two odd numbers (MAX_NUMBER is always odd)
				if (x->next->tag & x->tag & 0x1 == 0x1) y->tag++;
			}
			
			/// See if there is a "collision" of tags
			if (x->next->tag - x->tag <= 1)
				{
				/// Thin out the list - make room for y
				rebalance_tl(list, x);

				/// Dont assign pointers, call insert again to put y after x
				insert_tl(list, x, y);
			}
			else{
				/// Note this is not atomic

				/// Reassign prev/next pointers
				y->prev = x;
				y->next = x->next;
				x->next = y;
				if ( y->next != NULL) y->next->prev = y;
				
				(list->size)++;
			}
		}

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  order
 *  Description:  Determines the total ordering (that is top and bottom ordering) of nodes x and y.
 *  			  Returns 1 if x  <  y
 *  			  Returns 0 if x  >= y
 *  			  
 * =====================================================================================
 */
int order(OM_Node * x, OM_Node * y)
{
	if (x->ds == y->ds ) {
		if (x->tag < y->tag)
			return 1;
		else 
			return 0;
	}
	else {
		if (x->ds->tag < y->ds->tag)
			return 1;
		else
			return 0;
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  split_bl
 *  Description:  Splits the bottom list into two bottom lists. Then it inserts the second
 *  			  after the first in the top list.
 * =====================================================================================
 */
void split_bl(Top_List * list, Bottom_List * list_to_split){
	OM_Node * current = list_to_split->head, *middle_node;
	
	/// Create new list to add to top 
	Bottom_List * to_add = create_bl();
	
	/// Keep track of num  nodes visited
	int node_count = 1;

	/// Each node in the list will be spaced out by skip_size tag spaces
	unsigned long skip_size = MAX_NUMBER / ((list_to_split->size >> 1) + 2);

	while (node_count < (list_to_split->size >> 1)){
		/// Increment the tag of the next node
		current->next->tag = current->tag + skip_size;
		/// Move node along
		current = current->next;	
		/// Increase node count	
		node_count++;
	}
	/// Update sizes of the lists
	to_add->size = list_to_split->size - node_count;
	list_to_split->size = node_count;
	
	/// Make current the end of the original list
	list_to_split->tail = current;
	/// Save the middle (the beginning of the second list)
	middle_node = current->next;
	current->next = NULL;


	/// Get current node back;
	current = middle_node;

	/// Set the head of the new list to middle node
	to_add->head = middle_node;

	/// Nullify prev of the middle node
	middle_node->prev = NULL;

	current->tag = 0;
	while (current != NULL){
		/// Reassign current ds
		current->ds = to_add;

		/// Move node along
		current = current->next;	

		/// Update tag
		current->tag = current->prev->tag + skip_size;
	}

	list_to_split->reorder_flag = to_add->reorder_flag = 0;

	/// Insert the newly created list this into the top list
	insert_tl(list, list_to_split, to_add);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rebalance_tl
 *  Description:  Rebalances the top list around a pivot node according to bender's 
 *  			  algorithm.
 * =====================================================================================
 */
void rebalance_tl(Top_List * list, Bottom_List * pivot){
	/// NOTE: Improve comments	
	Bottom_List *lList = pivot, *rList = pivot;
	double overflow_density, overflow_threshold;
	unsigned long enclosing_tag_range, lTag, rTag, num_elements_in_sublist = 2, skip_size;
	double i = -1;

	do	/// Check if range is in overflow
	{
		/// Move overflow list head and tail outward
		if (lList->prev){
			num_elements_in_sublist++;
			lList = lList->prev;
			lTag = lList->tag;
		}
		if (rList->next){
			num_elements_in_sublist++;
			rList = rList->next;
			rTag = rList->tag;
		}
		else {
			rTag = MAX_NUMBER;	
		}

		/// Calculate overflow_density
		enclosing_tag_range = rTag - lTag;

		i = ceil( log2((double)enclosing_tag_range) );
		overflow_threshold = pow(OVERFLOW_CONSTANT, -1.0 * i); 

		overflow_density = (num_elements_in_sublist) / (double)enclosing_tag_range ;
	}
	while ( (enclosing_tag_range == 0 || overflow_density > overflow_threshold ) && enclosing_tag_range != MAX_NUMBER);

	/// This is the spacing in between tags of Bottom_Lists in between lList and rList
	skip_size = (unsigned long)((enclosing_tag_range ) / (num_elements_in_sublist + 1) );
	#ifdef RD_DEBUG
	/// NOTE: this may fail and still be correct when rList is the list->tail and our skip size is balancing our sublist all the
	/// way to the end of the possible tag range.
		assert(skip_size>0 && ((skip_size * (num_elements_in_sublist - 1)) + lList->tag <= rList->tag ));
	#endif
	/// Relabel the tag range
	relabel_tl_tag_range(lList, rList, skip_size);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  relabel_tl_tag_range
 *  Description:  Relabels the range of tags from list start to end using a distance of skip_size
 * =====================================================================================
 */
void relabel_tl_tag_range(Bottom_List *start, Bottom_List *end, const long skip_size){
	Bottom_List * current = start;
	current->tag = start->tag;
	
	do {
		current->next->tag = current->tag + skip_size;
		current = current->next;
	}
	while (current != end);
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  rebalance_bls
 *  Description:  Rebalance all bottom lists with the reorder flag
 * =====================================================================================
 */
void rebalance_bls(Top_List * list){
	/// The Iterators
	Bottom_List * current, * current_h;
	current = list->head;

	while(current != NULL) {
		if(current->reorder_flag == 1) ///< If 1, then needs split
			split_bl(list, current);
		current = current->next;
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  print_tl
 *  Description:  Print all the ID's of the "nodes" in top list
 * =====================================================================================
 */
void print_tl(Top_List * list){
	Bottom_List * current = list->head;

	printf ( "(size = %i) Head->", list->size );
	while (current != NULL)

	{
		printf ( "(%lu)->", current->tag);
		current = current->next;
	}

	printf ( "Tail\n" );
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  print_bl
 *  Description:  Print all the ID's of the nodes in bottom list
 * =====================================================================================
 */
void print_bl(Bottom_List * list){
	OM_Node * current = list->head;

	printf ( "(size = %i) Head->", list->size );
	while (current != NULL)

	{
		printf ( "(%i | %lu)->", current->ID, current->tag);
		current = current->next;
	}

}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  free_bl
 *  Description:  Free all nodes in bottom list and the list itself
 * =====================================================================================
 */
void free_bl (Bottom_List * list){
	OM_Node *next, * current = list->head;

	while (current != NULL){
		next = current->next;	
		free(current);
		current = next;
		}


	free(list);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  free_tl
 *  Description:  Free all bottom lists in top lists and the list itself
 * =====================================================================================
 */
void free_tl ( Top_List * list )
{
	Bottom_List *next, * current = list->head;

	while (current != NULL){
		next = current->next;	
		free(current);
		current = next;
		}


	free(list);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  check_sub_correctness
 *  Description:  Checks the list so each node's tag is less than the next node
 * =====================================================================================
 */
void check_sub_correctness (Top_List * list){
	Bottom_List * current = list->head;
	OM_Node * cur_node;

	while (current != NULL){

		cur_node = current->head;
		while(cur_node != current->tail) {
			
			if (!(cur_node->tag < cur_node->next->tag))
				printf("Node tags are out of order\n");
			cur_node = cur_node->next;
		}
		current = current->next;
	}
}

