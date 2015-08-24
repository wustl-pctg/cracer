/* Copied version of .cilkh header modified for c translation */

/*
 * =====================================================================================
 *
 *       Filename:  OM_DS_TREE.h
 *
 *    Description:  Header 
 *
 *        Version:  1.0
 *        Created:  07/15/2014 11:36:19 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (aj), atjalex@gmail.com; Shane Deiley, shanedeiley@wustl.edu
 *        Company:  Washington University in St. Louis Summer REU 2014
 *
 * =====================================================================================
 */

#ifndef _OM_DS_H
#define _OM_DS_H

//Parallel
#include <cilk-lib.cilkh>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

/*/// Pre-compiler macro for debugging */
//#define RD_DEBUG
//#define RD_STATS
#define PARALLEL_THRESHOLD_FLAG

#define LEFT 0
#define RIGHT 1

/// FWD Declaration
struct Bottom_List_s;
struct Top_List_s;
struct Internal_Node_s; 

/*/// The Node that makes up Bottom_Lists*/
typedef struct OM_Node_s{

	struct OM_Node_s *next;
	struct OM_Node_s *prev;
	unsigned /*long*/ int tag;
	int ID;
	struct Bottom_List_s * ds;

} OM_Node;

#ifdef RD_STATS
typedef struct ll_node_s {
	unsigned int data;
	struct ll_node_s 	 *next;
} ll_node;
#endif


/// Holds OM_Nodes and is what comprises the Top_List
typedef struct Bottom_List_s {
	struct Top_List_s *parent;
	OM_Node *head,*tail;
	int size;
	int reorder_flag;
	struct Bottom_List_s *next;
	struct Bottom_List_s *prev;
	unsigned /*long*/ int tag;

	struct Internal_Node_s * internal;

#ifdef RD_STATS
	ll_node * list_of_size_of_bottom_list_when_split_head, *list_of_size_of_bottom_list_when_split_tail;
#endif

} Bottom_List;

typedef struct Internal_Node_s {
	Bottom_List * bl;
	unsigned int base, lvl, num_children;
	struct Internal_Node_s *parent, *left, *right;
} Internal_Node;

typedef struct InsertRecord_s{
	OM_Node *x;
	OM_Node *y;
} InsertRecord;

/// Top-level LL made up of Bottom_List(s)
typedef struct Top_List_s{

	Bottom_List *head, *tail; /// TODO: change to Bottom_List of the sublist
	int size;

#ifdef RD_STATS
	ll_node * list_of_size_of_top_list_when_split_head, *list_of_size_of_top_list_when_split_tail;
#endif

} Top_List;

/// Parallel:
Internal_Node ** build_array_from_rebalance_list(Internal_Node *current_node);
void create_scaffolding(Internal_Node *, Internal_Node *);
void rebuild_tree (Internal_Node * current_node, Internal_Node ** nodeArray, int startIndex,  int endIndex);
void par_build_array_from_rebalance_list(Internal_Node ** buildArray,Internal_Node * current_node, int start, int end);


/// Declarations of the OM-functions (in batchify part)
void insert (OM_Node *x, OM_Node *y);
void insert_internal(OM_Node * x, OM_Node *y);

/// Declarations of the OM-functions
Bottom_List * create_bl();
Top_List * create_tl();
void first_insert_bl(Bottom_List * ds, OM_Node * y);
void first_insert_tl(Top_List * list, Bottom_List * y);
void first_insert(Top_List *, OM_Node *);
void insert_tl(Bottom_List *x, Bottom_List *y);
int order(OM_Node * x, OM_Node * y);
void split_bl(Top_List * list, Bottom_List * list_to_split);
void rebalance_tl(Bottom_List * pivot);
void relabel_tl_tag_range(Bottom_List *start, Bottom_List *end, const /*long*/ int skip_size);
void rebalance_bls(Top_List * list);
void print_tl(Top_List * list);
void print_bl(Bottom_List * list);
void free_bl (Bottom_List * list);
void free_tl ( Top_List * list );
void check_sub_correctness (Top_List * list);
void print_tree (Top_List * list);
void c_rebuild_tree(Internal_Node * current_node, Internal_Node ** nodeArray, int startIndex,  int endIndex);
void c_par_build_array_from_rebalance_list(Internal_Node ** buildArray,Internal_Node * current_node, int start, int end);




#endif 