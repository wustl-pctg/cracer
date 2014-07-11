#include	<assert.h>
#include "OM_sublist.h"

unsigned long MAX_NUMBER = ~0;
/*unsigned long MAX_NUMBER_HALF; [> = MAX_NUMBER >> 1;<]*/
/*TEMP: need to get off machine*/ 
unsigned int INT_BIT_SIZE = 64;

/// Allocate memory and set variables
Bottom_List * Bottom_List_init(){

	Bottom_List * list;

	/*alex : try this
	 * MAX_NUMBER = ~0;*/
	;//printf("Max # in init: %u\n", MAX_NUMBER);

	list = (Bottom_List*)malloc(sizeof(Bottom_List));
	list->head = (OM_Node*)malloc(sizeof(OM_Node));
	list->tail = (OM_Node*)malloc(sizeof(OM_Node));

	// Set up list
	list->head->next_e = list->head->next_h = list->tail;
	list->tail->prev_e = list->tail->prev_h = list->head;

	list->size_e = list->size_h = list->Reorder_flag_h = list->Reorder_flag_e = 0;

	// Set up nodes
	list->head->tag_e = list->head->tag_h = 0;
	list->tail->tag_e = list->tail->tag_h = MAX_NUMBER;
	;//printf("Debug: list->tail->tag_e & id: %u & %i\n", list->tail->tag_e, list->tail->id);
	
	return list;
}

/// Frees the nodes of the linked list
/// Since both linked lists contain the same nodes, it is sufficient
/// to move through the english list only.
void OM_free_and_free_nodes(Bottom_List * list){

	/// Create tmp nodes
	OM_Node  * node, *nextNode;

	/// Assign node to head of english list
	node = list->head;

	/// Move through english, freeing each node.
	while(node != list->tail){
		nextNode = node->next_e;
		free(node);
		node = nextNode;
	}
	free(list->tail);
	free(list);
}


/// Prints the linked list in english or hebrew ordering depending
/// on the ID parameter passed in.
void printList(Bottom_List * list, const int ID) {
    OM_Node * n;

    if (list && list->head)
        n = list->head;
    else
		exit(10);
	if( ID == HEBREW_ID )
		printf("Hebrew : Head->");
	else
		printf(" English: Head->");

    while (n != NULL){
		if( ID == HEBREW_ID ) {
			printf("%d (%u)->", n->id, n->tag_h);
        	n = n->next_h;
		}
		else {
			printf("%d (%u)->", n->id, n->tag_e);
			n = n->next_e;
		}
    }
    printf("Tail\n");
}

/// Function that splits a sublist in half and adds second
/// half to top list as a new sublist
void Split_and_add_to_top(Top_List * tlist, Bottom_List * blist) {
	/// ***** Alex's version ***** ///
	
	Bottom_List * to_add = Bottom_List_init();/*Init_bottom_list();*/
	
	int node_count_e = 1, node_count_h = 1;
	/// English
	if (blist->size_e > (MAX_NUMBER >> 1 )){
		unsigned long skip_size = (MAX_NUMBER - 1) / (unsigned long)((blist->size_e >> 1) + 1) ;
		

		/// Assign first node tag
		OM_Node * current_e = blist->head->next_e;
		current_e->tag_e = skip_size;

		/// Update first half of the list
		while(node_count_e < (blist->size_e >> 1) ) {
			/// Move current node along
			current_e = current_e->next_e;
			/// Update current tage
			current_e->tag_e = current_e->prev_e->tag_e + skip_size;
			/// Update node count
			node_count_e++;
		}

		/// Holds the middle node
		/*OM_Node * middle_node = current_e;*/

		/// Update the tail of each list
		blist->tail->prev_e = current_e;/*middle_node;*/


		/// Update size of first and second lists
		to_add->size_e = blist->size_e - node_count_e;
		blist->size_e = node_count_e; 

		///Reassign head->next_e of to_add
		// and update to_add head and tail references
		current_e = current_e->next_e;
		to_add->head->next_e = to_add->tail->prev_e = current_e;
		
		/// Update current node references to prev and next
		/* don't update this yet, or we will lose the next node reference
		 * current_e->next_e = to_add->tail;
		 * */

		current_e->prev_e = to_add->head;
		
		/// Update current node reference to ds
		current_e->ds_e = to_add;

		current_e->tag_e = skip_size;
		
		while (current_e->next_e != blist->tail)
		{
			/// Move along current node
			current_e = current_e->next_e;

			/// Update current tag_e
			current_e->tag_e = current_e->prev_e->tag_e + skip_size;

			/// Update node's ds
			current_e->ds_e = to_add;
		}
		/// Update tag_e
		/*current_e->tag_e = current_e->prev_e->tag_e + skip_size;*/

		/// Update next_e pointer to the new list's tail
		current_e->next_e = to_add->tail;

		/// Update tail of to_add list
		to_add->tail->prev_e = current_e;

		

	}
	/// Hebrew
	if (blist->size_h > (MAX_NUMBER >> 1 )){
		unsigned long skip_size = (MAX_NUMBER - 1) / (unsigned long)(1+ (blist->size_h >> 1)) ;
		

		/// Assign first node tag
		OM_Node * current_h = blist->head->next_h;
		current_h->tag_h = skip_size;

		/// Update first half of the list
		while(node_count_h < (blist->size_h >> 1) ) {
			/// Move current node along
			current_h = current_h->next_h;
			/// Update current tage
			current_h->tag_h = current_h->prev_h->tag_h + skip_size;
			/// Update node count
			node_count_h++;
		}

		/// Holds the middle node
		/*OM_Node * middle_node = current_h;*/

		/// Update the tail of each list
		blist->tail->prev_h = current_h;/*middle_node;*/


		/// Update size of first and second lists
		to_add->size_h = blist->size_h - node_count_h;
		blist->size_h = node_count_h; 

		///Reassign head->next_h of to_add
		// and update to_add head and tail references
		current_h = current_h->next_h;
		to_add->head->next_h = to_add->tail->prev_h = current_h;
		
		/// Update current node references to prev and next
		/* don't update this yet, or we will lose the next node reference
		 * current_h->next_h = to_add->tail;
		 * */

		current_h->prev_h = to_add->head;
		
		/// Update current node reference to ds
		current_h->ds_h = to_add;

		current_h->tag_h = skip_size;
		
		while (current_h->next_h != blist->tail)
		{
			/// Move along current node
			current_h = current_h->next_h;

			/// Update current tag_h
			current_h->tag_h = current_h->prev_h->tag_h + skip_size;

			/// Update node's ds
			current_h->ds_h = to_add;
		}
		/// Update tag_h
		/*current_h->tag_h = current_h->prev_h->tag_h + skip_size;*/

		/// Update next_h pointer to the new list's tail
		current_h->next_h = to_add->tail;

		/// Update tail of to_add list
		to_add->tail->prev_h = current_h;



	}
	/// Reset reorder flag
	blist->Reorder_flag_e = blist->Reorder_flag_h = 0;

	/// Insert into top list
	insert_top_list(tlist, blist, to_add, ENGLISH_ID, 0, NULL);


}

