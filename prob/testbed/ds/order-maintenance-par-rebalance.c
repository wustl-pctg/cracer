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
#include <time.h>

/// Constants used within this source file
static unsigned /*long*/ int MAX_NUMBER = ~0;
static int INT_BIT_SIZE = 32;
static int HALF_INT_BIT_SIZE = 16;
static int lg_HALF_INT_BIT_SIZE = 4;
static double OVERFLOW_CONSTANT = 1.30;
static unsigned int rebuild_tree_count = 0;


void print_rebuild_tree(){
	printf ( "Rebuld tree count: %i\n", rebuild_tree_count );
}

/// Helper function that checks that the tree is correct
void check_subtree_correctness( Internal_Node *x)
{
	if (x->lvl == 0)
	{
		assert(x->bl != NULL);
		assert(x->bl->internal == x);
		assert(x->left == NULL);
		assert(x->right == NULL);
		assert(x->base <= x->parent->right->base);
		assert(x->base >= x->parent->left->base);
		assert(x->bl->tag == x->base);
		return;
	}
	else
	{
		assert(x->left);
		assert(x->right);
	
		assert(x->left->base == x->base);
	
		assert(x->right->base > x->base);
		assert(x->left->parent == x);
		assert(x->right->parent == x);
		if (x->num_children == 2)
		{
			assert(x->left->lvl == 0);
			assert(x->right->lvl == 0);
		}
		if (x->left->lvl != 0 && x->right->lvl != 0)
		{
			assert(x->left->num_children + x->right->num_children == x->num_children);
		}
		else if ((x->left->lvl != 0) ^ (x->right->lvl != 0))
		{
			if (x->left->lvl == 0)
			{
				assert(x->right->num_children + 1 == x->num_children);
			}
			if (x->right->lvl == 0){
				assert(x->left->num_children + 1 == x->num_children);
			}
		}
		assert(x->num_children != 1);

		/// Now recurse down the tree
		check_subtree_correctness(x->left);
		check_subtree_correctness(x->right);

		return;
	}
}

void check_tree_correctness (Internal_Node * x){
	while (x->parent)
	{
		x = x->parent;
	}
	if (x->lvl == 0)
	{
		assert(x->bl != NULL);
		assert(x->left == NULL);
		assert(x->right == NULL);
		assert(x->base <= x->parent->right->base);
		assert(x->base >= x->parent->left->base);
		assert(x->bl->tag == x->base);
		return;
	}

	assert(x->left);
	assert(x->right);
	assert(x->left->base == x->base);

	assert(x->left->parent ==x);
	assert(x->right->parent == x);
	if (x->num_children == 2)
	{
		assert(x->left->lvl == 0);
		assert(x->right->lvl == 0);
	}
	if (x->left->lvl != 0 && x->right->lvl != 0)
	{
		assert(x->left->num_children + x->right->num_children == x->num_children);
	}
	else if ((x->left->lvl != 0) ^ (x->right->lvl != 0))
	{
		if (x->left->lvl == 0)
		{
			assert(x->right->num_children + 1 == x->num_children);
		}
		if (x->right->lvl == 0)
		{
			assert(x->left->num_children + 1 == x->num_children);
		}
	}

	assert(x->num_children != 1);
	check_subtree_correctness(x->left);
	check_subtree_correctness(x->right);

	return;
}

static double scaffolding_total_time = 0;
void print_scaffolding_timing(){

	printf ( "Scaffolding total time(s):%f \n", (scaffolding_total_time  / CLOCKS_PER_SEC));
}

