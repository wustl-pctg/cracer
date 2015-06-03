#include "RD.h"


void * create(size_t size) 
{
	MemoryStruct * memPtr = (MemoryStruct*)malloc(sizeof(MemoryStruct));
	
	memPtr->data = malloc(sizeof(size));
	memPtr->size = size;


	return (void*)memPtr;
	
}

int OM_DS_order(void *ds, void * _x, void * _y, const int ID){

	// Really basic order function for ll
	// Assumes both _x and _y are in the list
	OM_Node * current;
	current = ((OM_DS*)ds)->head;
	if (ID != HEBREW_ID && ID != ENGLISH_ID)	
	{
		printf("ID given to order is not valid\n");
		printf("ID1: %i\t ID2: %i\t\n", ((OM_Node*)_x)->id, ((OM_Node*)_y)->id);
		printf("Invalid ID: %i\n", ID);
		printf("HEBREW_ID: %i\t ENGLISH_ID: %i\n", HEBREW_ID, ENGLISH_ID);
		exit(1);
	}
	do {
		if (current == _y)
			return 0;
		else if (current == _x)
			return 1;
		else if (ID == ENGLISH_ID)
			current = current->next_english;
		else if (ID == HEBREW_ID)
			current = current->next_hebrew;
	} while( current != ((OM_DS*)ds)->tail);

	printf("Debug: Neither node found in linked list. Returning false");

	return 0;

}

void * ReadTest(WorkerState * const ws, void * memPtr)
{
	//cast
	MemoryStruct * mem = (MemoryStruct*)memPtr;

	//! Retrieve currentNode from workerstate
	OM_Node * currentNode = ws->current_node;

	//! This is only true when it is the first read-node checked
	if( !(mem->left_r && mem->right_r) )
	{
		//!Must initalize node pointers for comparison
		mem->left_r = mem->right_r = currentNode;
		//!Only node means no race, so return
		return mem->data;
	}
	
	/*! Check if there is a race:
	 * Race if another write occurs in parallel
	 * (1)
	 *   if the currentNode is before leftmost write in eng and
	 *      the currentNode is after leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (2) or
	 *   if the currentNode is before rightmost write in eng and
	 *      the currentNode is after rightmost write in heb,
	 *      then they are in parallel => race condition
	 * (3) or
	 *   if the currentNode is after leftmost write in eng and
	 *      the currentNode is before leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (4) or
	 *   if the currentNode is after rightmost write in eng and
	 *      the currentNode is before rightmost write in heb,
	 *      then they are in parallel => race condition
	 */
	if(    //(1)
		(OM_DS_order(WS_REF_ENG, currentNode, mem->left_w,ENGLISH_ID ) &&
		 OM_DS_order(WS_REF_HEB, mem->left_w, currentNode, HEBREW_ID))
		||  //(2)
		(OM_DS_order(WS_REF_ENG, currentNode, mem->right_w, ENGLISH_ID) &&
		 OM_DS_order(WS_REF_HEB, mem->right_w, currentNode, HEBREW_ID))
		||  //(3)
		(OM_DS_order(WS_REF_ENG, mem->left_w, currentNode, ENGLISH_ID) &&
		 OM_DS_order(WS_REF_HEB, currentNode, mem->left_w, HEBREW_ID))
		||  //(4)
		(OM_DS_order(WS_REF_ENG, mem->right_w, currentNode, ENGLISH_ID) &&
		 OM_DS_order(WS_REF_HEB, currentNode, mem->right_w, HEBREW_ID))
		) { printf("READ RACE"); exit(0); } // Halt program
	//TODO ========== THROW RACE DETECTION ===== FIGURE THIS OUT

	//! Update nodes (if necessary)
	if(OM_DS_order(WS_REF_ENG, currentNode, mem->left_r, ENGLISH_ID))
		mem->left_r = currentNode;
	if(OM_DS_order(WS_REF_ENG, mem->right_r, currentNode, ENGLISH_ID))
		mem->right_r = currentNode;
	
	//return
	return mem->data;
	
}

