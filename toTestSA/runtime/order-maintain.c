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
#include <order-maintain.h>
#include <stdio.h>
#include <stdlib.h>






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

}

void OM_DS_cleanup(CilkContext *const context){

}


#endif
