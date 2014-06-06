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
 * Currently implemented as a linked list node
 */
typedef struct OMNode_s{

	struct OMNode_s *next;

} OMNode;
/*
 * Abstract Order maintenance DS
 */
typedef struct linked_list_s {

	OMNode * head, *tail;
	int size;

} OM_DS;


typedef struct insert_op_s {
 OM_DS * ds; //data struct to operate on
 OMNode * x, *y;//insert node y after x
} InsertRecord;


