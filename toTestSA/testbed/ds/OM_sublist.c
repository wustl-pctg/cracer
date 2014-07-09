#include	<assert.h>
#include "OM_sublist.h"

/// Allocate memory and set variables
OM_DS * OM_DS_init(){

	OM_DS * list;

	MAX_NUMBER = ~0;
	;//printf("Max # in init: %u\n", MAX_NUMBER);

	list = (OM_DS*)malloc(sizeof(OM_DS));
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
void OM_free_and_free_nodes(OM_DS * list){

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
void printList(OM_DS * list, const int ID) {
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
void Split_and_add_to_top(Top_List * tlist, OM_DS * blist) {

	OM_Node * current_e, * current_h, * middle_e, * middle_h, *iter_node;
	int temp_e = 0, temp_h = 0;

	/// New list to be inserted on top
	OM_DS * to_add = OM_DS_init();

	INT_BIT_SIZE =  32;

	current_e = current_h = blist->head;

	/// English
	if ( blist->size_e > 1) {

		/// Iterate until middle
		while(temp_e < blist->size_e/2 ) {
			current_e = current_e->next_e;
			++temp_e;
		}

		/// Hold middle_e as the middle
		middle_e = current_e;

		/// Take middle+1 and put it in new ds
		current_e = current_e->next_e;
		iter_node = current_e->next_e;
		/// Insert first node
		OM_DS_add_first_node(to_add, current_e, ENGLISH_ID);

		/// Insert rest of second half into new ds
		while(iter_node != blist->tail) {
			/// Update iteration node (tmp) and the current node
			current_e = iter_node;
			iter_node = iter_node->next_e;

			OM_DS_insert(current_e->prev_e, current_e, ENGLISH_ID);
		}

		/// Do some maintenence on the original ds
		/// in preparation to reinsert nodes
		current_e = blist->head->next_e;
		blist->head->next_e = blist->tail;
		blist->tail->prev_e = blist->head;
		blist->size_e = 0;

		/// Add "first" node
		iter_node = current_e->next_e;

		OM_DS_add_first_node(blist, current_e, ENGLISH_ID);

		/// Reinsert until middle
		while(iter_node != middle_e) {
			current_e = iter_node;
			iter_node = iter_node->next_e;

			OM_DS_insert(current_e->prev_e, current_e, ENGLISH_ID);
		}
		current_e = iter_node;/// i.e. the middle_e node
		OM_DS_insert(current_e->prev_e, current_e, ENGLISH_ID);

		/// Update flags based on size
		if(blist->size_e < (INT_BIT_SIZE >> 1) )
			blist->Reorder_flag_e = 0;
		else blist->Reorder_flag_e = 1;

		if(to_add->size_e < (INT_BIT_SIZE >> 1) )
			to_add->Reorder_flag_e = 0;
		else to_add->Reorder_flag_e = 1;

		
	}

	/// Hebrew
	if ( blist->size_h > 1) {

		/// Iterate until middle
		while(temp_h < blist->size_h/2 ) {
			current_h = current_h->next_h;
			++temp_h;
		}

		/// Hold middle_h as the middle
		middle_h = current_h;

		/// Take middle+1 and put it in new ds
		current_h = current_h->next_h;
		iter_node = current_h->next_h;
		/// Insert first node
		OM_DS_add_first_node(to_add, current_h, HEBREW_ID);

		/// Insert rest of second half into new ds
		while(iter_node != blist->tail) {
			/// Update iteration node (tmp) and the current node
			current_h = iter_node;
			iter_node = iter_node->next_h;

			OM_DS_insert(current_h->prev_h, current_h, HEBREW_ID);
		}

		/// Do some maintenence on the original ds
		/// in preparation to reinsert nodes
		current_h = blist->head->next_h;
		blist->head->next_h = blist->tail;
		blist->tail->prev_h = blist->head;
		blist->size_h = 0;

		/// Add "first" node
		iter_node = current_h->next_h;

		OM_DS_add_first_node(blist, current_h, HEBREW_ID);

		/// Reinsert until middle
		while(iter_node != middle_h) {
			current_h = iter_node;
			iter_node = iter_node->next_h;

			OM_DS_insert(current_h->prev_h, current_h, HEBREW_ID);
		}
		current_h = iter_node;/// i.e. the middle_h node
		OM_DS_insert(current_h->prev_h, current_h, HEBREW_ID);

		/// Update flags based on size
		if(blist->size_h < (INT_BIT_SIZE >> 1) )
			blist->Reorder_flag_h = 0;
		else blist->Reorder_flag_h = 1;

		if(to_add->size_h < (INT_BIT_SIZE >> 1) )
			to_add->Reorder_flag_h = 0;
		else to_add->Reorder_flag_h = 1;

		
	}
	
	/// Insert into top list
	insert_top_list(tlist, blist, to_add, ENGLISH_ID, 0, NULL);

	/// Check if correct for the sake of being thorough
	;//check_sub_correctness(tlist);

}

/// Iterate through the top list to find sublists needing reordered
void Rebalance_bottom_lists(Top_List * list) {

	/// The iterators
	OM_DS * current_e, * current_h;
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
int OM_DS_insert(OM_Node * x, OM_Node * y, const int ID){

	OM_DS * ds;

	INT_BIT_SIZE =  32;

	switch(ID){
	case HEBREW_ID:

		/// Retrieve the data structure known node
		ds = x->ds_h;
	
		/// Update the ds y is in 
		y->ds_h = ds;

		//if x is null
		if (!(x && y && ds) ){
			printf("Some node or ds is null,\
               skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n",
				   x->id, x, y->id, y, ds->tail->id, ds->tail);
			return 0 ;
		}

		
		/// This is the procedure:
		y->next_h = x->next_h;
		x->next_h->prev_h = y;
		x->next_h = y;
		y->prev_h = x;
		
		/// Assign y's tag
		y->tag_h = ((y->next_h->tag_h >> 1) + (y->prev_h->tag_h >> 1));
		

		if( !(ds->size_h < (INT_BIT_SIZE >> 1) ) )
			ds->Reorder_flag_h = 1;

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

		//if x is null
		if (!(x && y && ds) ){
			printf("Some node or ds is null,\
               skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n",
				   x->id, x, y->id, y, ds->tail->id, ds->tail);
			return 1;
		}

		/// This is the procedure:
		y->next_e = x->next_e;
		x->next_e->prev_e = y;
		x->next_e = y;
		y->prev_e = x;
		
		/// Assign y's tag
		y->tag_e = ((y->next_e->tag_e >> 1) + (y->prev_e->tag_e >> 1));

		if( !(ds->size_e < (INT_BIT_SIZE >> 1) ) )
	  		ds->Reorder_flag_e = 1;

		ds->size_e++;

		if(ds->size_e == INT_BIT_SIZE)
			return 1; ///< Needs to be split
		return 0; ///< Doesn't needs immediately split

	}
	return 100;

}

/// This is called in the initialization of cilk
/// Add the first node to the OM_DS
void OM_DS_add_first_node(void *ds, void * _x, const int ID){

	/// Enter if ds and x are not NULL
	if (ds && _x){
		OM_DS * om_ds = (OM_DS *)ds;
		OM_Node * node = (OM_Node*)_x;

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

	}
	else {
		printf("Debug: appending null node or to null ds\n");
	}

}

////TODO: fix this
/// Within the void *ds, depending on macros defined in main, determine the order
/// of x and y. If x <= y, return true. Otherwie, return false.
/// Note: the ID will determine which ordering to follow (english or hebrew)
int OM_DS_order(void *ds, void * _x, void * _y, const int ID){

	OM_Node * x, * y;

	if( !(_x && _y) ) {

		printf("Debug: Order - one of the nodes null\n");
		exit(10);
	}
	
	x = (OM_Node*) _x;
	y = (OM_Node*) _y;

	switch(ID){
	case HEBREW_ID:
		if(x->tag_h > y->tag_h) return 0; else return 1;

	case ENGLISH_ID:
		if(x->tag_e > y->tag_e) return 0; else return 1;
	}
	printf("Debug: something went wrong in OM_DS_order\n");
	return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  check_sub_correctness
 *  Description:  Checks the list so each node's tag is less than the next node
 * =====================================================================================
 */
void check_sub_correctness (Top_List * list){
	OM_DS * current = list->head;
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

