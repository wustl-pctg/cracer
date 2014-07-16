/*
 * =====================================================================================
 *
 *       Filename:  order-maintenance-general.c
 *
 *    Description:  Source file for Bender's OM-Data Structure implemented with a LL top
 *                  and a LL bottom
 *
 *        Version:  1.0
 *        Created:  07/15/2014 11:36:19 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (aj), atjalex@gmail.com; Shane Deiley, shanedeiley@wustl.edu
 *        Company:  Washington University in St. Louis Summer REU 2014
 *
 * =====================================================================================
 */

#include "order-maintenance-general.h"

/// Constants used within this source file
static unsigned long MAX_NUMBER = ~0;
static int INT_BIT_SIZE = 64;
static double OVERFLOW_CONSTANT = 1.85;

/*!
 * ===  FUNCTION  ======================================================================
 *         Name:  create_bl
 *  Description:  Creates a new Bottom_List and allocates memory for it.
 * =====================================================================================
 */
Bottom_List * create_bl ()
{
	/// Allocate memory for top list
	Bottom_List * list = malloc(sizeof(*list));

	/// Assign correct vals to head and tail pointers
	list->head = list->tail = NULL; 

	/// Reset size and reorder flag
	list->size = list->reorder_flag = 0;

	return list;
}

/*!
 * ===  FUNCTION  ======================================================================
 *         Name:  create_tl
 *  Description:  Allocate memory and initialize Top_List.
 * =====================================================================================
 */
