/*
 * =====================================================================================
 *
 *       Filename:  order-maintenance-par-rebalances
 *
 *    Description:  Source file for Bender's OM-Data Structure with parallel rebalances
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

#include "order-maintenance-par-rebalances.h"

/// Constants used within this source file
static unsigned long int MAX_NUMBER = ~0;
static int INT_BIT_SIZE = 64;
static double OVERFLOW_CONSTANT = 1.5;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  build_array_from_rebalance_list
 *  Description:  Gets an array of all of the nodes that need to be put in the rebalanced tree.
 * =====================================================================================
 */
Internal_Node ** build_array_from_rebalance_list(Internal_Node *current_node, unsigned int num_children){
	/// Holds pointers to all of the internal nodes needed
	Internal_Node ** nodeArray = malloc(sizeof(Internal_Node *) * num_children)
#ifdef RD_DEBUG
	assert(current_node != NULL);
#endif
	/// Get the right most node and store it in current_node
	while (current_node->lvl > 1){
		if (current_node->right)
			current_node = current_node->right;
		else
			current_node = current_node->left;
	}
	while (num_children > 0){
	#ifdef RD_DEBUG
		assert(current_node != NULL);
	#endif
		nodeArray[--num_children] = current_node;
		current_node = current_node->bl->prev->internal;
	}

	return nodeArray;
}

/// Create the tree above x and y
void create_btree_scaffolding(Bottom_List *_x, Bottom_List *_y){
	/// TODO: double check validity
	/// Get the internal node
	Internal_Node *x = _x->internal, *y = _y->internal;
	unsigned int current_lvl = 1,
				 xtag = x->tag,
				 ytag = y->tag, 
				 lvl_count = INT_BIT_SIZE,
				 bit_counter = (0x1) << ( INT_BIT_SIZE - 1);

	/// This will get the first bit from the left in x->tag and y->tag that 
	/// are not the same. That bit (counted from the right) will be lvl_count.
	while ( ((!(xtag ^ ytag)) & bit_counter) == bit_counter){
		lvl_count--;
		bit_counter = bit_counter >> 1;
	}

	bit_counter = 0x1;

	// This is lvl_count -1 because at lvl count we want to create the same node
	while (current_lvl < lvl_count-1){
		/// Deal with X
		if (!(x->parent))
				x->parent = malloc(sizeof(Internal_Node));
		/// Assign x->parent's reference to x (left if bit is 0, right if bit is 1)
		if (xtag & bit_counter == bit_counter)
			x->parent->right = x;
		else
			x->parent->left = x;

		x->parent->num_children++;
		x = x->parent;

		/// Deal with Y
		if (!(y->parent))
				y->parent = malloc(sizeof(Internal_Node));
		/// Assign y->parent's reference to y (left if bit is 0, right if bit is 1)
		if (ytag & bit_counter == bit_counter)
			y->parent->right = y;
		else
			y->parent->left = y;

		y->parent->num_children++;
		y = y->parent;

		/// Update base
		if (current_lvl = 1 ){
			x->parent->base = (_x->tag >> 1) << 1;
			y->parent->base = (_y->tag >> 1) << 1;
		}
		else {
			x->parent->base = (x->base >> current_lvl) << current_lvl; 
			y->parent->base = (y->base >> current_lvl) << current_lvl; 
		}
		/// Update bit_counter (move up one bit/multiply by 2)
		bit_counter = bit_counter <<  1;
		/// Update lvl of x/y
		x->lvl = y->lvl = ++current_lvl;
	}

	//Now current_lvl == lvl_count-1
	if (x->parent || y->parent){
		if (x->parent)
		{
			x->parent->left = x;
			x->parent->right = y;
			y->parent = x->parent;	
		}
		else if (y->parent) {
			y->parent->left = x;
			y->parent->right = y;
			x->parent = y->parent;
		}
	}
	else
	{
		x->parent = y->parent = malloc(sizeof(Internal_Node));
		x->parent->lvl = ++current_lvl;
	}

	x->parent->num_children = x->num_children + y->num_children;

}

void insert(OM_Node *x, OM_Node *y){
	/*InsertRecord *ir = malloc(sizeof(InsertRecord));*/
	/*ir->x  = x;*/
	/*ir->y  = y;*/
	
	/*Cilk_batchify(_cilk_ws, &batchInsertOp ,NULL, ir, sizeof(InsertRecord), NULL);*/
	/// This is the C (non cilk) version
	insert_internal(x, y);
}

//This is for the cilk version 
/*cilk void batchInsertOp (void *dataStruct, void *data, size_t size, void *result)*/
/*{*/
	/*int i = 0;*/
	/*InsertRecord * irArray = (InsertRecord *)data,* ir;*/
	/*/// use a regular for loop to be sequential*/
	/*for(; i < size; i++){*/
		/*ir = &irArray[i];	*/
		/*insert_internal(ir->x, ir->y); */
	/*}*/
/*}*/

