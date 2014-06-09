/*
 * =====================================================================================
 *
 *       Filename:  order-maintain.c
 *
 *    Description:  Holds data structures and routines that are used to maintain
 *    		    order of the SP parse tree.
 *
 *        Version:  1.0
 *        Created:  06/06/2014 04:01:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (aj), atjalex@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _CILK_ORDER_MAINTAIN_C
#define _CILK_ORDER_MAINTAIN_C
#include <cilk.h>
#include <cilk-cilk2c-pre.h>
#include <cilk-cilk2c.h>
#include <stdio.h>
#include <stdlib.h>


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




OM_DS * createOM_DS(){
	OM_DS * tmp = (OM_DS *) malloc(sizeof(OM_DS));

	//initialize variables (in this case linked list params)
	tmp->head = NULL;
	tmp->tail = NULL;
	tmp->size = 0;

	return tmp;
}

void destroyOM_DS(OM_DS * ds){


}


void insert(OM_DS * ds, OM_Node *x, OM_Node *y){



}

void appendNode(OM_DS * ds, OM_Node *x){


}

void OM_DS_Create(CilkContext *const context){

	if (context->Cilk_global_state){
		context->Cilk_global_state->englishOM_DS = malloc(sizeof(OM_DS));
		context->Cilk_global_state->hebrewOM_DS = malloc(sizeof(OM_DS));
		assert(context->Cilk_global_state->englishOM_DS && context->Cilk_global_state->hebrewOM_DS); //should this be cilk_assert?	
	}
}

void OM_DS_cleanup(CilkContext *const context){

	if (context->Cilk_global_state){
		
		//TODO:free node memory
		

		//free DS memory
		(context->Cilk_global_state->englishOM_DS)?free(sizeof(OM_DS)):printf("englishOM_DS is null, can't free");
		(context->Cilk_global_state->hebrewOM_DS)?free(sizeof(OM_DS)):printf("hebrewOM_DS is nul, can't free");

	}
}


#endif
