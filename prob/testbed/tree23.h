// tree23.h
// Robert Utterback
// 29-01-2013

#ifndef TREE23_H
#define TREE23_H

typedef element int;

typedef struct tree23 {
	node23_t *root;
	int numElems;

} tree23_t;

typedef struct node23 {
	// lVal stores a leaf node's value
	element lVal,mVal,rVal;
	node23_t *l, *m, *r;

	// We could use some kind of stack instead of parent pointers
	node23_t *p;	
} node23_t;

tree23_t* new23();

void free23(tree23_t* t);

void insert23(tree23_t* t, element e);

void* search23(tree23_t* t, element e);

void delete23(tree23_t* t, element e);




#endif
