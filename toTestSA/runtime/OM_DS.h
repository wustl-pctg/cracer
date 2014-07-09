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
 *         Author:  Alex Jones (aj), atjalex@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _OM_DS_H
#define _OM_DS_H

struct Bottom_List_s;

typedef struct OM_Node_s{
	struct OM_Node_s *next_e, *next_h;
	struct OM_Node_s *prev_e, *prev_h;
	int id;
	unsigned int tag_e, tag_h;
	struct Bottom_List_s * ds_h, * ds_e;

} OM_Node;

typedef struct Bottom_List__s {
	OM_Node *head,*tail;
	int size_e, size_h;
	int Reorder_flag_e, Reorder_flag_h;
	struct Bottom_List_s *next;
	struct Bottom_List_s *prev;
	int id;

	unsigned long int tag;


} Bottom_List;

typedef struct InsertRecord_s{
	OM_Node *x;
	OM_Node *y;
	int ID;

} InsertRecord;

typedef struct Top_List_s{

	Bottom_List *head, *tail; 
	const int ID; /// English or Hebrew: do i need this?
	int size;
	double overflow_constant; /// T in the bender paper
	/// TODO: precompute values of 1 / (T^i)  for much needed values of i
} Top_List;


#endif



/*! Previous Structs before OM_Bender

typedef struct OM_Node_s{
	struct OM_Node_s *next_english, *next_hebrew;
	int id;

} OM_Node;

typedef struct OM_DS_s {
	OM_Node *head,*tail;
	int size;

} OM_DS;

typedef struct InsertRecord_s{
	OM_DS *ds;
	OM_Node *x;
	OM_Node *y;
	int ID;

} InsertRecord;
#endif

*/

