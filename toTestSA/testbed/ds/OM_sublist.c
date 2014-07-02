#include "OM_sublist.h"



/// Allocate memory and set variables
void OM_DS_init(OM_DS * list){;
	
	list = (OM_DS* )malloc(sizeof(OM_DS));
	list->head = (OM_Node*)malloc(sizeof(OM_Node));
	list->tail = (OM_Node*)malloc(sizeof(OM_Node));

	// Set up list
	list->head->next_e = list->head->next_h = tail;
	list->tail->prev_e = list->tail->prev_h = head;
	list->size = list->Reorder_flag = 0;

	// Set up nodes
	list->head->tag_e = list->head->tag_h = 0;
	list->tail->tag_e = list->tail->tag_h = UINT_MAX;

}

/// Frees the nodes of the linked list
/// Since both linked lists contain the same nodes, it is sufficient
/// to move through the english list only.
void free_and_free_nodes(OM_DS * list){

	/// Create tmp nodes
	OM_Node  * node, *nextNode;

	/// Assign node to head of english list
	node = list->head;

	/// Move through english, freeing each node.
	while(node != list->tail){
		nextNode = node->next_english;
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
			printf("%d->", n->tag_h);
        	n = n->next_hebrew;
		}
		else {
			printf("%d->", n->tag_e);
			n = n->next_english;
		}
    }
    printf("Tail\n");
}


void OM_DS_insert(OM_DS *ds, OM_Node * x, OM_Node * y, const int ID){


	//if x is null
	if (!(x && y && ds) ){
		printf("Some node or ds is null,\
               skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n",
			   x->id, x, y->id, y, ds->tail->id, ds->tail);
		return;
	}

	switch(ID){
	case HEBREW_ID:
		
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

	if( !(ds.size < INT_BIT_SIZE/2) )
		ds->Reorder_flag = 1;

	ds->size++;

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
			list->head->next_e = list->head->next_h = node;
			
			/// Change tail->prev to be this node
			list->tail->prev_e = list->tail->prev_h = head;

			/// Assign unique node id
			node->id =global_node_count++;

			/// Assign tag
			node->tag_e = node->tag_h = UINT_MAX / 2;

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

	switch(ID){
	case HEBREW_ID:
		_x->tag_h > _y->tag_h ? return 0 : return 1;

	case ENGLISH_ID:
		_x->tag_e > _y->tag_e ? return 0 : return 1;
	}
	printf("Debug: something went wrong in OM_DS_order\n");
	return 0;
}
