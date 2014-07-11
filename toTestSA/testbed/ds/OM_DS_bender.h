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
/// Implementation of Bender's order maintenance data structure.
/// This is the top tier of the list
typedef struct Top_List_s{

	Bottom_List *head, *tail; /// TODO: change to Bottom_List of the sublist
	const int ID; /// English or Hebrew: do i need this?
	int size;
	double overflow_constant; /// T in the bender paper
	/// TODO: precompute values of 1 / (T^i)  for much needed values of i
} Top_List;



/// Function forward declarations
void tag_range_relabel (Top_List *list, Bottom_List *x, Bottom_List *y, const int ID, unsigned long tag_spacing );
void top_list_rebalance(Top_List * list,Bottom_List *pivot ,const int ID);
void insert_top_list(Top_List * list, Bottom_List * x, Bottom_List *y, const int ID, unsigned long int RELABELING_END_TAG, int * collision_detection );
void append_first_list (Top_List * list, Bottom_List * first_sub_list, const int ID);
Top_List * init_top_list ();
void Top_List_free_and_free_nodes ( Top_List * list );

#endif
