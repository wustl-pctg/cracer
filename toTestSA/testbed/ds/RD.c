#include "RD.h"

#define HEBREW_ID(0)
#define ENGLISH_ID(1)

void * create(size_t size) 
{
	MemoryStruct * memPtr = (MemoryStruct*)malloc(sizeof(MemoryStruct));
	
	memPtr->data = malloc(sizeof(size));
	memPtr->size = size;

	return (void*)memPtr;
	
}

void * ReadTest(void * memPtr)
{
	//cast
	MemoryStruct * mem = (MemoryStruct*)memPtr;
	
	//return
	return mem->data;
	
}

void WriteTest(void * memPtr, const void *  writeValue)
{
	//cast
	MemoryStruct * mem = (MemoryStruct*)memPtr;
	
	//write
	memcpy( (mem->data), writeValue, mem->size);
	
}

void OM_DS_insert(void *ds, void * _x, void * _y, int id) {

	if(id)
	{
		//Do insert here

		OM_Node * x = (OM_Node *)_x;
		OM_Node * y = (OM_Node *)_y;
		OM_Node * z;

		//if something is null
		if (!(x && y && ds) ){
			printf("Some node or ds is null,
                       skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n",
				   x->id, x, y->id, y, ds->tail->id, ds->tail);
			return;
		}
		//printf("Debug: INSERT: ds:%p , x: %d , y: %d \n", ds, x->id, y->id);
		//if x isnt tail
		if (x->next_english)
			z = x->next_english;
		else  //make z null and change y to tail
		{
			z = NULL;
			( (OM_DS*)ds)->tail = y;
		}

		//change next pointers
		y->next_english = x->next_english;

		if(!(__sync_bool_compare_and_swap(&(x->next_english), x->next_english, y)))
		{
			printf("Exiting, atomic insert failed");
			exit(0);
		}

		((OM_DS*)ds)->size++;
	} else {

		//Do insert here

		OM_Node * x = (OM_Node *)_x;
		OM_Node * y = (OM_Node *)_y;
		OM_Node * z;

		//if something is null
		if (!(x && y && ds) ){
			printf("Some node or ds is null,
                       skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n",
				   x->id, x, y->id, y, ds->tail->id, ds->tail);
			return;
		}
		//printf("Debug: INSERT: ds:%p , x: %d , y: %d \n", ds, x->id, y->id);
		//if x isnt tail
		if (x->next_hebrew)
			z = x->next_hebrew;
		else  //make z null and change y to tail
		{
			z = NULL;
			( (OM_DS*)ds)->tail = y;
		}

		//change next pointers
		y->next_hebrew = x->next_hebrew;

		if(!(__sync_bool_compare_and_swap(&(x->next_hebrew), x->next_hebrew, y)))
		{
			printf("Exiting, atomic insert failed");
			exit(0);
		}

		((OM_DS*)ds)->size++;
	}
}


int OM_DS_order(void *ds, void * _x, void * _y, int id) {

	if(id)
	{
		// Really basic order function for ll
		// Assumes both _x and _y are in the list
		OM_Node * current;
		current = ((OM_DS*)ds)->head;

		do {
			if (current == _y)
				return 0;
			else if (current == _x)
				return 1;
			else
				current = current->next_english;
		} while( current != ((OM_DS*)ds)->tail);

		printf("Debug: Neither node found in linked list. Returning false");

		return 0;
	} else {
		// Really basic order function for ll
		// Assumes both _x and _y are in the list
		OM_Node * current;
		current = ((OM_DS*)ds)->head;

		do {
			if (current == _y)
				return 0;
			else if (current == _x)
				return 1;
			else
				current = current->next_hebrew;
		} while( current != ((OM_DS*)ds)->tail);

		printf("Debug: Neither node found in linked list. Returning false");

		return 0;
	}
	
}