Top_List * create_tl ()
{
	/// Allocate memory for top list
	Top_List * list = malloc(sizeof(*list));

	/// Assign appropriate vals to list
	list->size = 0;

	/// Assign correct vals to head and tail pointers
	list->head = list->tail = NULL;

	return list;
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  first_insert_bl
 *  Description:  Insert the first OM_Node y into the Bottom_List ds
 * =====================================================================================
 */
void first_insert_bl(Bottom_List * ds, OM_Node * y)
{
#ifdef RD_DEBUG
	/// Make sure they're not NULL
	if ( !(ds && y) )
	{
		printf("First node or ds null: ds: %p y:(%d) %p\n", ds, y->ID, y);
		assert(0);
	}
	
	if (ds->size != 0)
	{
		printf("Size was not 0. Improper call to first_insert_bl\n");
		assert(0);
	}
#endif

	y->ds = ds;
	ds->head = ds->tail = y;
	y->next = y->prev = NULL;
	ds->size = 1;
	y->tag = 0;
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  insert_bl
 *  Description:  Insert node y after node x in the Bottom_List specified in x->ds.
 *  How to Call:  Pass in ds: if this is the first call, x is null and ds will be known.
 *                Otherwise, ds is x->ds.
 * =====================================================================================
 */
void insert_bl (OM_Node * x, OM_Node *y)
{
	/// Retrieve the Bottom_List
	Bottom_List * ds = x->ds;

#ifdef RD_DEBUG
	/// y and ds should never be NULL
	if ( !(x && y && ds) )
	{
		printf("Some node or ds is null, skipping insert; x(%d) and y(%d)\n", x->ID, y->ID);
		assert(0);
	}
#endif

	/// Update the ds y is in 
	y->ds = ds;

	if (x == ds->tail)
	{
		/// y's tage is the average of its neighbors
		y->tag = (x->tag >> 1) + (MAX_NUMBER >> 1);

		/// Check for collision
		if ((MAX_NUMBER - x->tag) <= 1) ///< If x is tail, use MAX_NUMBER instead of x->next->tag		
		{
			split_bl(ds->parent, ds);
			insert_bl(x, y);
			return;
		}

		/// Correct for adding two odd numbers (MAX_NUMBER is always odd)
		if (x->tag & 0x1 == 0x1) y->tag += 1;

		ds->tail = y;
	}
	else
	{
		/// y's tage is the average of its neighbors
		y->tag = (x->tag >> 1) + (x->next->tag >> 1);

		/// Check for collision
		if ((x->next->tag - x->tag) <= 1)
		{
			split_bl(ds->parent, ds);
			insert_bl(x, y);
			return;
		}

		/// Correct for adding two odd numbers (MAX_NUMBER is always odd)
		if (x->next->tag & x->tag & 0x1 == 0x1) y->tag += 1;
	}

	/// Reassign prev/next pointers
	y->prev = x;
	y->next = x->next;
	x->next = y;
	if (y->next != NULL) y->next->prev = y; ///< If y isn't tail, make the next node point to it

	ds->size += 1;


	/// Mark flag in each list greater than half it's capacity
//	if (ds->size > (INT_BIT_SIZE >> 1))
//		ds->reorder_flag = 1;

//	if (ds->size == INT_BIT_SIZE)
//		return 1; ///< Needs to be split
//	else
//		return 0; ///< Doesn't needs immediately split
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  first_insert_tl
 *  Description:  Insert list y after list x in Top_List list.
 * =====================================================================================
 */
void first_insert_tl(Top_List * list, Bottom_List * y)
{

#ifdef RD_DEBUG
	/// Make sure they're not NULL
	if ( !(list && y) )
	{
		printf("First node or ds null: ds: %p y: %p\n", list, y);
		assert(0);
	}
	
	if (list->size != 0)
	{
		printf("Size was not 0. Improper call to first_insert_bl\n");
		assert(0);
	}
#endif
	
	/// Update necessary parameters
	y->parent = list;
	list->head = list->tail = y;	
	y->tag = 0;

	y->reorder_flag = 0;
	y->next = y->prev = NULL;
	list->size += 1;
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  insert_tl
 *  Description:  Insert list y after list x in Top_List list.
 * =====================================================================================
 */
void insert_tl (Bottom_List *x, Bottom_List *y)
{
	/// Retrieve Top_List
	Top_List * list = x->parent;

#ifdef RD_DEBUG
	assert( (list->size == 0 || x != NULL) && (y != NULL) && (list != NULL) );
#endif
	
	if (x == list->tail)
	{
		/// y's tag is the average of the max and the prior tag
		y->tag = (x->tag >> 1) + (MAX_NUMBER >> 1);

		/// Correct for adding two odd numbers (MAX_NUMBER is always odd)
		if (x->tag & 0x1 == 0x1) y->tag++;
	}
	else
	{		
		/// y's tag is the average of the next and prior tags
		y->tag = (x->tag >> 1) + (x->next->tag >> 1);

		/// Correct for adding two odd numbers (MAX_NUMBER is always odd)
		if (x->next->tag & x->tag & 0x1 == 0x1) y->tag++;
	}
			
	/// See if there is a "collision" of tags
	if (    (x == list->tail && (MAX_NUMBER - x->tag <= 1)  ) || ///< If x is tail, use MAX_NUMBER instead of x->next->tag
			(x != list->tail && (x->next->tag - x->tag <= 1)) )
	{
		/// Thin out the list - make room for y
		rebalance_tl(list, x);

		/// Dont assign pointers, call insert again to put y after x
		insert_tl(x, y);
	}
	else
	{
		/// NOTE: This is not atomic

		/// Reassign prev/next pointers
		y->prev = x;
		y->next = x->next;
		x->next = y;
		if (y->next != NULL)
			y->next->prev = y; ///< If not null, the we can update the next nodes prev reference
		else 
			list->tail = y; ///< If x was the previous tail, y->next is NULL and is new tail 

		list->size += 1;
	}

}


/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  order
 *  Description:  Determines the total ordering (that is top and bottom ordering) of nodes x and y.
 *  			  Returns 1 if x < y
 *  			  Returns 0 if x > y
 *                
 * =====================================================================================
 */
int order (OM_Node * x, OM_Node * y)
{
	/// If they're in the same list, compare tags
	if (x->ds == y->ds )
	{
		if (x->tag < y->tag)
			return 1;
		else 
			return 0;
	}
	
	/// Otherwise, compare their lists' tags
	else
	{
		if (x->ds->tag < y->ds->tag)
			return 1;
		else
			return 0;
	}
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  split_bl
 *  Description:  Splits the bottom list into two bottom lists. Then it inserts the second
 *  			  after the first in the top list.
 * =====================================================================================
 */
void split_bl (Top_List * list, Bottom_List * list_to_split)
{
	OM_Node * current = list_to_split->head, *middle_node;
	
	/// Create new list to add to top 
	Bottom_List * to_add = create_bl();
	
	/// Keep track of num  nodes visited
	int node_count = 1;

	/// Each node in the list will be spaced out by skip_size tag spaces
	/// NOTE: +2 needed instead of +1 to ensure small enough skip size for odd-sized lists
	unsigned long skip_size = MAX_NUMBER / ((list_to_split->size >> 1) + 2); 

	/// Iterate to the middle updating tags along the way
	while (node_count < (list_to_split->size >> 1))
	{
		current->next->tag = current->tag + skip_size;
		current = current->next;	
		node_count++;
	}

	/// Update sizes of the lists
	to_add->size = list_to_split->size - node_count;
	list_to_split->size = node_count;
	
	/// Make current the end of the original list and save the middle
	list_to_split->tail = current;
	middle_node = current->next;
	current->next = NULL;

	/// Get current node back;
	current = middle_node;

	/// Set the head of the new list to middle node
	to_add->head = middle_node;

	/// Nullify prev of the middle node since it's the head
	middle_node->prev = NULL;

	/// Assign 0 to first tag of to_add list
	current->tag = 0;

	/// Reassign current ds
	current->ds = to_add;

	/// Iterate through remaining nodes updating tags and their ds
	while (current->next != NULL)
	{
		current = current->next;	
		current->ds = to_add;
		current->tag = current->prev->tag + skip_size;
	}

	/// Make the last node the tail of the to_add list
	to_add->tail = current;
	current->next = NULL;

	/// No longer need reordered
	list_to_split->reorder_flag = to_add->reorder_flag = 0;

	/// Insert the newly created list this into the top list
	insert_tl(list_to_split, to_add);
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  rebalance_tl
 *  Description:  Rebalances the top list around a pivot node according to Bender's 
 *  			  algorithm.
 * =====================================================================================
 */
void rebalance_tl (Top_List * list, Bottom_List * pivot)
{
	/// Pointers for walking out from the pivot
	Bottom_List *lList = pivot, *rList = pivot;
	
	/// Constants used to calculate when to rebalance
	double overflow_density, overflow_threshold, i = -1;
	unsigned long enclosing_tag_range, lTag, rTag, num_elements_in_sublist = 2, skip_size;

	/// Check if range is in overflow
	do	
	{
		/// Move overflow list head and tail outward
		if (lList->prev)
		{
			num_elements_in_sublist++;
			lList = lList->prev;
			lTag = lList->tag;
		}
		if (rList->next)
		{
			num_elements_in_sublist++;
			rList = rList->next;
			rTag = rList->tag;
		}
		else
		{
			/// If we reach the tail, just make the tag the maximum number
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
	skip_size = (unsigned long)( enclosing_tag_range / (num_elements_in_sublist + 1) );

#ifdef RD_DEBUG
	if (rTag != MAX_NUMBER)
		assert(skip_size>0 && ((skip_size * (num_elements_in_sublist - 1)) + lList->tag <= rList->tag ));
	else
		assert(skip_size>0 && ((skip_size * (num_elements_in_sublist - 1)) + lList->tag <= MAX_NUMBER ));		
#endif

	/// Relabel the tag range
	relabel_tl_tag_range(lList, rList, skip_size);
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  relabel_tl_tag_range
 *  Description:  Relabels the range of tags from list start to end using a distance of skip_size.
 * =====================================================================================
 */
void relabel_tl_tag_range (Bottom_List *start, Bottom_List *end, const long skip_size)
{
	Bottom_List * current = start;
	current->tag = start->tag;
	
	/// Iterate through and update tags
	do
	{
		current->next->tag = current->tag + skip_size;
		current = current->next;
	}
	while (current != end);
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  rebalance_bls
 *  Description:  Rebalance all bottom lists with the reorder flag.
 * =====================================================================================
 */
void rebalance_bls (Top_List * list)
{
	/// The Iterators
	Bottom_List * current, * current_h;
	current = list->head;

	while(current != NULL)
	{
		if(current->reorder_flag == 1) ///< If 1, then needs split
			split_bl(list, current);
		current = current->next;
	}
}


/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  print_tl
 *  Description:  Print all the tags of the "nodes" in Top_List list.
 * =====================================================================================
 */
void print_tl (Top_List * list)
{
	Bottom_List * current = list->head;

	printf ( "(size = %i) Head->", list->size );
	while (current != NULL)
	{
		printf ( "(%lu)->", current->tag);
		current = current->next;
	}
	printf ( "Tail\n" );
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  print_bl
 *  Description:  Print all the ID's of the nodes in the Bottom_List list.
 * =====================================================================================
 */
void print_bl (Bottom_List * list)
{
	OM_Node * current = list->head;

	printf ( "(size = %i) Head->", list->size );
	while (current != NULL)
	{
		printf ( "(%i | %lu)->", current->ID, current->tag);
		current = current->next;
	}
	printf ( "Tail\n" );
}


/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  free_bl
 *  Description:  Free all nodes in bottom list and the list itself
 * =====================================================================================
 */
void free_bl (Bottom_List * list)
{
	OM_Node *next, * current = list->head;

	/// Iterate through and free the OM_Node's membory
	while (current != NULL){
		next = current->next;	
		free(current);
		current = NULL; ///< Prevent dangling pointers
		current = next;
	}

	/// Finally free the Bottom_List
	free(list);
	list = NULL; ///< Prevent dangling pointers
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  free_tl
 *  Description:  Free all Bottom_Lists in Top_List list and the list itself.
 * =====================================================================================
 */
void free_tl (Top_List * list)
{
	Bottom_List *next, * current = list->head;

	/// Iterate through and call free_bl on the Bottom_List current
	while (current != NULL){
		next = current->next;	
		free_bl(current); ///< Implicitly prevents dangling pointers
		current = next;
	}

	/// Finally free the Top_List
	free(list);
	list = NULL; ///< Prevent dangling pointers
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  check_sub_correctness
 *  Description:  Checks the list so each node's tag is less than the next node's.
 * =====================================================================================
 */
void check_sub_correctness (Top_List * list)
{
	Bottom_List * current = list->head;
	OM_Node * cur_node;

	/// Iterate through the Bottom_List(s)
	while (current != NULL)
	{
		/// Iterate through the OM_Node(s) in each Bottom_List
		cur_node = current->head;
		while(cur_node != current->tail)
		{	
			if ( !(cur_node->tag < cur_node->next->tag) )
				printf("Node tags are out of order\n");
			cur_node = cur_node->next;
		}
		current = current->next;
	}
}