/// Create the tree above x and y
void create_scaffolding (Internal_Node *x, Internal_Node *y)
{
    unsigned int
		xtag = x->bl->tag,
		ytag = y->bl->tag,
		ztag = ytag, ///< Just a tmp variable
		lvl_count = INT_BIT_SIZE, ///< This represents the lvl where x and y have their most recent common ancestor
		bit_counter = (0x1) << ( INT_BIT_SIZE - 1);

#ifdef RD_TIMING
	clock_t temp = clock();
#endif

    /// Find whether y->prev or y->next  has a closer common ancestor to x.
    if ((y->bl->next))
    {
		ztag = y->bl->next->tag;
		while (bit_counter != 0)
		{
		    /// If z and y bit are different, then x will be closer to y. Use x.
		    if ( (((ztag ^ ytag)) & bit_counter) == bit_counter)
		    {
				while ( ((~(xtag ^ ytag)) & bit_counter) == bit_counter)
				{
				    /// Must find the right lvl first
				    lvl_count--;
				    bit_counter = bit_counter >> 1;
				}
				break;
		    }
		    /// If x and y bit are not the same at the bit_counter, then Z will be the closer node.
		    else if ( (((xtag ^ ytag)) & bit_counter) == bit_counter)
		    {
				while ( ((~(ztag ^ ytag)) & bit_counter) == bit_counter)
				{
				    /// Must find the right lvl first
				    lvl_count--;
				    bit_counter = bit_counter >> 1;
				}
				//Use y->next as x
				x = y->bl->next->internal;

				/// We then assign z to x.
				xtag = ztag;
				break;
		    }

		    /// Otherwise keep iterating until a bit match
		    lvl_count--;
		    bit_counter = bit_counter >> 1;
		}
    }
    else ///< If here, y has no next, so use x.
    {
		while (bit_counter != 0)
		{
		    if ( (((xtag ^ ytag)) & bit_counter) == bit_counter)
				break;

		    /// Need to update the level count
		    lvl_count--;
		    bit_counter = bit_counter >> 1;
		}
    }

    Internal_Node * new_parent, * iter_node;

    /// The old parent is below the new parent that is to be created
    if (x->parent->lvl < lvl_count)
    {
#ifdef RD_DEBUG
		//printf ( "Debug: Create scaffold - old parent below new parent\n" );
#endif

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

		/// We know the direction of the child based on if z was changed
		if (xtag != ztag)
		{
		    /*new_parent->left = iter_node;*/
		    new_parent->right = y;
		}
		else
		{
		    /*new_parent->right = iter_node;*/
		    new_parent->left = y;
		}

		y->parent = new_parent;

		/// Assign lvl to new parent
		new_parent->lvl = lvl_count;

		new_parent->bl = NULL;

		/// Update new parent bas
 		if (lvl_count == INT_BIT_SIZE)
 			new_parent->base =0x0;
		else
			new_parent->base = (xtag >> lvl_count) << lvl_count;

		// Update new_parent children
		new_parent->num_children = iter_node->num_children + 1;
    }
    /// The old parent is above new parent that is to be created
    else if (x->parent->lvl > lvl_count)
    {
		new_parent = malloc(sizeof(Internal_Node));

		/// Nullify the base
		new_parent->bl = NULL;

		/// Assign x's old parent to new_parent's parent
		new_parent->parent = x->parent;

		/// We know the direction of the child based on if z was changed
		if (xtag != ztag)
		{
		    new_parent->right = y;
		    new_parent->left = x; ///< Also update left in this case
		}
		else
		{
		    new_parent->left = y;
		    new_parent->right = x; ///< Also update right in this case
		}

		/// Reassign the old parent's left/right references
		if (x->parent->left == x)
		    x->parent->left = new_parent;
		else if (x->parent->right == x)
		    x->parent->right = new_parent;
		else
		{
		    printf ( "Debug: Create scaffolding -x->parent has two children already, or no children.\n" );
		    assert(0);
		}

		/// Assign x/y parent
		x->parent = y->parent = new_parent;

		/// Assign lvl to new parent
		new_parent->lvl = lvl_count;

		/// Update new parent bas
 		if (lvl_count == INT_BIT_SIZE)
 			new_parent->base =0x0;
		else
			new_parent->base = (xtag >> lvl_count) << lvl_count;

		/// This is a new node with just 2 children
		new_parent->num_children = 2;
    }
	else if (lvl_count == INT_BIT_SIZE) // root is already made)
	{
		new_parent = x->parent;
		assert(new_parent->lvl == INT_BIT_SIZE);
		new_parent->num_children = 2;
		new_parent->right = y;
		y->parent = new_parent;
	}
    else ///< They are equal, i.e. the same node
    {
		new_parent = malloc(sizeof(Internal_Node));

		/// Update new parent bas
 		if (lvl_count == INT_BIT_SIZE)
 			new_parent->base =0x0;
		else
			new_parent->base = (xtag >> lvl_count) << lvl_count;

		new_parent->parent = x->parent;
		new_parent->lvl = lvl_count;
		new_parent->num_children = 2;
		new_parent->bl = NULL;

		/// Reassign the old parent's left/right references
		if (x->parent->left == x)
		    x->parent->left = new_parent;
		else if (x->parent->right == x)
		    x->parent->right = new_parent;
		else
		{
		    printf ( "Debug: Create scaffolding -x->parent has two children already, or no children.\n" );
		    assert(0);
		}

		/// We know the direction of the child based on if z was changed
		if (xtag != ztag)
		    new_parent->right = y;
		else
		    new_parent->left = y;

		y->parent = x->parent = new_parent;
    }

    /// Update number of children
    iter_node = new_parent->parent;

    while (iter_node != NULL)
    {
		iter_node->num_children += 1;
		iter_node = iter_node->parent;
    }

//    printf ( "Printing tree after scaffolding\n" );
//    print_tree(x->bl->parent);

#ifdef RD_DEBUG
	if (x->bl->parent->size > 1)
	check_tree_correctness(x);
#endif

#ifdef RD_TIMING
	scaffolding_total_time += (double)(clock() - temp);
#endif

}