void insert_internal(OM_Node *x, OM_Node *y){
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

		/// Correct for adding two odd numbers (MAX_NUMBER is always odd)
		if (x->tag & 0x1 == 0x1) y->tag += 1;

		/// Check for collision
		if ((MAX_NUMBER - x->tag) <= 1) ///< If x is tail, use MAX_NUMBER instead of x->next->tag		
		{
#ifdef RD_STATS
			if (ds->list_of_size_of_bottom_list_when_split_head == NULL)
			{
				ds->list_of_size_of_bottom_list_when_split_head = malloc(sizeof(ll_node));
				ds->list_of_size_of_bottom_list_when_split_tail = ds->list_of_size_of_bottom_list_when_split_head;
				ds->list_of_size_of_bottom_list_when_split_head->data = ds->size;
			}
			else
			{
				ll_node * nextnode = malloc(sizeof(ll_node));
				ds->list_of_size_of_bottom_list_when_split_tail->next = nextnode;
				ds->list_of_size_of_bottom_list_when_split_tail = nextnode;
				nextnode->next = NULL;
				nextnode->data = ds->size;
			}

#endif
			split_bl(ds->parent, ds);
			printf ( "Depth:%i\n", depth );
			insert(x, y,depth++ );
			return;
		}

		ds->tail = y;
	}
	else
	{
		/// y's tage is the average of its neighbors
		y->tag = (x->tag >> 1) + (x->next->tag >> 1);

		/// Correct for adding two odd numbers (MAX_NUMBER is always odd)
		if (x->next->tag & x->tag & 0x1 == 0x1) y->tag += 1;
			

		/// Check for collision
		if ((x->next->tag - x->tag) <= 1)
		{
#ifdef RD_STATS
			
			if (ds->list_of_size_of_bottom_list_when_split_head == NULL)
			{
				ds->list_of_size_of_bottom_list_when_split_head = malloc(sizeof(ll_node));
				ds->list_of_size_of_bottom_list_when_split_tail = ds->list_of_size_of_bottom_list_when_split_head;
				ds->list_of_size_of_bottom_list_when_split_head->data = ds->size;
			}
			else
			{
				ll_node * nextnode = malloc(sizeof(ll_node));
				ds->list_of_size_of_bottom_list_when_split_tail->next = nextnode;
				ds->list_of_size_of_bottom_list_when_split_tail = nextnode;
				nextnode->next = NULL;
				nextnode->data = ds->size;
			}

#endif

			split_bl(ds->parent, ds);
			
			printf ( "Depth:%i\n", depth );
			insert(x, y,depth++);
			return;
		}

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
	return;
}

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
#ifdef RD_STATS
	list->list_of_size_of_bottom_list_when_split_head = list->list_of_size_of_bottom_list_when_split_head = NULL;
#endif

	return list;
}

/*!
 * ===  FUNCTION  ======================================================================
 *         Name:  create_tl
 *  Description:  Allocate memory and initialize Top_List; inialize and insert first Bottom_List.
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
#ifdef RD_STATS
	list->list_of_size_of_top_list_when_split_head = list->list_of_size_of_top_list_when_split_head = NULL;
#endif

	/// Insert the first Bottom_List
	Bottom_List * bl = create_bl();
	first_insert_tl(list, bl);

	return list;
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  first_insert_bl
 *  Description:  Insert the first OM_Node y into the Bottom_List ds.
 * =====================================================================================
 */
void first_insert_bl (Bottom_List * ds, OM_Node * y)
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
 *         Name:  first_insert_tl
 *  Description:  Insert y into list and initialize the necessary parameters of each.
 * =====================================================================================
 */
void first_insert_tl (Top_List * list, Bottom_List * y)
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
		printf("Size was not 0. Improper call to first_insert_tl\n");
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

	/// Parallel: Binary tree internal node
	y->internal = malloc(sizeof(Internal_Node));
	// Base level
	y->internal->lvl = 1;
	// This is a leaf internal node, so no children. Base won't be used in leaf node.
	y->internal->num_children = y->internal->base =  0; 
	y->internal->parent = y->internal->left = y->internal->right = NULL;
	// Give a reference to the internal node to y itself
	y->internal->bl = y;
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  first_insert
 *  Description:  Insert y into the first Bottom_List in the Top_List specified (list)
 * =====================================================================================
 */
void first_insert (Top_List * list, OM_Node * y)
{
	/// Call the function for the first insert in a Bottom_List
	first_insert_bl(list->head, y);
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
	else /// Not tail
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
		rebalance_tl(x);

		/// PARALLEL:
		/*spawn rebalance_tl(x);sync;*/

		/// Dont assign pointers, call insert again to put y after x
		insert_tl(x, y);
	}
	else /// No collision
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

		/// Assign the parent of y to the list
		y->parent = list;
		list->size += 1;

		///Parallel: Create binary tree scaffolding
		///TODO: Make this parallel
		create_btree_scaffolding(x, y);
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
	unsigned long int skip_size = MAX_NUMBER / ((list_to_split->size >> 1) + 2); 

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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rebuild_tree
 *  Description:  Recursively calls rebuild on it's children. If lvl 2, then assign children.
 * =====================================================================================
 */