/// Iterate through the top list to find sublists needing reordered
void Rebalance_bottom_lists(Top_List * list) {

	/// The iterators
	Bottom_List * current_e, * current_h;
	current_e = current_h = list->head;

	/// English
	while(current_e != list->tail) {
		if(current_e->Reorder_flag_e == 1) ///< If 1, then needs split
			Split_and_add_to_top(list, current_e);
		current_e = current_e->next_e;
	}
	;//next_echeck_sub_correctness(list);

	/// Hebrew
	while(current_h != list->tail) {
		if(current_h->Reorder_flag_h == 1) ///< If 1, then needs split
			Split_and_add_to_top(list, current_h);
		current_h = current_h->next_h;
	}
	;//check_sub_correctness(list);
	
}

/// Insert y after x into appropriate list based on ID
/// Returns 1 if full and needs reorderd immediately and 0 otherwise
int Bottom_List_insert(OM_Node * x, OM_Node * y, const int ID){

	Bottom_List * ds;

	/*alex: try thisINT_BIT_SIZE =  64;*/

	switch(ID){
	case HEBREW_ID:

		/// Retrieve the data structure known node
		ds = x->ds_h;
	
		/// Update the ds y is in 
		y->ds_h = ds;
/*
 *
 *        //if x is null
 *        if (!(x && y && ds) ){
 *            printf("Some node or ds is null,\
 *               skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n",
 *                   x->id, x, y->id, y, ds->tail->id, ds->tail);
 *            return 0 ;
 *        }
 *
 */
		
		/// This is the procedure:
		y->next_h = x->next_h;
		x->next_h->prev_h = y;
		x->next_h = y;
		y->prev_h = x;
		
		/// Assign y's tag
		y->tag_h = ((y->next_h->tag_h >> 1) + (y->prev_h->tag_h >> 1));
		
/* alex: try this
 *
		if( !(ds->size_h < (INT_BIT_SIZE >> 1) ) )
			ds->Reorder_flag_h = 1;
		if( (ds->size_h >= (INT_BIT_SIZE >> 1) ) )
			ds->Reorder_flag_h = 1;
*/
		ds->size_h++;

		if(ds->size_h == INT_BIT_SIZE)
			return 1; ///< Needs to be split
		return 0; ///< Doesn't needs immediately split

		break;

	case ENGLISH_ID:

		/// Retrieve the data structure known node
		ds = x->ds_e;
	
		/// Update the ds y is in 
		y->ds_e = ds;

		/*alex : try this
		//if x is null
		if (!(x && y && ds) ){
			printf("Some node or ds is null,\
               skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n",
				   x->id, x, y->id, y, ds->tail->id, ds->tail);
			return 1;
		}*/

		/// This is the procedure:
		y->next_e = x->next_e;
		x->next_e->prev_e = y;
		x->next_e = y;
		y->prev_e = x;
		
		/// Assign y's tag
		y->tag_e = ((y->next_e->tag_e >> 1) + (y->prev_e->tag_e >> 1));
/*alex: try this
		if( !(ds->size_e < (INT_BIT_SIZE >> 1) ) )
	  		ds->Reorder_flag_e = 1;
		if( (ds->size_e >= (INT_BIT_SIZE >> 1) ) )
	  		ds->Reorder_flag_e = 1;
*/
		ds->size_e++;

		if(ds->size_e == INT_BIT_SIZE)
			return 1; ///< Needs to be split
		return 0; ///< Doesn't needs immediately split

	}
	/// This should never occur
	assert(0/* why is this occuring here? wrong ID given to function*/);
	return 100;

}

