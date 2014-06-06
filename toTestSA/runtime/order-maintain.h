/*
 * =====================================================================================
 *
 *       Filename:  order-maintain.h
 *
 *    Description:  Header for batched order maintenance data structure
 *    		 
 *
 *        Version:  1.0
 *        Created:  06/06/2014 04:05:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (aj), atjalex@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

/*
 * Abstract Order maintenance DS
 */
typedef struct linked_list_s {

	OMNode * head, *tail;
	int size;

} OM_DS;

/*
 * Currently implemented as a linked list node
 */
typedef struct OMNode_s{

	struct OMNode_s *next;

} OMNode;

typedef struct insert_op_s {
 OM_DS * ds; //data struct to operate on
 OM_Node * x, *y;//insert node y after x
} InsertRecord;


