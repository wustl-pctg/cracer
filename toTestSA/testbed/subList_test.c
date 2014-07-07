#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "ds/OM_DS.h"
#include "ds/OM_sublist.h"


void MAL_test(int * intptr) {

	intptr = (int*)malloc(sizeof(int));
	if (intptr == NULL)
		exit(10);
	*intptr = 3;
}

void Initialize_test(OM_DS * list) {

	OM_DS_init(list);
	assert( list->head != list->tail );
	assert( list->head->next_e == list->tail );
	assert( list->head->tag_h == 0 );
	assert( list->tail->tag_h == MAX_NUMBER );
	
}


void Firstnode_test(OM_DS * list, OM_Node * x) {

	OM_DS_add_first_node(list, x);
	assert( list->size == 1);
	assert( x->tag_h == MAX_NUMBER / 2 );
	assert( list->head->next_e == x);
	assert( (OM_Node*) list->tail->prev_h == (OM_Node*) x);
}


void Insert_h_test(OM_DS * list, OM_Node * x, OM_Node * y) {

	OM_DS_insert(list, x, y, HEBREW_ID);
	assert( x->next_h == y);
	assert( (OM_Node*)y->prev_h == (OM_Node*) x);
	printf("Insert: x->tag_h: %i and y->tag_h: %i\n", x->tag_h, y->tag_h);
}

void Insert_e_test(OM_DS * list, OM_Node * x, OM_Node * y) {

	OM_DS_insert(list, x, y, ENGLISH_ID);
	assert( x->next_e == y);
	assert( (OM_Node*)y->prev_e == (OM_Node*) x);
	printf("Insert: x->tag_e: %i and y->tag_e: %i\n", x->tag_h, y->tag_h);
}





int main(int argc, char * argv[]) {

	OM_DS *list;
	OM_Node *one, *two, *three, *four, *five, *six;
	int *test;

	MAX_NUMBER = ~0;

	MAL_test(test);
	printf("test is: %i \n", *test);
	
	Initialize_test(list);

	one = (OM_Node *)malloc(sizeof(OM_Node));
	two = (OM_Node *)malloc(sizeof(OM_Node));
	three = (OM_Node *)malloc(sizeof(OM_Node));
	four = (OM_Node *)malloc(sizeof(OM_Node));
	five = (OM_Node *)malloc(sizeof(OM_Node));
	six = (OM_Node *)malloc(sizeof(OM_Node));

	Firstnode_test(list, one);

	Insert_h_test(list, one , two );
	Insert_e_test(list, one , two );

	Insert_h_test(list, two , three );
	Insert_e_test(list, one , three );

	Insert_h_test(list, three  , four );
	Insert_e_test(list, two , four );

	Insert_h_test(list, four , five );
	Insert_e_test(list, one , five );

	Insert_h_test(list, five , six );
	Insert_e_test(list, five , six );


	printf("H: 1-2-3-4-5-6\n");
	printf("E: 1-5-6-3-2-4\n");

	printList(list, HEBREW_ID);
	printList(list, ENGLISH_ID);

	OM_free_and_free_nodes(list);

	return 0;
}