/// This is called in the initialization of cilk
/// Add the first node to the Bottom_List
void Bottom_List_add_first_node(Bottom_List *om_ds,OM_Node  * node, const int ID){
/* alex: try this
	/// Enter if ds and x are not NULL
	if (ds && _x){
		Bottom_List * om_ds = (Bottom_List *)ds;
		OM_Node * node = (OM_Node*)_x;
*/
		switch(ID) {

		case ENGLISH_ID:

			if (om_ds->size_e == 0)
			{
				/// Change head->next to be this node
				om_ds->head->next_e = node;

				/// Change node->prev to be the head
				node->prev_e = om_ds->head;
			
				/// Change node->next to be tail
				node->next_e = om_ds->tail;

				/// Change tail->prev to be this node
				om_ds->tail->prev_e = node;

				/// Assign unique node id
//			node->id =global_node_count++;

				/// Assign tag
				/*alex: try this*/
			 node->tag_e = (om_ds->tail->tag_e >> 1);

				/// Increment size of linked list
				om_ds->size_e++;

				/// Assign node to this ds
				node->ds_e = om_ds;
			}
			else 	{
				/// Debug code
				/// If linked list has nodes already, exit. Don't let this be called
				/// incorrectly and let code continue
				printf("List is non-empty, dont call add first node\n");
				exit(0);
			}
			break;
		
		case HEBREW_ID:

			if (om_ds->size_h == 0)
			{
				/// Change head->next to be this node
				om_ds->head->next_h = node;

				/// Change node->prev to be the head
				node->prev_h = om_ds->head;
			
				/// Change node->next to be tail
				node->next_h = om_ds->tail;

				/// Change tail->prev to be this node
				om_ds->tail->prev_h = node;

				/// Assign unique node id
//			node->id =global_node_count++;

				/// Assign tag
				node->tag_h = (om_ds->tail->tag_h >> 1);
				/*node->tag_h = MAX_NUMBER_HALF;*/

				/// Increment size of linked list
				om_ds->size_h++;

				/// Assign node to this ds
				node->ds_h = om_ds;
			}
			else 	{
				/// Debug code
				/// If linked list has nodes already, exit. Don't let this be called
				/// incorrectly and let code continue
				printf("List is non-empty, dont call add first node\n");
				exit(0);
			}
			break;

		}
/*alex: try this
	}
	else {
		printf("Debug: appending null node or to null ds\n");
	}
*/
}

////TODO: fix this
/// Within the void *ds, depending on macros defined in main, determine the order
/// of x and y. If x <= y, return true. Otherwie, return false.
/// Note: the ID will determine which ordering to follow (english or hebrew)
int Bottom_List_order(/*alex: try this void *ds, */OM_Node*  x, OM_Node * y, const int ID){

/*alex: try this
	OM_Node * x, * y;
	if( !(_x && _y) ) {

		printf("Debug: Order - one of the nodes null\n");
		exit(10);
	}
	x = (OM_Node*) _x;
	y = (OM_Node*) _y;

	*/
	switch(ID){
	case HEBREW_ID:
		if(x->tag_h > y->tag_h) return 0; else return 1;

	case ENGLISH_ID:
		if(x->tag_e > y->tag_e) return 0; else return 1;
	}
	printf("Debug: something went wrong in Bottom_List_order\n");
	return 0;
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

	while (current != list->tail){

		cur_node = current->head;
		while(cur_node != current->tail) {
			
			assert(cur_node->tag_e < cur_node->next_e->tag_e);
			cur_node = cur_node->next_e;
		}

		current = current->next_e;

	}

	current = list->head;

	while (current != list->tail){

		cur_node = current->head;
		while(cur_node != current->tail) {
			
			assert(cur_node->tag_h < cur_node->next_h->tag_h);
			cur_node = cur_node->next_h;
		}

		current = current->next_h;
	}

}

