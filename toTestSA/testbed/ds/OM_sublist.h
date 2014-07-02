#ifndef OM_SUBLIST_H
#define OM_SUBLIST_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <OM_DS.h>

#define INT_BIT_SIZE(64)

void OM_DS_init();

void free_and_free_nodes(OM_DS *);

void printList(OM_DS *, const int);

void OM_DS_insert(OM_DS *, OM_Node *, OM_Node*, const int);

void OM_DS_add_first_node(void*, void*);

int OM_DS_order(void*, void*, void*, const int);



#endif
