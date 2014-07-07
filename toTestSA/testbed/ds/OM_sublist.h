#ifndef OM_SUBLIST_H
#define OM_SUBLIST_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "OM_DS.h"
//#include "power.h"

#define HEBREW_ID 10
#define ENGLISH_ID 11
unsigned int MAX_NUMBER;

void OM_DS_init();

void OM_free_and_free_nodes(OM_DS *);

void printList(OM_DS *, const int);

void OM_DS_insert(OM_DS *, OM_Node *, OM_Node*, const int);

void OM_DS_add_first_node(void*, void*);

int OM_DS_order(void*, void*, void*, const int);



#endif