void rebuild_tree(Internal_Node * current_node, Internal_Node * nodeArray, int startIndex,  int endIndex){
	int newStartIndex, newEndIndex;
#ifdef RD_DEBUG
	if (current_node->lvl < 2){
	{

		printf ( "Debug: rebuild tree lvl is too small.\n" );
		assert(0);
	}
#endif 

	if (current_node->lvl == 2)
	{
		switch (endIndex - startIndex){
			case -1:
				current_node->left = NULL;
				current_node->right = NULL;
				current_node->num_children = 0;
			case 0:
				current_node->left = nodeArray[startIndex];
				current_node->right = NULL;
				current_node->num_children = 1;
				break;
			case 1:
				current_node->left = nodeArray[startIndex];
				current_node->right = nodeArray[endIndex];
				current_node->num_children = 2;
				break;
			default:
#ifdef RD_DEBUG
				printf ( "Debug: error too many nodes given to this internal node.\n" );
#endif
				break;
		}
	}
	else {
		//Parallel:
		current_node->num_children = (endIndex - startIndex) + 1;
		// Get new start and end indexes
		if (startIndex == endIndex){
			startIndex = 1;
			endIndex  = 0;
		}
		else {
			//startIndex stays the same
			//so does endIndex
			newEndIndex = endIndex >> 1;
			newStartIndex = newEndIndex + 1;

		}

		if(! (current_node->left))
		{ //No left branch, so make one
			current_node->left = malloc(sizeof(Internal_Node));
			current_node->left->lvl = current_node->lvl - 1;
			current_node->left->base = current->base;	
		}

		if(! (current_node->right))
		{ //No right branch, so make one
			current_node->right = malloc(sizeof(Internal_Node));
			current_node->right->lvl = current_node->lvl - 1;
			//Append a 1 onto the base
			current_node->right->base = current->base + (0x1 << (current_node->right->lvl - 1 ));	
		}
		rebuild_tree(current_node->left, nodeArray, startIndex, newEndIndex);
		rebuild_tree(current_node->right, nodeArray, newStartIndex, endIndex);
	}

}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  rebalance_tl
 *  Description:  Rebalances the top list around a pivot node according to Bender's 
 *  			  algorithm.
 * =====================================================================================
 */
void rebalance_tl (Bottom_List * pivot){
			///TODO: degug logically
#ifdef RD_STATS
			
			if (list->list_of_size_of_top_list_when_split_head == NULL)
			{
				list->list_of_size_of_top_list_when_split_head = malloc(sizeof(ll_node));
				list->list_of_size_of_top_list_when_split_tail = list->list_of_size_of_top_list_when_split_head;
				list->list_of_size_of_top_list_when_split_head->data = list->size;
			}
			else
			{
				ll_node * nextnode = malloc(sizeof(ll_node));
				list->list_of_size_of_top_list_when_split_tail->next = nextnode;
				list->list_of_size_of_top_list_when_split_tail = nextnode;
				nextnode->next = NULL;
				nextnode->data = list->size;
			}

#endif
	/// Temp current internal node
	Internal_Node *current_node = pivot->internal;
	
	/// Constants used to calculate when to rebalance
	double overflow_density, overflow_threshold, i = -1;
	unsigned int current_tag_range = 1, current_tree_lvl = 1;
	/// Check if range is in overflow
	/// Calculate overflow_density
	//
	do 
	{	
		if (current_node->parent)
			current_node = current_node->parent;
		else //For whatever reason, the existing scaffolding is not large enough
			 //to hold the nodes and still be under the threshold.
		{
			//TODO: figure if this ever happens and how to deal with it.
			assert(0);
			current_node->parent = malloc(sizeof(Internal_Node));
			//if the ith bit is 1, it's parent should look to it as the right node
			if (current_node & current_tag_range == current_tag_range)
				current_node->parent->right = current_node;
			else
				current_node->parent->left = current_node;
			current_node = current->node;

		}
		///Double tag range
		current_tag_range = current_tag_range << 1;

		/// This would have current_tree_lvl -1 if the current_tree_lvl++ were before this line.	
		overflow_threshold = pow(OVERFLOW_CONSTANT, -1.0 * (current_tree_lvl));
	
		overflow_density = ((double)current_node->num_children) / ((double)current_tag_range);

		/// Increase the tree level
		current_tree_lvl++;
	}
	while ( (overflow_density > overflow_threshold ) && (current_node->lvl < INT_BIT_SIZE) );
	//TODO: put list into array
	Internal_Node ** nodeArray = build_array_from_rebalance_list(current_node);

	
	//Parallel: rebuild left and right part of tree
	//TODO: make parallel
#ifdef RD_DEBUG
	assert(current_node->num_children > 0);
#endif
	rebuild_tree(current_node->left, nodeArray, 0, (signed)(current_node->num_children / 2));
	rebuild_tree(current_node->right,  nodeArray, (signed)(current_node->num_children / 2) + 1, (signed)current_node->num_children);
	free(nodeArray);
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  relabel_tl_tag_range
 *  Description:  Relabels the range of tags from list start to end using a distance of skip_size.
 * =====================================================================================
 */
void relabel_tl_tag_range (Bottom_List *start, Bottom_List *end, const long int  skip_size)
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
		current = NULL; ///< Prevent dangling pointers
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

