/*
 * =====================================================================================
 *
 *       Filename:  OM_DS.h
 *
 *    Description:  Contains the data structures for order maintenance data structure
 *
 *        Version:  1.0
 *        Created:  06/23/2014 03:13:03 PM
 *       Revision:  Contains linked list only
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (aj), atjalex@gmail.com & Shane Deiley
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _OM_DS_H
#define _OM_DS_H

#include <limits.h>
#include <math.h>
#include <cilk.h>

/// Pre-compiler macro for debugging 
#define RD_DEBUG
#define WS_TOP_LIST_ENGLISH ws->context->Cilk_global_state->englishOM_DS
#define WS_TOP_LIST_HEBREW ws->context->Cilk_global_state->hebrewOM_DS

/// FWD Declaration
struct Bottom_List_s;
struct Top_List_s;
struct Runtime_node_s;

/// The Node that makes up Bottom_Lists
typedef struct OM_Node_s{

	struct OM_Node_s *next;
	struct OM_Node_s *prev;
	int ID;
	unsigned /*long*/ int tag;
	struct Bottom_List_s * ds;

	struct Runtime_node_s * parent;

} OM_Node;


#ifdef RD_STATS
typedef struct ll_node_s {
	unsigned int data;
	struct ll_node_s 	 *next;
} ll_node;
#endif

/// Holds OM_Nodes and is what comprises the Top_List
typedef struct Bottom_List_s {

	struct Top_List_s * parent;
	OM_Node *head,*tail;
	int size;
	int reorder_flag; 
	struct Bottom_List_s *next;
	struct Bottom_List_s *prev;
	unsigned /*long*/ int tag;


#ifdef RD_STATS
	ll_node * list_of_size_of_bottom_list_when_split_head, *list_of_size_of_bottom_list_when_split_tail;
#endif

} Bottom_List;

typedef struct InsertRecord_s{

	Bottom_List *ds;
	OM_Node *x;
	OM_Node *y;
	int ID;

} InsertRecord;

/// Top-level LL made up of Bottom_List(s)
typedef struct Top_List_s{

	Bottom_List *head, *tail; /// TODO: change to Bottom_List of the sublist
	int size;


#ifdef RD_STATS
	ll_node * list_of_size_of_top_list_when_split_head, *list_of_size_of_top_list_when_split_tail;
#endif

} Top_List;

/*The runtime node that is carried through the cilk processes*/
typedef struct Runtime_node_s{
	struct OM_Node_s * english, *hebrew;
} Runtime_node;



/// Declarations of the OM-functions
Bottom_List * create_bl();
Top_List * create_tl();
void first_insert_bl(Bottom_List * ds, OM_Node *y);
void first_insert_tl(Top_List * list, Bottom_List * y);
void first_insert(Top_List * list, OM_Node* y);
void insert(struct CilkWorkerState_s *const ws, OM_Node * x, OM_Node * y);
void insert_tl(Bottom_List *x, Bottom_List *y);
//CILK2C
int order(OM_Node * x, OM_Node * y);
//CILK2C
void batchInsertOp (struct CilkWorkerState_s *const ws, void *dataStruct, void *data, size_t size, void *result);
void split_bl(Top_List * list, Bottom_List * list_to_split);
void rebalance_tl(Top_List * list, Bottom_List * pivot);
void relabel_tl_tag_range(Bottom_List *start, Bottom_List *end, const /*long */int skip_size);
void rebalance_bls(Top_List * list);
void print_tl(Top_List * list);
void print_bl(Bottom_List * list);
void free_bl (Bottom_List * list);
void free_tl ( Top_List * list );
void check_sub_correctness (Top_List * list);

#endif 


