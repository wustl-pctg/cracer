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