void WriteTest(WorkerState * const ws, void * memPtr, const void *	writeValue)
{
	//cast
	MemoryStruct * mem = (MemoryStruct*)memPtr;

		
	//! Retrieve currentNode from workerstate
	OM_Node * currentNode = ws->current_node;

	//! This is only true when it is the first write-node checked
	if( !(mem->left_w && mem->right_w) )
	{
		//!Inialize ptrs for struct
		mem->left_w = mem->right_w = currentNode;
		
		//!Write the value
		memcpy(&(mem->data),&writeValue, mem->size);
		
		//!Only node so no race, return
		return;
	}
	
	/*! Check if there is a race:
	 * Race if another write/read occurs in parallel
	 * (1)   == WRITES ==
	 *   if the currentNode is before leftmost write in eng and
	 *      the currentNode is after leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (2) or
	 *   if the currentNode is before rightmost write in eng and
	 *      the currentNode is after rightmost write in heb,
	 *      then they are in parallel => race condition
	 * (3) or
	 *   if the currentNode is after leftmost write in eng and
	 *      the currentNode is before leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (4) or
	 *   if the currentNode is after rightmost write in eng and
	 *      the currentNode is before rightmost write in heb,
	 *      then they are in parallel => race condition
	 * (5) or  == READS ==
	 *   if the currentNode is before leftmost read in eng and
	 *      the currentNode is after leftmost read in heb,
	 *      then they are in parallel => race condition
	 * (6) or
	 *   if the currentNode is before rightmost read in eng and
	 *      the currentNode is after rightmost read in heb,
	 *      then they are in parallel => race condition
	 * (7) or
	 *   if the currentNode is after leftmost read in eng and
	 *      the currentNode is before leftmost read in heb,
	 *      then they are in parallel => race condition
	 * (8) or
	 *   if the currentNode is after rightmost read in eng and
	 *      the currentNode is before rightmost read in heb,
	 *      then they are in parallel => race condition
	 */
	if(    //(1)
		(OM_DS_order(WS_REF_ENG, currentNode, mem->left_w, ENGLISH_ID) &&
		 OM_DS_order(WS_REF_HEB, mem->left_w, currentNode, HEBREW_ID))
		||  //(2)
		(OM_DS_order(WS_REF_ENG, currentNode, mem->right_w, ENGLISH_ID) &&
		 OM_DS_order(WS_REF_HEB, mem->right_w, currentNode, HEBREW_ID))
		||  //(3)
		(OM_DS_order(WS_REF_ENG, mem->left_w, currentNode, ENGLISH_ID) &&
		 OM_DS_order(WS_REF_HEB, currentNode, mem->left_w, HEBREW_ID))
		||  //(4)
		(OM_DS_order(WS_REF_ENG, mem->right_w, currentNode, ENGLISH_ID) &&
		 OM_DS_order(WS_REF_HEB, currentNode, mem->right_w, HEBREW_ID))
		||  //(5)
		(OM_DS_order(WS_REF_ENG, currentNode, mem->left_r, ENGLISH_ID) &&
		 OM_DS_order(WS_REF_HEB, mem->left_r, currentNode, HEBREW_ID))
		||  //(6)
		(OM_DS_order(WS_REF_ENG, currentNode, mem->right_r, ENGLISH_ID) &&
		 OM_DS_order(WS_REF_HEB, mem->right_r, currentNode, HEBREW_ID))
		||  //(7)
		(OM_DS_order(WS_REF_ENG, mem->left_r, currentNode, ENGLISH_ID) &&
		 OM_DS_order(WS_REF_HEB, currentNode, mem->left_r, HEBREW_ID))
		||  //(8)
		(OM_DS_order(WS_REF_ENG, mem->right_r, currentNode, ENGLISH_ID) &&
		 OM_DS_order(WS_REF_HEB, currentNode, mem->right_r, HEBREW_ID))
		) { printf("WRITE RACE"); exit(0); } // Halt program
	/* ========== THROW RACE DETECTION ===== FIGURE THIS OUT */
	//TODO: Decide how to interrupt for race-detection

	//! Update nodes (if necessary)
	if(OM_DS_order(WS_REF_ENG, currentNode, mem->left_w, ENGLISH_ID))
		mem->left_w = currentNode;
	if(OM_DS_order(WS_REF_ENG, mem->right_w, currentNode, ENGLISH_ID))
		mem->right_w = currentNode;

	//! Write the data
	memcpy( mem->data, writeValue, mem->size);

}

void OM_DS_insert(OM_DS *ds, OM_Node * x, OM_Node * y, const int ID){

	//Do insert here

	//if list is empty, make head, tail, and return
	if(ds->size == 0) {
		ds->head = ds->tail = y;
		ds->head->next_english = ds->head->next_hebrew = ds->tail;
		ds->size++;
		return;
	}

	//if x is null
	if (!(x && y && ds) ){
		printf("Some node or ds is null, skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n",
			   x->id, x, y->id, y, ds->tail->id, ds->tail);
		return;
	}

	//printf("Debug: INSERT: ds:%p , x: %d , y: %d \n", ds, x->id, y->id);

	switch(ID){

	case HEBREW_ID:
		//if x->next is null, x  is tail
		if (!(x->next_hebrew))
			ds->tail = y;	

		//change next pointers
		y->next_hebrew = x->next_hebrew;

		if(!(__sync_bool_compare_and_swap(&(x->next_hebrew), x->next_hebrew, y)))
		{
			printf("Exiting, atomic insert failed");
			exit(0);
		}
	break;

	case ENGLISH_ID:
		//if x->next is null, x  is tail
		if (!(x->next_english))
			ds->tail = y;
	
		//change next pointers
		y->next_english = x->next_english;
	
		if(!(__sync_bool_compare_and_swap(&(x->next_english), x->next_english, y)))
		{
			printf("Exiting, atomic insert failed");
			exit(0);
		}

		break;
	}

	ds->size++;

}