void insert(OM_Node *x, OM_Node *y){
    insert_internal(x, y);
}

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

	/// Make sure the internal node is NULL
	list->internal = NULL;

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
	printf("Insert_t\n");
    assert( (list->size == 0 || x != NULL) && (y != NULL) && (list != NULL) );
#endif

    if (x == list->tail)
    {
		/// y's tag is the average of the max and the prior tag
		y->tag = (x->tag >> 1) + (1 << (INT_BIT_SIZE - 1));

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
		/// Thin out the list - make room for y
		rebalance_tl(x);

		/// Dont assign pointers, call insert again to put y after x
		insert_tl(x,y);
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

		/// Base level
		y->internal->lvl = 0;

		/// This is a leaf internal node, so no children. Base won't be used in leaf node.
		y->internal->num_children = 0;
		y->internal->base = y->tag;
		y->internal->parent = y->internal->left = y->internal->right = NULL;

		/// Give a reference to the internal node to y itself
		y->internal->bl = y;

		create_scaffolding(x->internal, y->internal);

    }
}


/*!
 * ===  FUNCTION  ======================================================================
 *         Name:  order
 *  Description:  Determines the total ordering (that is top and bottom ordering) of nodes x and y.
 *  												  Returns 1 if x < y
 *  												  Returns 0 if x > y
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
 *  												  after the first in the top list.
 * =====================================================================================
 */
void split_bl (Top_List * list, Bottom_List * list_to_split)
{
    OM_Node * current = list_to_split->head, *transition_node;

    /// Create new list to add to top
    Bottom_List * to_add, * holder;

    /// Keep track of num nodes and lists through iterations
    int node_count = 1, list_count = 1, num_lists_needed = (list_to_split->size >> lg_HALF_INT_BIT_SIZE);

    /// Each node in the list will be spaced out by skip_size tag spaces
    unsigned long int skip_size = MAX_NUMBER >> lg_HALF_INT_BIT_SIZE;

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

#ifdef RD_DEBUG
		assert(to_add->internal == NULL);
#endif

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

#ifdef RD_DEBUG
		assert(to_add->internal == NULL);
#endif
		/// Insert the finished DS into the Top_List
		insert_tl(holder, to_add);
    }

}

