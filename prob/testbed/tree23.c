// tree23.c
// Robert Utterback
// 29-01-2013

#include "tree23.h"

tree23_t* new23()
{

}

void free23(tree23_t* t)
{

}

node23_t* search23(tree23_t* t, element e)
{
	node23_t *next = t->root;
	while(next != NULL && next->mVal != NULL) {
		if (e <= next->lVal) {
			next = next->l;
		} else if (e <= next->mVal) {
			next = next->m;
		} else {
			next = next->r;
		}
	}
	if (next == NULL) next = next->p->m; // the last leaf in the tree
	if (e < next->lVal) next = NULL; // e is lower than everything
	return next;
}

void insert23(tree23_t* t, element e)
{
	node23_t *leaf = search23(t, e);
	if (leaf->lVal == e) return;
	node23_t *newNode;
	newNode->lVal = e;
	newNode->mVal = newNode->rVal = newNode->l = newNode->m = newNode->r = NULL;

	node23_t *parent = leaf->p;
	while(parent->r != NULL) { // too many children!
		node23_t *q;
		newNode->p = parent;
		if(newNode->lVal < parent->mVal) { 
			q->l = parent->m;
			q->m = parent->r
			parent->m = newNode;
		} else if (newNode->lVal < parent->rVal) {
			q->l = newNode;
			q->m = parent->r;
		} else {
			q->l = parent->r;
			q->m = newNode;
		}
		q->l->p = q;
		q->m->p = q;

		parent->lVal = parent->l->rVal;
		parent->mVal = parent->m->rVal;
		parent->rVal = NULL;
		parent->r = NULL;
		q->lVal = q->l->rVal;
		q->mVal = q->m->rVal;
		q->rVal = NULL;
		q->r = NULL;

		newNode = q;
		parent = parent->p;
	}
	// now we know r is NULL
	if (newNode->lVal < parent->mVal) { // put as middle child
		parent->r = parent->m;
		parent->m = newNode;
	} else { // put as right child
		parent->r = newNode;
	}
}

void delete23(tree23_t* t, element e)
{

}
