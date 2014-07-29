/*
 * =====================================================================================
 *
 *       Filename:  order-maintenance-par-rebalance
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

#include "order-maintenance-par-rebalance.h"

/// Constants used within this source file
static unsigned /*long*/ int MAX_NUMBER = 255;
static int INT_BIT_SIZE = 8;
static int HALF_INT_BIT_SIZE = 4;
static int lg_HALF_INT_BIT_SIZE = 2;
static double OVERFLOW_CONSTANT = 1.05;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  build_array_from_rebalance_list
 *  Description:  Gets an array of all of the nodes that need to be put in the rebalanced tree.
 * =====================================================================================
 */
Internal_Node ** build_array_from_rebalance_list (Internal_Node *current_node)
{
	/// +1 because we want to include the new node
	int num_children = (signed int)current_node->num_children + 1, i=0;

	/// Holds pointers to all of the internal nodes needed
	Internal_Node ** nodeArray = malloc(sizeof(Internal_Node *) * num_children);

#ifdef RD_DEBUG
	assert(current_node != NULL);
	assert(num_children > 1);
#endif

	/// Get the left most base node and store it in current_node
	while (current_node->lvl > 1)
	{
		if (current_node->left)
			current_node = current_node->left;
		else
			current_node = current_node->right;
	}

	nodeArray[i++] = current_node;
	/// Iterate from the last Bottom_List storing the internal nodes in nodeArray
	while (i < num_children)
	{

#ifdef RD_DEBUG
		assert(current_node != NULL);
#endif

		// IF this was the node that we inserted that has no place to fit in the tags
		if (!(current_node->bl->next->internal) )
			//We need to allocate space for it's internal node
		{
#ifdef RD_DEBUG		
			printf ( "Allocating space for node to be inserted (in build array)\n" );
#endif
			current_node->bl->next->internal = malloc(sizeof(Internal_Node));
			// We dont need a base, that will be assigned in rebuiild_tree
			// Same for parent, left, right 
			current_node->bl->next->internal->lvl = 1;
			// Assign the internal node's bl pointer to the bl to be inserted
			current_node->bl->next->internal->bl = current_node->bl->next;
		}
		current_node = current_node->bl->next->internal;
		nodeArray[i++] = current_node;
	}

	return nodeArray;
}

