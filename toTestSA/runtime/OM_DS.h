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

/// Pre-compiler macro for debugging 
//#define RD_DEBUG

/// FWD Declaration
struct Bottom_List_s;
struct Runtime_node_s;

/// The Node that makes up Bottom_Lists
typedef struct OM_Node_s{

	struct OM_Node_s *next;
	struct OM_Node_s *prev;
	int ID;
	unsigned long int tag;
	struct Bottom_List_s * ds;

	Runtime_node_s * parent;
} OM_Node;


/*The runtime node that is carried through the cilk processes*/
typedef struct Runtime_node_s{
	OM_Node * english, *hebrew;
} Runtime_node;




/// Holds OM_Nodes and is what comprises the Top_List
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

/// Top-level LL made up of Bottom_List(s)
typedef struct Top_List_s{

	Bottom_List *head, *tail; /// TODO: change to Bottom_List of the sublist
	int size;

} Top_List;


/// Declarations of the OM-functions
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


