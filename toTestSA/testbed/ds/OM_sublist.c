#include "OM_sublist.h"
#include "OM_DS_bender.h"


/// Allocate memory and set variables
void * OM_DS_init(){

	OM_DS * list;

	MAX_NUMBER = ~0;
	;//printf("Max # in init: %u\n", MAX_NUMBER);

	list = (OM_DS*)malloc(sizeof(OM_DS));
	list->head = (OM_Node*)malloc(sizeof(OM_Node));
	list->tail = (OM_Node*)malloc(sizeof(OM_Node));

	// Set up list
	list->head->next_e = list->head->next_h = list->tail;
	list->tail->prev_e = list->tail->prev_h = list->head;

	list->size = list->Reorder_flag = 0;

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

void Split_and_add_to_top(Top_List * tlist, OM_DS * blist) {

	OM_Node * current_e, * current_h;
	int temp_e = 0, temp_h = 0;

	/// New list to be inserted on top
	OM_DS * to_add = OM_DS_init();

	INT_BIT_SIZE =  32;

	current_e = current_h = blist->head;

	/// ===== ENGLISH Case =====
	/// First find middle
	do {
		current_e = current_e->next_e;
		++temp_e;
	} while(temp_e < blist->size/2 );
	
	/// This is the procedure:
	/// === update to_add tail ptrs ===
	/// to_add->tail->prev_e = blist->tail->prev_e
	/// blist->tail->prev_e->next_e = to_add->tail
	/// === update blist tail ptrs ===
	/// blist->tail->prev_e = current_e->prev_e
	/// current_e->prev_e->next_e = blist->tail
	/// === update to_add head ptrs ===
	/// to_add->head->next_e = current_e
	/// current_e->prev_e = to_add->head
	

	/// to_add->tail->prev_e = blist->tail->prev_e
	if(!(__sync_bool_compare_and_swap(&(to_add->tail->prev_e), to_add->tail->prev_e, blist->tail->prev_e)))
	{
		printf("Exiting, atomic insert (to_add tail, blist tail prev e)failed");
		exit(0);
	}

	/// blist->tail->prev_e->next_e = to_add->tail
	if(!(__sync_bool_compare_and_swap(&(blist->tail->prev_e->next_e), blist->tail->prev_e->next_e, to_add->tail)))
	{
		printf("Exiting, atomic insert (blist tail prev next, toadd tail e)failed");
		exit(0);
	}

	/// blist->tail->prev_e = current_e->prev_e
	if(!(__sync_bool_compare_and_swap(&(blist->tail->prev_e), blist->tail->prev_e, current_e->prev_e)))
	{
		printf("Exiting, atomic insert (blist tailprev, currprev e )failed");
		exit(0);
	}

	/// current_e->prev_e->next_e = blist->tail
	if(!(__sync_bool_compare_and_swap(&(current_e->prev_e->next_e), current_e->prev_e->next_e, blist->tail)))
	{
		printf("Exiting, atomic insert (curr prev next, blist tail e)failed");
		exit(0);
	}

	/// to_add->head->next_e = current_e
	if(!(__sync_bool_compare_and_swap(&(to_add->head->next_e), to_add->head->next_e, current_e)))
	{
		printf("Exiting, atomic insert (toadd head next, curr e)failed");
		exit(0);
	}
		
	/// current_e->prev_e = to_add->head
	if(!(__sync_bool_compare_and_swap(&(current_e->prev_e), current_e->prev_e, to_add->head)))
	{
		printf("Exiting, atomic insert (curr prev, toadd head e)failed");
		exit(0);
	}
	
	/// Insert into top list for english
	insert_top_list(tlist, blist, to_add, ENGLISH_ID, 0);


	/// ===== Hebrew Case =====
	/// First find middle
	do {
		current_h = current_h->next_h;
		++temp_h;
	} while(temp_h < blist->size/2 );
	
	/// This is the procedure:
	/// === update to_add tail ptrs ===
	/// to_add->tail->prev_h = blist->tail->prev_h
	/// blist->tail->prev_h->next_h = to_add->tail
	/// === update blist tail ptrs ===
	/// blist->tail->prev_h = current_h->prev_h
	/// current_h->prev_h->next_h = blist->tail
	/// === update to_add head ptrs ===
	/// to_add->head->next_h = current_h
	/// current_h->prev_h = to_add->head
	

	/// to_add->tail->prev_h = blist->tail->prev_h
	if(!(__sync_bool_compare_and_swap(&(to_add->tail->prev_h), to_add->tail->prev_h, blist->tail->prev_h)))
	{
		printf("Exiting, atomic insert (to_add tail, blist tail prev e)failed");
		exit(0);
	}

	/// blist->tail->prev_h->next_h = to_add->tail
	if(!(__sync_bool_compare_and_swap(&(blist->tail->prev_h->next_h), blist->tail->prev_h->next_h, to_add->tail)))
	{
		printf("Exiting, atomic insert (blist tail prev next, toadd tail e)failed");
		exit(0);
	}

	/// blist->tail->prev_h = current_h->prev_h
	if(!(__sync_bool_compare_and_swap(&(blist->tail->prev_h), blist->tail->prev_h, current_h->prev_h)))
	{
		printf("Exiting, atomic insert (blist tailprev, currprev e )failed");
		exit(0);
	}

	/// current_h->prev_h->next_h = blist->tail
	if(!(__sync_bool_compare_and_swap(&(current_h->prev_h->next_h), current_h->prev_h->next_h, blist->tail)))
	{
		printf("Exiting, atomic insert (curr prev next, blist tail e)failed");
		exit(0);
	}

	/// to_add->head->next_h = current_h
	if(!(__sync_bool_compare_and_swap(&(to_add->head->next_h), to_add->head->next_h, current_h)))
	{
		printf("Exiting, atomic insert (toadd head next, curr e)failed");
		exit(0);
	}
		
	/// current_h->prev_h = to_add->head
	if(!(__sync_bool_compare_and_swap(&(current_h->prev_h), current_h->prev_h, to_add->head)))
	{
		printf("Exiting, atomic insert (curr prev, toadd head e)failed");
		exit(0);
	}
	
	/// Update sizes (utilize temp variables in a general way here)
	temp_h = blist->size;
	blist->size = temp_e;
	to_add->size = temp_h - temp_e;
	
	/// Update flags based on size
	blist->size < (INT_BIT_SIZE >> 1) ? \
		blist->Reorder_flag = 0: blist->Reorder_flag = 1;
	to_add->size < (INT_BIT_SIZE >> 1) ? \
		to_add->Reorder_flag = 0: to_add->Reorder_flag = 1;

	/// Insert into top list for hebrew
	insert_top_list(tlist, blist, to_add, HEBREW_ID, 0);

	/// Insert into top lsit for english
	insert_top_list(tlist, blist, to_add, ENGLISH_ID, 0);
}

void Rebalance_bottom_lists(Top_List * list) {

	OM_DS * current_e, * current_h;
	current_e = current_h = list->head;

	while(current_e != list->tail) {
		if(current_e->Reorder_flag == 1)
			Split_and_add_to_top(list, current_e);
		current_e = current_e->next_e;
	}

	while(current_h != list->tail) {
		if(current_h->Reorder_flag == 1)
			Split_and_add_to_top(list, current_h);
		current_h = current_h->next_h;
	}

}



int OM_DS_insert(OM_DS *ds, OM_Node * x, OM_Node * y, const int ID){

	INT_BIT_SIZE =  32;

	//if x is null
	if (!(x && y && ds) ){
		printf("Some node or ds is null,\
               skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n",
			   x->id, x, y->id, y, ds->tail->id, ds->tail);
		return;
	}

	switch(ID){
	case HEBREW_ID:
		
		/// This is the procedure:
		/// y->next = x->next_h
		/// x->next_h->prev_h = y
		/// x->next_h = y
		/// y->prev_h = x
		
		// y->next = x->next_h
		if(!(__sync_bool_compare_and_swap(&(y->next_h), y->next_h, x->next_h)))
		{
			printf("Exiting, atomic insert (heb, next2)failed");
			exit(0);
		}

		// x->next_h->prev_h = y
		if(!(__sync_bool_compare_and_swap(&(x->next_h->prev_h), x->next_h->prev_h, y)))
		{
			printf("Exiting, atomic insert (heb, prev2)failed");
			exit(0);
		}

		// x->next_h = y
		if(!(__sync_bool_compare_and_swap(&(x->next_h), x->next_h, y)))
		{
			printf("Exiting, atomic insert (heb, next)failed");
			exit(0);
		}

		// y->prev_h = x
		if(!(__sync_bool_compare_and_swap(&(y->prev_h), y->prev_h, x)))
		{
			printf("Exiting, atomic insert (heb, prev)failed");
			exit(0);
		}
		
		/// Assign y's tag
		y->tag_h = (y->next_h->tag_h + y->prev_h->tag_h) / 2;

		break;

	case ENGLISH_ID:

		/// This is the procedure:
		/// y->next = x->next_e
		/// x->next_e->prev_e = y
		/// x->next_e = y
		/// y->prev_e = x
		
		// y->next = x->next_e
		if(!(__sync_bool_compare_and_swap(&(y->next_e), y->next_e, x->next_e)))
		{
			printf("Exiting, atomic insert (eng, next2)failed");
			exit(0);
		}

		// x->next_e->prev_e = y
		if(!(__sync_bool_compare_and_swap(&(x->next_e->prev_e), x->next_e->prev_e, y)))
		{
			printf("Exiting, atomic insert (eng, prev)failed");
			exit(0);
		}

		// x->next_e = y
		if(!(__sync_bool_compare_and_swap(&(x->next_e), x->next_e, y)))
		{
			printf("Exiting, atomic insert (eng, next)failed");
			exit(0);
		}
		
		// y->prev_e = x
		if(!(__sync_bool_compare_and_swap(&(y->prev_e), y->prev_e, x)))
		{
			printf("Exiting, atomic insert (eng, prev)failed");
			exit(0);
		}

		/// Assign y's tag
		y->tag_e = (y->next_e->tag_e + y->prev_e->tag_e) / 2;

		break;

	}

	if( !(ds->size < (INT_BIT_SIZE >> 1) ) )
		ds->Reorder_flag = 1;

	ds->size++;

	if(ds->size == INT_BIT_SIZE)
		return 1; ///< Needs to be split
	return 0; ///< Doesn't needs immediately split

}

/// This is called in the initialization of cilk
/// Add the first node to the OM_DS
void OM_DS_add_first_node(void *ds, void * _x){

	/// Enter if ds and x are not NULL
	if (ds && _x){
		OM_DS * om_ds = (OM_DS *)ds;
		OM_Node * node = (OM_Node*)_x;
		if (om_ds->size == 0)
		{
			/// Change head->next to be this node
			om_ds->head->next_e = om_ds->head->next_h = node;

			/// Change node->prev to be the head
			node->prev_e = node->prev_h = om_ds->head;
			
			/// Change node->next to be tail
			node->next_e = node->next_h = om_ds->tail;

			/// Change tail->prev to be this node
			om_ds->tail->prev_e = om_ds->tail->prev_h = node;

			/// Assign unique node id
//			node->id =global_node_count++;

			/// Assign tag
			node->tag_e = node->tag_h = (om_ds->tail->tag_h >> 1);

			/// Increment size of linked list
			om_ds->size++;
		}
		else 	{
			/// Debug code
			/// If linked list has nodes already, exit. Don't let this be called
			/// incorrectly and let code continue
			printf("List is non-empty, dont call add first node\n");
			exit(0);
		}
	}
	else {
		printf("Debug: appending null node or to null ds\n");
	}

}

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
