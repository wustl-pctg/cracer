#ifndef OM_SUBLIST_H
#define OM_SUBLIST_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "OM_DS.h"
#include "OM_DS_bender.h"

#define HEBREW_ID 10
#define ENGLISH_ID 11
unsigned int MAX_NUMBER;
unsigned int INT_BIT_SIZE;

OM_DS * OM_DS_init();

void OM_free_and_free_nodes(OM_DS *);

void printList(OM_DS *, const int);

void Split_and_add_to_top(Top_List * tlist, OM_DS * blist);

void Rebalance_bottom_lists(Top_List * list);

int OM_DS_insert(OM_Node *, OM_Node*, const int);

void OM_DS_add_first_node(void*, void*, const int ID);

int OM_DS_order(void*, void*, void*, const int);

void check_sub_correctness(Top_list * list);

#endif
