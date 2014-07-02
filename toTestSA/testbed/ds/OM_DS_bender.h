#ifndef OM_DS_BENDER_H
#define OM_DS_BENDER_H
/*
 * =====================================================================================
 *
 *       Filename:  OM_DS_bender.h
 *
 *    Description:  Contains all of the forward declarations for OM_DS_bender.c
 *
 *        Version:  1.0
 *        Created:  07/01/2014 01:46:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (aj), atjalex@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "OM_DS.h"
#define ENGLISH_ID	10
#define HEBREW_ID	11
/// Implementation of Bender's order maintenance data structure.
/// This is the top tier of the list
typedef struct Top_List_s{

	OM_DS *head, *tail; /// TODO: change to OM_DS of the sublist
	const int ID; /// English or Hebrew: do i need this?
	int size;
	float overflow_threshold; /// T in the bender paper
	/// TODO: precompute values of 1 / (T^i)  for much needed values of i
} Top_List;



/// Function forward declarations
void tag_range_relabel (Top_List *list, OM_DS *x, OM_DS *y, const int ID );
void top_list_rebalance(Top_List * list,OM_DS *pivot_l ,OM_DS *pivot_r, const int ID);
void insert_top_list(Top_List * list, OM_DS * x, OM_DS *y, const int ID, unsigned int end_tag);
void append_first_list (Top_List * list, OM_DS * first_sub_list, const int ID);
Top_List * init_top_list ();
void Top_List_free_and_free_nodes ( Top_List * list );

#endif