/// Create the tree above x and y
void create_btree_scaffolding (Internal_Node *x, Internal_Node *y)
{
	unsigned int 
			xtag = x->bl->tag,
			ytag = y->bl->tag,
			ztag = ytag,//Just a tmp variable
			lvl_count = INT_BIT_SIZE, // This represents the lvl where x and y have their most recent common ancestor
			bit_counter = (0x1) << ( INT_BIT_SIZE - 1);


	/// Find whether y->prev or y->next  has a closer common ancestor to x.
	/// If y has no next, then use x.
	if ((y->bl->next))
	{
		ztag = y->bl->next->tag;
		while (bit_counter != 0) {
			if ( (((ztag ^ ytag)) & bit_counter) == bit_counter) 
			{
				/// If z and y bit are different, then x will be closer to y. Use x.
				/// Find the right lvl
				while ( (((xtag ^ ytag)) & bit_counter) == bit_counter) 
				{
					lvl_count--;
					bit_counter = bit_counter >> 1;
				}
				break;
			}

			else if ( (((xtag ^ ytag)) & bit_counter) == bit_counter) 
			{
				/// If x and y bit are not the same at the bit_counter, then Z will be the closer node. 
				/// We then assign z to x.
				/// Before that find the right lvl
				while ( (((ztag ^ ytag)) & bit_counter) == bit_counter) 
				{
					lvl_count--;
					bit_counter = bit_counter >> 1;
				}
				x = y->bl->next->internal;
				//Use y->next as x
				xtag = ztag;
				break;
			}
			lvl_count--;
			bit_counter = bit_counter >> 1;
		}
	}
	else{
		while (bit_counter != 0) {
			if ( (((xtag ^ ytag)) & bit_counter) == bit_counter) 
			{
				break;
			}
			lvl_count--;
			bit_counter = bit_counter >> 1;
		}
	}

	Internal_Node * new_parent, * iter_node;

	/// The old parent is below the new parent that is to be created
 	if (x->parent->lvl < lvl_count){
		printf ( "Debug: Create scaffold - old parent below new parent\n" );
 		new_parent = x->parent;

 		while (new_parent->lvl < lvl_count)
		{
			/// Iter node follows new parent
			iter_node = new_parent;
			new_parent = new_parent->parent;
		}
#ifdef RD_DEBUG
		assert(new_parent->lvl == lvl_count);
#endif
		

		if (xtag != ztag) // if x/z not swapped
		{
			/*new_parent->left = iter_node;*/
			new_parent->right = y;
		}
		else {
			/*new_parent->right = iter_node;*/
			new_parent->left = y;
		}
		y->parent = new_parent;
		/// Assign lvl to new parent
		new_parent->lvl = lvl_count;
		new_parent->num_children = iter_node->num_children + 1;
	}
	/// The old parent is above new parent that is to be created
	else if (x->parent->lvl > lvl_count){
		printf ( "Debug: Create scaffold - old parent above new parent\n" );
 		new_parent = malloc(sizeof(Internal_Node));

		// Assign x's old parent to new_parent's parent
		new_parent->parent = x->parent;
		//Increase children count of old parent
		// DOING THIS AT END OF FUNCTION
		/*x->parent->num_children++;*/

		if (xtag != ztag) // if x/z not swapped
		{
			new_parent->left = x;
			new_parent->right = y;
		}
		else {
			new_parent->left = y;
			new_parent->right = x;
		}

		//Reassign the old parent's left/right references
		if (x->parent->left == x)
		{
			x->parent->left = new_parent;
		}
		else if (x->parent->right == x)
			x->parent->right = new_parent;
		else
			{
			printf ( "Debug: Create scaffolding -x->parent has two children already, or no children.\n" );
			assert(0);
		}

		//Assign x/y parent
		x->parent = y->parent = new_parent;
		/// Assign lvl to new parent
		new_parent->lvl = lvl_count;
		// This is a new node with just 2 children
		new_parent->num_children = 2;
	}
	else //they are equal, i.e. the same node 
	{
		printf ("Debug : Create scaffolding - Old and new parent of x are the same\n" );
		new_parent = malloc(sizeof(Internal_Node));
		new_parent->parent = x->parent;
		new_parent->lvl = lvl_count;
		// Is this right?
		new_parent->num_children = 2;

		//Reassign the old parent's left/right references
		if (x->parent->left == x)
			x->parent->left = new_parent;
		else if (x->parent->right == x)
			x->parent->right = new_parent;
		else
			{
			printf ( "Debug: Create scaffolding -x->parent has two children already, or no children.\n" );
			assert(0);
		}

		if (xtag != ztag) // if x and z not swapped	
		{
			new_parent->right = y;		
		}
		else{
			new_parent->left = y;
		}
		y->parent = x->parent = new_parent;
	}

    // Update number of children
	iter_node = new_parent->parent;
	while (iter_node != NULL)
	{
		iter_node->num_children += 1;
		iter_node = iter_node->parent;
	}
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
	Bottom_List *ds = x->ds;
#ifdef RD_DEBUG
	/// y and ds should never be NULL
	if ( !(x && y) )
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
		if ((x->tag & 0x1) == 0x1) y->tag += 1;

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
			insert(x, y );
			return;
		}

		ds->tail = y;
	}
	else
	{
		/// y's tage is the average of its neighbors
		y->tag = (x->tag >> 1) + (x->next->tag >> 1);

		/// Correct for adding two odd numbers (MAX_NUMBER is always odd)
		if ((x->next->tag & x->tag & 0x1) == 0x1) y->tag += 1;
			

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
			
			insert(x, y);
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
void first_insert_tl (Top_List * list, Bottom_List * _y)
{
	Internal_Node *y;
	int i = 1;
#ifdef RD_DEBUG
	/// Make sure they're not NULL
	if ( !(list && _y) )
	{
		printf("First node or ds null: ds: %p _y: %p\n", list, _y);
		assert(0);
	}
	
	if (list->size != 0)
	{
		printf("Size was not 0. Improper call to first_insert_tl\n");
		assert(0);
	}
#endif
	
	/// Update necessary _y parameters
	_y->parent = list;
	list->head = list->tail = _y;	
	_y->tag = 0;

	_y->reorder_flag = 0;
	_y->next = _y->prev = NULL;
	list->size += 1;

	/// Parallel: Binar_y tree internal node
	_y->internal = y = malloc(sizeof(Internal_Node));
	// Base level
	y->lvl = 0;
	// This is a leaf internal node, so no children. Base won't be used in leaf node.
	y->num_children = y->base =  0; 
	// Give a reference to the internal node to _y itself
	y->bl = _y;


	Internal_Node * root = malloc(sizeof(Internal_Node));
	root->left = y;
	y->parent = root;
	//Top lvl node
	root->lvl = INT_BIT_SIZE;
	root->num_children = 1;
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  first_insert
 *  Description:  Insert y into the first Bottom_List in the Top_List specified (list)
 * =====RR==============================================================================
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
		if ((x->tag & 0x1) == 0x1) y->tag++;
		/// Make the last number the MAX_NUMBER
		if (MAX_NUMBER -  x->tag == 1)
			y->tag = MAX_NUMBER;
		
	}
	else /// Not tail
	{		
		/// y's tag is the average of the next and prior tags
		y->tag = (x->tag >> 1) + (x->next->tag >> 1);

		/// Correct for adding two odd numbers (MAX_NUMBER is always odd)
		if ((x->next->tag & x->tag & 0x1) == 0x1) y->tag++;
	}
			
	/// See if there is a "collision" of tags
	if (    (x == list->tail && (MAX_NUMBER == x->tag )  ) || ///< If x is tail, use MAX_NUMBER instead of x->next->tag
			(x != list->tail && (x->next->tag - x->tag <= 1)) )
	{
		//Link y in the linked list.

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


#ifdef RD_DEBUG
		
		printf ( "Before rebalance\n" );
		/*print_tree(list);*/
#endif
		/// Thin out the list - make room for y
		rebalance_tl(x);

#ifdef RD_DEBUG

		printf ( "After rebalance\n" );
		/*print_tree(list);*/
#endif
		/// PARALLEL:
		/*spawn rebalance_tl(x);sync;*/

		/// Dont assign pointers, call insert again to put y after x
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
		y->internal = malloc(sizeof(Internal_Node));
		// Base level
		y->internal->lvl = 0;
		// This is a leaf internal node, so no children. Base won't be used in leaf node.
		y->internal->num_children = 0;
		y->internal->base = y->tag; 
		y->internal->parent = y->internal->left = y->internal->right = NULL;
		// Give a reference to the internal node to y itself
		y->internal->bl = y;
		///TODO: Make this parallel
		create_btree_scaffolding(x->internal, y->internal);
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
	/* PREVIOUS VERSION
	OM_Node * current = list_to_split->head, *middle_node;
	/// Create new list to add to top 
	Bottom_List * to_add = create_bl();
	/// Keep track of num  nodes visited
	int node_count = 1;
	/// Each node in the list will be spaced out by skip_size tag spaces
	/// NOTE: +2 needed instead of +1 to ensure small enough skip size for odd-sized lists
	unsigned int skip_size = MAX_NUMBER / ((list_to_split->size >> 1) + 2); 
	/// Iterate to the middle updating tags along the way
	while (node_count < (list_to_split->size >> 1)){
		current->next->tag = current->tag + skip_size;
		current = current->next;	
		node_count++;}
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
	while (current->next != NULL){
		current = current->next;	
		current->ds = to_add;
		current->tag = current->prev->tag + skip_size;}
	/// Make the last node the tail of the to_add list
	to_add->tail = current;
	current->next = NULL;
	/// No longer need reordered
	list_to_split->reorder_flag = to_add->reorder_flag = 0;
	/// Insert the newly created list this into the top list
	insert_tl(list_to_split, to_add);
	*/
	OM_Node * current = list_to_split->head, *transition_node;
	
	/// Create new list to add to top 
	Bottom_List * to_add, * holder;
	
	/// Keep track of num nodes and lists through iterations
	int node_count = 1, list_count = 1, num_lists_needed = (list_to_split->size >> lg_HALF_INT_BIT_SIZE);

	/// Each node in the list will be spaced out by skip_size tag spaces
	unsigned long int skip_size = MAX_NUMBER >> lg_HALF_INT_BIT_SIZE;

#ifdef RD_DEBUG
	printf("In New version of split.\n");
#endif

	/// First reorganize list_to_split appropriately
	current->tag = 0;
	while (node_count < HALF_INT_BIT_SIZE && current->next != NULL)
	{
		current->next->tag = current->tag + skip_size;
		current = current->next;
		++node_count;
	}

	/// Finalize list_to_splits adjustments
	list_to_split->size = node_count;
	list_to_split->tail = current;
	transition_node = current->next;
	current->next = NULL;
	list_to_split->reorder_flag = 0;
	
	/// Define the holder for the following lists
	holder = list_to_split;

	/// Now reorganize each set of 32 into a new Bottom_list to_add
	while (list_count < num_lists_needed)
	{
		/// Update node count for current iteration
		node_count = 1; 
		
		/// This particular iteration's list to be added
		to_add = create_bl(); 
		
		/// Node maintenence
		current = transition_node;
		to_add->head = current;
		current->prev = NULL;
		current->tag = 0;

		/// Upate the DS for the head
		current->ds = to_add;
		
		/// Now iterate through the nodes for this iteration's DS
		while ((node_count < HALF_INT_BIT_SIZE) && (current->next != NULL))
		{
			current = current->next;
			current->ds = to_add;
			current->tag = current->prev->tag + skip_size;
			++node_count;
		}
		
		/// Finalize this iteration's DS
		to_add->tail = current;
		transition_node = current->next; ///< Even if NULL (out of nodes), will be fine
		current->next = NULL;
		to_add->reorder_flag = 0;
		to_add->size = node_count;

		/// Insert the finished DS into the Top_List
		insert_tl(holder, to_add);

		/// Update place holder for next Top_List insert
		holder = to_add;
	    to_add = NULL;

		/// Increment the list count
		++list_count;
	}

	/// Finally, check if there are leftover elements and add to a list
	if (transition_node != NULL)
	{
		node_count = 1; 
		
		/// This particular iteration's list to be added
		to_add = create_bl(); 
		
		/// Node maintenence
		current = transition_node;
		to_add->head = current;
		current->prev = NULL;
		current->tag = 0;

		/// Upate the DS for the head
		current->ds = to_add;
		
		/// Now iterate through the nodes for this iteration's DS
		while (current->next != NULL)
		{
			current = current->next;
			current->ds = to_add;
			current->tag = current->prev->tag + skip_size;
			++node_count;
		}
		
		/// Finalize this iteration's DS
		to_add->tail = current;
		current->next = NULL;
		to_add->reorder_flag = 0;
		to_add->size = node_count;

		/// Insert the finished DS into the Top_List
		insert_tl(holder, to_add);
	}


}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rebuild_tree
 *  Description:  Recursively calls rebuild on it's children. If lvl 2, then assign children.
 *  			  If in the right, start counting from the startIndex.
 *  			  If in the left, stop counting 1 before the endIndex.
 * =====================================================================================
 */
void rebuild_tree(Internal_Node * current_node, Internal_Node ** nodeArray, int startIndex,  int endIndex){
	int newStartIndex, newEndIndex, diff = endIndex - startIndex;
	
	if (current_node == NULL )
		return;
	else
		current_node->num_children = diff + 1;
	
#ifdef RD_DEBUG
	if (current_node->lvl < 2)
	{
		printf ( "Debug: rebuild tree lvl is too small.\n" );
		assert(0);
	}
#endif 

	if (current_node->lvl == 2)
	{
		
		printf ( "In a lvl 2 node\n" );
		if (diff == -1)
		{
			current_node->left = NULL;
			current_node->right = NULL;
		}
		else if (diff == 0)
		{
			current_node->left = nodeArray[startIndex];
			current_node->left->parent = current_node;
			current_node->left->base = current_node->left->bl->tag = current_node->base;
			current_node->right = NULL;
		}
		else if (diff == 1)
		{
			/// Left node
			current_node->left = nodeArray[startIndex];
			current_node->left->parent = current_node;
			current_node->left->base = current_node->left->bl->tag = current_node->base;
			/// Right node
			current_node->right = nodeArray[endIndex];
			current_node->right->parent = current_node;
			current_node->right->base = current_node->right->bl->tag = current_node->base + 1;
		}
		else
		{
#ifdef RD_DEBUG
			printf ( "Debug: error too many nodes given to this internal node.\n" );
#endif
		}

		printf ( "My num children %i, my indexes [%i %i].\n", current_node->num_children, startIndex, endIndex );
		return;
	}
	else { // lvl > 2
		printf ( "Indexes before rebuild calls (num_child: %i lvl: %i) : [%i %i]", current_node->num_children,current_node->lvl, startIndex, endIndex);
		//Parallel: Get new start and end indexes
		if (startIndex > endIndex){ // no children
			// Left subarray
			startIndex = 1;
			newEndIndex = 0;
			// Right subarray
			newStartIndex = 1;
			endIndex  = 0;
		}
		else if (startIndex == endIndex) // one child
		{
			newEndIndex = endIndex;
			//Dont use right sub array
			newStartIndex = 1;
			endIndex = 0;
		}
		else {
			//startIndex stays the same
			//so does endIndex
			if ( (diff & 0x1 ) == 0x1) // if endIndex - startIndex is odd
				newEndIndex = startIndex + ((endIndex - startIndex ) / 2 );
			else
				newEndIndex = startIndex + ((endIndex - startIndex + 1) / 2 );

			newStartIndex = newEndIndex + 1;
		}

		printf ( "After: [%i %i] [%i %i]\n", startIndex,newEndIndex,newStartIndex, endIndex );
		if (startIndex <= newEndIndex)
		{
			if (!(current_node->left))
			{
				current_node->left = malloc(sizeof(Internal_Node));
				current_node->left->base = current_node->base;
				current_node->left->parent = current_node;

				current_node->left->lvl = current_node->lvl -1;
				// num children taken care of in rebuild
			}

			rebuild_tree(current_node->left, nodeArray, startIndex, newEndIndex);
		}
		if (newStartIndex <= endIndex ){
			if(!(current_node->right) )
			{
				current_node->right = malloc(sizeof(Internal_Node));
				current_node->right->base = current_node->base;
				current_node->right->parent = current_node;

				current_node->right->lvl = current_node->lvl -1;
				// num children taken care of in rebuild
			}
			rebuild_tree(current_node->right, nodeArray, newStartIndex, endIndex);
		}
	} //end else
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
	unsigned int current_tag_range = 1, current_tree_lvl = 0;
	/// Check if range is in overflow
	/// Calculate overflow_density
	//
	do 
	{	
		/// Increase the tree level
		current_tree_lvl++;

		if (current_node->parent)
			current_node = current_node->parent;
		else //For whatever reason, the existing scaffolding is not large enough
			 //to hold the nodes and still be under the threshold.
		{
			//TODO: figure if this ever happens and how to deal with it.
			assert(0);
			/*current_node->parent = malloc(sizeof(Internal_Node));*/
			/*//if the ith bit is 1, it's parent should look to it as the right node*/
			/*if (current_ & current_tag_range == current_tag_range)*/
			/*current_node->parent->right = current_node;*/
			/*else*/
			/*current_node->parent->left = current_node;*/
			/*current_node = current->node;*/

		}
		///Double tag range
		current_tag_range = current_tag_range << 1;

		/// This would have current_tree_lvl -1 if the current_tree_lvl++ were before this line.	
		overflow_threshold = pow(OVERFLOW_CONSTANT, -1.0 * (current_tree_lvl));
	
		//This is +1 because we still need to insert the needed node in the tag range
		overflow_density = ((double)current_node->num_children + 1) / ((double)current_tag_range);

	}
	while ( (overflow_density > overflow_threshold ) && (current_node->lvl <= INT_BIT_SIZE) );

	/// Gets the 
	Internal_Node ** nodeArray = build_array_from_rebalance_list(current_node);

	
	//Parallel: rebuild left and right part of tree
	//TODO: make parallel
	// Include the extra node
	current_node->num_children += 1;
#ifdef RD_DEBUG
	assert(current_node->num_children > 1);
#endif

	int leftStart = 0;
	int leftEnd = (signed int)((current_node->num_children - 1 ) / 2);
	int rightStart = leftEnd+1;
	int rightEnd = current_node->num_children -1;
	
	if (leftStart <= leftEnd)
	{
		if (!(current_node->left))
		{
			current_node->left = malloc(sizeof(Internal_Node));
			current_node->left->base = current_node->base;
			current_node->left->parent = current_node;

			current_node->left->lvl = current_node->lvl -1;
			// num children taken care of in rebuild
		}

		rebuild_tree(current_node->left, nodeArray, leftStart, leftEnd);
	}
	if (rightStart <= rightEnd ){
		if(!(current_node->right) )
		{
			current_node->right = malloc(sizeof(Internal_Node));
			current_node->right->base = current_node->base;
			current_node->right->parent = current_node;

			current_node->right->lvl = current_node->lvl -1;
			// num children taken care of in rebuild
		}
		rebuild_tree(current_node->right, nodeArray, rightStart, rightEnd);
	}

	free(nodeArray);
}

/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  relabel_tl_tag_range
 *  Description:  Relabels the range of tags from list start to end using a distance of skip_size.
 * =====================================================================================
 */
void relabel_tl_tag_range (Bottom_List *start, Bottom_List *end, const /*long*/ int  skip_size)
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
 e/
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
		printf ( "(%u)->", current->tag);
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

	printf ( "(size = %i) Head->", list->size );
	while (current != NULL)
	{
		printf ( "(%i | %u)->", current->ID, current->tag);
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
		if (current->next && current->tag >=current->next->tag )
			printf ( "Bottom list tags are out of place\n" );
		current = current->next;
	}
}


void LNR (Internal_Node * current, int level)
{
	int temp = 9-level;
	if (current != NULL)
	{
		LNR(current->left, level-1);
		while (temp > 0)
		{
			printf("\t");
			--temp;
		}
		printf("Level: %i [%i, mem: %p, L: %p, R: %p]\n", level, current->base, current, current->left, current->right);
		LNR(current->right, level-1);
	}
}

void print_tree (Top_List * list)
{
	Internal_Node * current = list->head->internal;
	int level = 9;

	/// Get Current = root
	while (current->parent != NULL)
		current = current->parent;

	LNR(current, level);
	
}