static unsigned int rebuild_skip_count= 0;
void print_rebuild_count(){
	printf ( "rebuild count: %i\n", rebuild_skip_count );
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  par_build_array_from_rebalance_list
 *  Description:  Buiilds the array in parallel
 * =====================================================================================
 */
void par_build_array_from_rebalance_list(Internal_Node ** buildArray,Internal_Node * current_node, int start, int end)
{
	int left_side_end;

	/// Build left side
	if (current_node->left->lvl != 0)
	{
		par_build_array_from_rebalance_list(buildArray, current_node->left, (start), (start + current_node->left->num_children - 1));
		left_side_end = start + current_node->left->num_children;
	}
	else /// This is a leaf node
	{ 
		buildArray[start] = current_node->left;
		left_side_end = start + 1;
	}

	if (current_node->right->lvl != 0)
	{
		par_build_array_from_rebalance_list(buildArray, current_node->right, left_side_end, end);
	}
	else
	{
		buildArray[end] = current_node->right;
	}

}

/*!
 * ===  FUNCTION  ======================================================================
 *         Name:  build_array_from_rebalance_list
 *  Description:  Gets an array of all of the nodes that need to be put in the rebalanced tree.
 * =====================================================================================
 */
Internal_Node ** build_array_from_rebalance_list (Internal_Node *current_node)
{
    /// +1 because we want to include the new node
    int num_children = (signed int)current_node->num_children, i=0;

    /// Holds pointers to all of the internal nodes needed
    Internal_Node ** nodeArray = malloc(sizeof(Internal_Node *) * num_children);

#ifdef RD_DEBUG
    assert(current_node != NULL);
    assert(num_children > 1);
#endif

    /// Get the left most base node and store it in current_node
    while (current_node->lvl > 0)
    {
		if (current_node->left)
		    current_node = current_node->left;
		else
		{
			printf ( "Debug: our two child invariant failed!\n" );
		    current_node = current_node->right;
		}
    }

    /// Add leftmost node to the array
    nodeArray[i++] = current_node;

#ifdef RD_DEBUG
    assert(current_node->lvl == 0);
#endif

    /// Iterate from the last Bottom_List storing the internal nodes in nodeArray
    while (i < num_children)
    {
	rebuild_skip_count++;
#ifdef RD_DEBUG
		assert(current_node != NULL);
#endif

		/// If new Bottom_List doesn't yet have an Internal_Node
		if ( !(current_node->bl->next->internal) )
		{
			Internal_Node * new_node = malloc(sizeof(Internal_Node));

		    //We need to allocate space for it's internal node
		    current_node->bl->next->internal = new_node;
		    new_node->lvl = 0;
			new_node->left = new_node->right = NULL;
			new_node->base = current_node->bl->next->tag;

		    // Assign the internal node's bl pointer to the bl to be inserted
		    new_node->bl = current_node->bl->next;

			current_node = new_node;
		}
		else
		{
			/// Update the current node to iterate horizontally through base nodes
			current_node = current_node->bl->next->internal;
		}

		/// Add node to the array and increment i
		nodeArray[i++] = current_node;

#ifdef RD_DEBUG
		assert(current_node->lvl == 0);
#endif

    }

    return nodeArray;
}

static int rebalance_count = 0;
void print_rebalance_count(){

	printf ( "Rebalance count:%i\n", rebalance_count );
}

static double rebalance_total_time = 0;
void print_rebalance_timing(){

	printf ( "rebalance total time(s):%f \n", (rebalance_total_time  / CLOCKS_PER_SEC));
}

/*!
 * ===  FUNCTION  ======================================================================
 *         Name:  rebalance_tl
 *  Description:  Rebalances the top list around a pivot node according to Bender's
 *  												  algorithm.
 * =====================================================================================
 */
void rebalance_tl (Bottom_List * pivot){

#ifdef RD_TIMING
	clock_t temp_clock = clock();
#endif


#ifdef RD_STATS
	Top_List * list = pivot->parent;
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
    Internal_Node *current_node = pivot->internal, * temp; ///< temp for rebuild set-up

    /// Constants used to calculate when to rebalance
    double overflow_density, overflow_threshold, i = -1;
    unsigned int current_tag_range = 1, current_tree_lvl = 0, lvl_dif = 0;

	rebalance_count++;

#ifdef RD_DEBUG
	/// Make sure the bottom_lists are in order before everything
	check_sub_correctness(pivot->parent);
#endif

    /// Continually iterate up levels until threshold passes for space needed
    do
    {

#ifdef RD_DEBUG

		if (current_node == NULL)
			printf("Pivot is NULL\n");
		assert(current_node != NULL);
#endif
		if (current_node->lvl == INT_BIT_SIZE)
		{
		    /// Need to break because it has reached the root
		    break;
		}


		current_node = current_node->parent;

		/// Update the lvl difference
		lvl_dif = current_node->lvl - current_tree_lvl;

		/// Update the tree level
		current_tree_lvl = current_node->lvl;

		/// Bit Shift same number of places as level changes
		current_tag_range = current_tag_range << lvl_dif; ///< Same as doubling once for every level upward

		/// NOTE: This would have current_tree_lvl - 1 if the current_tree_lvl++ were before this line.
		overflow_threshold = pow(OVERFLOW_CONSTANT, -1.0 * (current_tree_lvl));

		/// NOTE: This is + 1 because we still need to insert the needed node in the tag range
		overflow_density = ((double)current_node->num_children + 1) / ((double)current_tag_range);

    }
    while ( (overflow_density > overflow_threshold ) && (current_node->lvl <= INT_BIT_SIZE) );

	Internal_Node ** nodeArray = malloc(sizeof(Internal_Node *) * current_node->num_children);

	par_build_array_from_rebalance_list(nodeArray, current_node, 0, current_node->num_children -1);

#ifdef RD_DEBUG
    assert(current_node->num_children > 1); ///< Minimum is 2 nodes
#endif

#ifdef RD_SIZE_REBALANCE
	printf("%i\n",current_node->num_children);
#endif

	i = rebuild_tree_count;
    rebuild_tree(current_node, nodeArray, 0, current_node->num_children - 1);

#ifdef RD_DEBUG
	check_tree_correctness(current_node);
	check_sub_correctness(pivot->parent);
#endif

    /// Free the array we created
    free(nodeArray);

#ifdef RD_TIMING
	rebalance_total_time += (double)(clock() - temp_clock);
#endif
}

static unsigned int remove_scaffolding_count = 0;
void print_remove_count()
{
	printf ( "Remove scaffolding: %i\n", remove_scaffolding_count );
}

void remove_scaffolding(Internal_Node * node)
{
	remove_scaffolding_count++;

	if (node->left && node->left->lvl > 0)
	{
		remove_scaffolding(node->left);
	}
	if (node->right && node->right->lvl > 0)
	{
		remove_scaffolding(node->right);
	}
	free(node);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  rebuild_tree
 *  Description:  Recursively calls rebuild on it's children. If lvl 2, then assign children.
 *  						  If in the right, start counting from the startIndex.
 *  						  If in the left, stop counting 1 before the endIndex.
 * =====================================================================================
 */
void rebuild_tree (Internal_Node * current_node, Internal_Node ** nodeArray, int startIndex,  int endIndex)
{
    Internal_Node * new_child;
    int num_children = (endIndex - startIndex) + 1;
    rebuild_tree_count++;

    /// If there is only one node, simply make it the left child of the current_node
    if (num_children == 1)
    {
		/*
		 * If there are already left and right children, it is because they are Internal_Nodes
		 * in the array that will be moved. So don't worry about them, and update the children
		 * accordingly
		 */
		if (current_node->left && current_node->left->lvl > 0)
		{
			remove_scaffolding(current_node->left);
		}
		if (current_node->right && current_node->right->lvl > 0)
		{
			remove_scaffolding(current_node->right);
		}

		current_node->left = nodeArray[startIndex];

		/// Update the parent pointer
		current_node->left->parent = current_node;

		/// Update base/tag
		current_node->left->base = current_node->left->bl->tag = current_node->base;

		/// Make sure num_children is correct
		current_node->num_children = 1;

		/// NOTE: Don't need to free the node already there - will be moved through array
		current_node->right = NULL;
    }
    else if (num_children == 2)
    {
		/*
		 * If there are already left and right children, it doesn't matter.
		 * Since our implementation doesn't include intermediate Internal_Node(s)
		 * we can simply make the left and right children of current_node the startIndex
		 * and endIndex respectively.
		 */
		if (current_node->left && current_node->left->lvl > 0)
		{
			remove_scaffolding(current_node->left);
		}
		if (current_node->right && current_node->right->lvl > 0)
		{
			remove_scaffolding(current_node->right);
		}

		current_node->left = nodeArray[startIndex];
		current_node->right = nodeArray[endIndex];

		/// Update Parent pointer
		current_node->left->parent = current_node;
		current_node->right->parent = current_node;

		/// Update the bases of the children/leaves
		current_node->left->base  = current_node->left->bl->tag = current_node->base;

		//If int bit size is the highest size, then we can't shift over that many levels or we lose all data
		if (current_node->lvl == INT_BIT_SIZE)
			current_node->right->base = current_node->right->bl->tag  = (1 << (INT_BIT_SIZE -1));
		else
			current_node->right->base = current_node->right->bl->tag  = current_node->base | (1 << (current_node->lvl - 1));

		/// Update current_node's children
		current_node->num_children = 2;
    }
    else ///< num_children >= 3 => split and recurse
    {
    	/// Flags
    	int rebuild_left_flag = 1, rebuild_right_flag = 1;

		/// First get the new indices of the split
		int leftStart = startIndex; ///< 0 is the first index in the array
		int leftEnd = startIndex + ((num_children - 1) >> 1); ///< If odd, will be floor of half the index range
		int rightStart = leftEnd + 1; ///< RightHalf starts immediately after LeftHalf
		int rightEnd = endIndex; ///< Index of last Internal_Node in array

		int num_children_left = (leftEnd - leftStart) + 1, num_children_right = (rightEnd - rightStart) + 1; ///< For this case (num_children >= 3)

		/// Update the number of children for the current_node
		current_node->num_children = num_children;

#ifdef RD_DEBUG
		assert(num_children_left >= 1 && num_children_right >= 1);
#endif

		/// Make a left node if it is null
		if (num_children_left == 1){///< Connect this node to the leaf

				/// Clear scaffolding
				if (current_node->right && current_node->right->lvl > 0)
					remove_scaffolding(current_node->right);

				/// Assign the left child from the node array
				current_node->left = nodeArray[leftStart];

				/// Assign the cur node to the left child as its parent
				current_node->left->parent = current_node;

				/// Assign its base/tag -- fill the remaining spaces with 1's
				current_node->left->base = current_node->left->bl->tag = current_node->base;

				/// We can stop the rebuild:
				rebuild_left_flag = 0;
			}
		else if (current_node->left == NULL) ///< We need an internal node to contain more than 1 node
		{ 
#ifdef RD_DEBUG
				/// If we have lvl 1 or 0, then we messed up because we have 3 children
				assert(current_node->lvl > 1);
#endif
				/// Allocate space for left internal node
				current_node->left = malloc(sizeof(Internal_Node));

				/// Update the new child lvl
				current_node->left->lvl = current_node->lvl -1;
				current_node->left->bl = NULL;

				/// Update the base
				current_node->left->base = current_node->base;

				/// Update cur node->left parent reference
				current_node->left->parent = current_node;

				current_node->left->left = current_node->left->right = NULL;
			}

		/// Now set-up the left-half and rebuild
		else if ((current_node->lvl - current_node->left->lvl) != 1) ///< If the cur->left isn't one level below it
		{
		    if (current_node->left->lvl == 0)
		    {
				if(num_children_left == 1)
				{
				    current_node->left = nodeArray[leftStart];
				    current_node->left->parent = current_node;

				    /// Update base/tags
				    current_node->left->base = current_node->left->bl->tag = current_node->base;
					rebuild_left_flag = 0;
				}
				else ///< The number of children on the left is > 1
				{
				    /// Need a new Internal_Node
				    new_child = malloc(sizeof(Internal_Node));

				    /// Update the pointers to the left
				    current_node->left = new_child;
				    new_child->parent = current_node;

					/// Null the pointer to the right and left
					/// Since the old left node was a leaf node, we won't lose track of it
					/// also nullify bl
				    new_child->left = new_child->right = NULL;
				    new_child->bl = NULL;

				    /// Update the new Internal_Node's level
				    new_child->lvl = current_node->lvl - 1;

				    /// Update this new node's num_children
				    current_node->left->num_children = num_children_left;

				    /// Update the new node's base
				    new_child->base = current_node->base;
				}
		    }
		    else ///<If the cur->left is more than one level below cur and cur->left is an internal node
		    {
				/// We Need to move up the left node
				/// Update lvl
				current_node->left->lvl = current_node->lvl - 1;
				current_node->left->base = current_node->base;
		    }
		}
		///< If the left node is already one level below the current_node, do nothing.
		if (rebuild_left_flag)
			rebuild_tree(current_node->left, nodeArray, leftStart, leftEnd);


		// Make a right node if it is null
		if (num_children_right == 1) ///< Connect this node to the leaf
		{
				if (current_node->right && current_node->right->lvl > 0)
					remove_scaffolding(current_node->right);

				/// Assign the right child from the node array
				current_node->right = nodeArray[rightStart];

				/// Assign the cur node to the right child as its parent
				current_node->right->parent = current_node;

				/// Assign its base/tag -- fill the next space with a 1
				if (current_node->lvl == INT_BIT_SIZE)
				{
					current_node->right->base = current_node->right->bl->tag = (1 << (INT_BIT_SIZE - 1));
				}
				else
					current_node->right->base = current_node->right->bl->tag = current_node->base | (1 << (current_node->lvl-1));

				//We can stop the rebuild:
				rebuild_right_flag = 0;
			}
		else if (current_node->right == NULL) /// We need an internal node to contain more than 1 node
		{ 
#ifdef RD_DEBUG
				/// If we have lvl 1 or 0, then we messed up because we have 3 children
				assert(current_node->lvl > 1);
#endif
				/// Allocate space for right internal node
				current_node->right = malloc(sizeof(Internal_Node));

				/// Update the new child lvl
				current_node->right->lvl = current_node->lvl -1;

				/// Update the base (since current_node->right->lvl is less than INT_BIT_SIZE we won't lose all this information if we shift left)
				current_node->right->base = current_node->base | (1 << current_node->right->lvl);

				/// Update cur node->right parent reference
				current_node->right->parent = current_node;

				current_node->right->right = current_node->right->left = NULL;
				current_node->right->bl =  NULL;
		}
		/// Now set-up the right-half and rebuild
		else if ((current_node->lvl - current_node->right->lvl) != 1) ///< If the cur->right isn't one level below it
		{
		    if (current_node->right->lvl == 0)
		    {
				if(num_children_right == 1)
				{
				    current_node->right = nodeArray[rightStart];
				    current_node->right->parent = current_node;

				    /// Update base/tags  (make the child the furthest left child of the right subtree)
					current_node->right->base = current_node->right->bl->tag = current_node->base | (1 << (current_node->lvl -1));

				    //Alex: no need to rebuild
					rebuild_right_flag = 0;
				}
				else ///< The number of children on the right is > 1
				{
				    /// Need a new Internal_Node
				    new_child = malloc(sizeof(Internal_Node));

				    /// Update the pointers to the right
				    current_node->right = new_child;
				    new_child->parent = current_node;

					// Null the pointer to the left/right/it's bl
				    new_child->left = new_child->right = NULL;
				    new_child->bl = NULL;

				    /// Update the new Internal_Node's level
				    new_child->lvl = current_node->lvl - 1;

				    /// Update this new node's num_children
				    new_child->num_children = num_children_right;

				    /// Update the new node's base
				    new_child->base = current_node->base | (1 << new_child->lvl);
				}
		    }
		    else ///<If the cur->right is more than one level below cur and cur->right is an internal node
		    {
				/// We Need to move up the right node
				/// Update lvl
				current_node->right->lvl = current_node->lvl - 1;
				current_node->right->base = current_node->base | (1 << current_node->right->lvl);
		    }
		}


		///< If the right node is already one level below the current_node, do nothing.
		if (rebuild_right_flag)
			rebuild_tree(current_node->right, nodeArray, rightStart, rightEnd);
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
				printf("Node tags are out of order; cur:[%p|%i] cur->next:[%p|%i]\n", cur_node, cur_node->tag, cur_node->next,  cur_node->next->tag);
		    cur_node = cur_node->next;
		}
		if (current->next && current->tag >=current->next->tag )
		    printf ( "Bottom list tags are out of place; cur:[B:%p|I:%p|%i] cur->next:[B:%p|I:%p|%i]\n", current, current->internal, current->tag, current->next, current->next->internal, current->next->tag );
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
typedef struct print_node_s{
    Internal_Node *data;
    struct		print_node_s *next;
}print_node;

int print_nodes_in_order(print_node * first, print_node *second){
    if (first->data->lvl < second->data->lvl)
		return 0;
    else if (first->data->lvl == second->data->lvl){
    	return first->data->base < second->data->base;
    }
    else // >
    {
		return 1;
    }
}
void append_and_sort(print_node * current, print_node *to_add)
{
    print_node *trailing = current;

    while ((current != NULL) && (print_nodes_in_order(current, to_add))){
		trailing = current;
		current = current->next;
    }

    to_add->next = trailing->next;
    trailing->next = to_add;

}
print_node * pop_print_node(print_node ** head){
    print_node *tmp = *head;
    if(!tmp)
		printf ( "No node to pop!\n" );
    // move head
    *head = (*head)->next;
    free(tmp);

    return *head;
}
void print_tree (Top_List * list)
{
    print_node *head, *current = malloc(sizeof(print_node));
    Internal_Node *iter, *trailing;

    int i = 0, tailIndex = 1, current_lvl;

    printf ( "Tree size: %i\n", list->size );
    iter = list->head->internal;
    while (iter!= NULL){
		trailing = iter;
		iter = iter->parent;
    }
    current->data = trailing;
    current->next = NULL;
    current_lvl = trailing->lvl;
    head = current;


    //head/current are the root


    printf ( "\n\nLevel: %i ", current_lvl );
    while (current)
    {
		if (current->data->left)
		{
		    print_node *left = malloc(sizeof(Internal_Node));
		    left->data = current->data->left;
		    append_and_sort(current, left);
		}
		if (current->data->right)
		{
		    print_node *right = malloc(sizeof(Internal_Node));
		    right->data = current->data->right;
		    append_and_sort(current, right);
		}

		printf(" --- (loc: %p|p: %p|lvl:%i|L:%p R:%p|base:%u) ---", current->data, current->data->parent, current->data->lvl, current->data->left, current->data->right, current->data->base);
		current = pop_print_node(&current);

		if (current == NULL)
		    break;
		if (current->data->lvl != current_lvl)
		{
		    current_lvl = current->data->lvl;
		    printf ( "\nLevel: %i", current_lvl );
		}

    }

    printf ( "\n\nEND TREE\n" );

}

























/* OLD REBUILD
   int newStartIndex, newEndIndex, diff = endIndex - startIndex;
   if (current_node == NULL )
   return;
   else
   current_node->num_children = diff + 1;
   #ifdef RD_DEBUG
   if (current_node->lvl < 2){
   printf ( "Debug: rebuild tree lvl is too small.\n" );
   assert(0);}
   #endif
   if (current_node->lvl == 2){
   #ifdef RD_DEBUG
   printf ( "In a lvl 2 node\n" );
   #endif
   if (diff == -1){
   current_node->left = NULL;
   current_node->right = NULL;}
   else if (diff == 0){
   current_node->left = nodeArray[startIndex];
   current_node->left->parent = current_node;
   current_node->left->base = current_node->left->bl->tag = current_node->base;
   current_node->right = NULL;}
   else if (diff == 1){
   /// Left node
   current_node->left = nodeArray[startIndex];
   current_node->left->parent = current_node;
   current_node->left->base = current_node->left->bl->tag = current_node->base;
   /// Right node
   current_node->right = nodeArray[endIndex];
   current_node->right->parent = current_node;
   current_node->right->base = current_node->right->bl->tag = current_node->base + 1;}
   else{
   #ifdef RD_DEBUG
   printf ( "Debug: error too many nodes given to this internal node.\n" );
   #endif
   }
   #ifdef RD_DEBUG
   printf ( "My num children %i, my indexes [%i %i].\n", current_node->num_children, startIndex, endIndex );
   #endif
   return;}
   else { // lvl > 2
   #ifdef RD_DEBUG
   printf ( "Indexes before rebuild calls (num_child: %i lvl: %i) : [%i %i]", current_node->num_children,current_node->lvl, startIndex, endIndex);
   #endif
   //Parallel: Get new start and end indexes
   if (startIndex > endIndex){ // no children
   // Left subarray
   startIndex = 1;
   newEndIndex = 0;
   // Right subarray
   newStartIndex = 1;
   endIndex  = 0;}
   else if (startIndex == endIndex) // one child{
   newEndIndex = endIndex;
   //Dont use right sub array
   newStartIndex = 1;
   endIndex = 0;}
   else {
   //startIndex stays the same
   //so does endIndex
   if ( (diff & 0x1 ) == 0x1) // if endIndex - startIndex is odd
   newEndIndex = startIndex + ((endIndex - startIndex ) / 2 );
   else
   newEndIndex = startIndex + ((endIndex - startIndex + 1) / 2 );
   newStartIndex = newEndIndex + 1;}
   #ifdef RD_DEBUG
   printf ( "After: [%i %i] [%i %i]\n", startIndex,newEndIndex,newStartIndex, endIndex );
   #endif
   if (startIndex <= newEndIndex){
   if (!(current_node->left)){
   current_node->left = malloc(sizeof(Internal_Node));
   current_node->left->base = current_node->base;
   current_node->left->parent = current_node;
   current_node->left->lvl = current_node->lvl -1;
   // num children taken care of in rebuild}
   rebuild_tree(current_node->left, nodeArray, startIndex, newEndIndex);}
   if (newStartIndex <= endIndex ){
   if(!(current_node->right) ){
   current_node->right = malloc(sizeof(Internal_Node));
   current_node->right->base = current_node->base;
   current_node->right->parent = current_node;
   current_node->right->lvl = current_node->lvl -1;
   // num children taken care of in rebuild}
   rebuild_tree(current_node->right, nodeArray, newStartIndex, endIndex);}} //end else
*/

/*//PREVIOUS VERSION of SPLIT_BL
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
