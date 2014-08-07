
#include <stdlib.h>
#include <stdio.h>
#include "OM_DS_LL.h"
#include <string.h>
#include <assert.h>
#include <time.h>

static unsigned /*long */int MAX_NUMBER = ~0;
static int INT_BIT_SIZE = 32;
static int HALF_INT_BIT_SIZE = 16;
static int lg_HALF_INT_BIT_SIZE = 4; ///< Actually lg(HALF_INT_BIT_SIZE) - 1
static double OVERFLOW_CONSTANT = 1.35;


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
 *         Name:  insert_internal
 *  Description:  Insert node y after node x in the Bottom_List specified in x->ds.
 * =====================================================================================
 */
void insert_internal(OM_Node * x, OM_Node *y)
{
	/// Retrieve the Bottom_List
	Bottom_List * ds = x->ds;

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
			
			// non cilk: insert(x, y);
			insert_internal(x, y);
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
			// non cilk: insert(x, y);
			insert_internal(x, y);
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
}



/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  first_insert_bl
 *  Description:  Insert the first OM_Node y into the Bottom_List ds.
 * =====================================================================================
 */
void first_insert_bl (Bottom_List * ds, OM_Node * y)
{
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
 *         Name:  insert
 *  Description:  Insert y after x via an InsertRecord * ir that is batched
 * =====================================================================================
 */

void insert(struct CilkWorkerState_s *const ws, OM_Node *x, OM_Node *y){

	InsertRecord *ir = Cilk_malloc(sizeof(InsertRecord));

	ir->x = x;
	ir->y = y;

	Cilk_batchify(ws, &batchInsertOp, NULL, ir, sizeof(InsertRecord), NULL);
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

		/// Assign the parent of y to the list
		y->parent = list;
		list->size += 1;
	}

}


/*! 
 * ===  FUNCTION  ======================================================================
 *         Name:  order
 *  Description:  Determines the total ordering (that is top and bottom ordering) of nodes x and y.
 *  			  Returns 1 if x < y
 *  			  Returns 0 if x > y
 * =====================================================================================
 */
void order (OM_Node * x, OM_Node * y, int *result)
{
	/// If they're in the same list, compare tags
	if (x->ds == y->ds )
	{
		if (x->tag < y->tag)
			*result = 1;
		else
			*result = 0;
	}
	
	/// Otherwise, compare their lists' tags
	else
	{
		if (x->ds->tag < y->ds->tag)
			*result = 1;
		else
			*result = 0;
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
	OM_Node * current = list_to_split->head, *transition_node;
	
	/// Create new list to add to top 
	Bottom_List * to_add, * holder;
	
	/// Keep track of num nodes and lists through iterations
	int node_count = 1, list_count = 1, num_lists_needed = (list_to_split->size >> lg_HALF_INT_BIT_SIZE);

	/// Each node in the list will be spaced out by skip_size tag spaces
	unsigned /*long*/ int skip_size = MAX_NUMBER >> lg_HALF_INT_BIT_SIZE;

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
	
	/// Constants used to calculate when to rebalance
	double overflow_density, overflow_threshold, i = -1;
	unsigned /* long */int enclosing_tag_range, lTag = 0, rTag = MAX_NUMBER, num_elements_in_sublist = 2, skip_size;

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
	skip_size = (unsigned /* long*/ int) ( enclosing_tag_range / (num_elements_in_sublist + 1) );

	assert(skip_size >0);
#ifdef RD_DEBUG
	if (rTag != MAX_NUMBER)
	{
		if (!(skip_size>0 && ((skip_size * (num_elements_in_sublist - 1)) + lList->tag <= rList->tag )))
		{
			printf("Skip_size: %u\nnum_elements:%u\n enclosing_tag_range %u\n", skip_size, num_elements_in_sublist , enclosing_tag_range);
			assert(10);
		}

	}
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
void relabel_tl_tag_range (Bottom_List *start, Bottom_List *end, const /*long*/int  skip_size)
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
#ifdef RD_DEBUG
	if(end->next)
		assert(end->tag < end->next->tag);
#endif
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
	while (current != NULL)
	{
		printf ( "(%p | %u)->", current, current->tag);
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
//POST CILK2C CODE #2

struct _cilk_batchInsertOp_frame{CilkStackFrame header;
struct{void*dataStruct;
void*data;
size_t size;
void*result;
}scope0;
struct{int i;
InsertRecord**irArray;
InsertRecord*ir;
}scope1;
};
struct _cilk_batchInsertOp_args{void*dataStruct;
void*data;
size_t size;
void*result;
};
static void _cilk_batchInsertOp_slow(CilkWorkerState*const _cilk_ws,struct _cilk_batchInsertOp_frame*_cilk_frame);
static CilkProcInfo _cilk_batchInsertOp_sig[]={{0,sizeof(struct _cilk_batchInsertOp_frame),_cilk_batchInsertOp_slow,0,0}};

void batchInsertOp (CilkWorkerState*const _cilk_ws,void*dataStruct,void*data,size_t size,void*result){struct _cilk_batchInsertOp_frame*_cilk_frame;
{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_batchInsertOp_frame), _cilk_batchInsertOp_sig);
 };
{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header));
 Cilk_cilk2c_event_new_thread_maybe(_cilk_ws);
 };

{
 int i= 0;

 InsertRecord *irArray= (InsertRecord *)data;
InsertRecord *ir;


 for (;
 i< size;
 i++) {
  ir = &irArray[i];

  insert_internal(ir->x, ir->y);

 }
{{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header));
 Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame));
 };
return;
}}}
static void _cilk_batchInsertOp_slow(CilkWorkerState*const _cilk_ws,struct _cilk_batchInsertOp_frame*_cilk_frame){void*dataStruct;
void*data;
size_t size;
void*result;
{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header));
 Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header));
 };
switch (_cilk_frame->header.entry) {}dataStruct=_cilk_frame->scope0.dataStruct;
data=_cilk_frame->scope0.data;
size=_cilk_frame->scope0.size;
result=_cilk_frame->scope0.result;

{
 int i= 0;

InsertRecord *irArray= (InsertRecord *)data;
InsertRecord*ir;


 for (;
 i< size;
 i++) {
  ir = &irArray[i];

  insert_internal(ir->x, ir->y);

 }
{{ Cilk_set_result(_cilk_ws, (void *)0, 0);
 };
{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header));
 Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame));
 };
return;
}}}
static void _cilk_batchInsertOp_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v)
{(void)_cilk_ws;
(void)_cilk_procargs_v;
batchInsertOp(_cilk_ws,((struct _cilk_batchInsertOp_args*)_cilk_procargs_v)->dataStruct,((struct _cilk_batchInsertOp_args*)_cilk_procargs_v)->data,((struct _cilk_batchInsertOp_args*)_cilk_procargs_v)->size,((struct _cilk_batchInsertOp_args*)_cilk_procargs_v)->result);

}
void mt_batchInsertOp(CilkContext*const context,void*dataStruct,void*data,size_t size,void*result)
{struct _cilk_batchInsertOp_args*_cilk_procargs;
_cilk_procargs=(struct _cilk_batchInsertOp_args*)Cilk_malloc_fixed(sizeof(struct _cilk_batchInsertOp_args));
_cilk_procargs->dataStruct=dataStruct;
_cilk_procargs->data=data;
_cilk_procargs->size=size;
_cilk_procargs->result=result;
Cilk_start(context,_cilk_batchInsertOp_import,_cilk_procargs,0);
Cilk_free(_cilk_procargs);

}

