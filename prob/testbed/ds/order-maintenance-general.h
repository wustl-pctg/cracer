/*
 * =====================================================================================
 *
 *       Filename:  order-maintenance-general.h
 *
 *    Description:  Header for same name .c
 *
 *        Version:  1.0
 *        Created:  07/15/2014 11:36:19 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (aj), atjalex@gmail.com, AND SHANE
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _ORDER_MAINTAIN_GENERAL_H
#define _ORDER_MAINTAIN_GENERAL_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

//#define RD_DEBUG

struct Bottom_List_s;

typedef struct OM_Node_s{
	struct OM_Node_s *next;
	struct OM_Node_s *prev;
	int ID;
	unsigned long int tag;
	struct Bottom_List_s * ds;

} OM_Node;

typedef struct Bottom_List_s {
	OM_Node *head,*tail;
	int size;
	int reorder_flag; 
	struct Bottom_List_s *next;
	struct Bottom_List_s *prev;

	unsigned long int tag;

} Bottom_List;

typedef struct InsertRecord_s{
	Bottom_List *ds;
	OM_Node *x;
	OM_Node *y;
	int ID;

} InsertRecord;

///bender
typedef struct Top_List_s{
	Bottom_List *head, *tail; /// TODO: change to Bottom_List of the sublist
	int size;
} Top_List;


Bottom_List * create_bl();
Top_List * create_tl();

int insert_bl(OM_Node * x, OM_Node *y, Bottom_List * ds);
void insert_tl(Top_List * list, Bottom_List *x, Bottom_List *y);
int order(OM_Node * x, OM_Node * y);
void split_bl(Top_List * list, Bottom_List * list_to_split);
void rebalance_tl(Top_List * list, Bottom_List * pivot);
void relabel_tl_tag_range(Bottom_List *start, Bottom_List *end, const long skip_size);
void rebalance_bls(Top_List * list);
void print_tl(Top_List * list);
void print_bl(Bottom_List * list);
void free_bl (Bottom_List * list);
void free_tl ( Top_List * list );
void check_sub_correctness (Top_List * list);

#endif 
