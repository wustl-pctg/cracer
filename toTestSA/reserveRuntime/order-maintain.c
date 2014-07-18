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
#include <malloc.h>




OM_DS * createOM_DS(){
  OM_DS * tmp = (OM_DS *) malloc(sizeof(OM_DS));

  //initialize variables (in this case linked list params)
  tmp->head = NULL;
  tmp->tail = NULL;
  tmp->size = 0;

  return tmp;
}

void destroyOM_DS(OM_DS * ds){

  return;
}


void insert(OM_DS * ds, OM_Node *x, OM_Node *y){

  return;

}

void appendNode(OM_DS * ds, OM_Node *x){

  return;
}

void OM_DS_Create(CilkContext *const context){

  if (context->Cilk_global_state){
    context->Cilk_global_state->englishOM_DS = createOM_DS());
  context->Cilk_global_state->hebrewOM_DS = createOM_DS();
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