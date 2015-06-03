#ifndef OM_SUBLIST_H
#define OM_SUBLIST_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "OM_DS.h"
#include "OM_DS_bender.h"

#define HEBREW_ID 10
#define ENGLISH_ID 11
/*alex: try this
 *unsigned long MAX_NUMBER;
 *unsigned long INT_BIT_SIZE;
 *
 */

Bottom_List * Bottom_List_init();

void OM_free_and_free_nodes(Bottom_List *);

void printList(Bottom_List *, const int);

void Split_and_add_to_top(Top_List * tlist, Bottom_List * blist);

void Rebalance_bottom_lists(Top_List * list);

int Bottom_List_insert(OM_Node *, OM_Node*, const int);

void Bottom_List_add_first_node(Bottom_List*, OM_Node *, const int ID);

int Bottom_List_order( OM_Node *, OM_Node *, const int);

void check_sub_correctness(Top_List * list);

#endif
