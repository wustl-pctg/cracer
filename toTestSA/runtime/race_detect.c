#include "race_detect.h"
#include <stdio.h>

/*! This is the race-detection function.
  \param memloc the memory location being checked
  \param currentNode The data structure node in eng-heb OM-DS
 */
void race_detect(MemoryTree * memTree, void * memloc, Node * currentNode)
{
  rb_red_blk_node * rbNode;
  rbNode = (rb_red_blk_node *)malloc(sizeof(rb_red_blk_node));
  rbNode = RBTreeInsert(memTree, memloc, currentNode);
  //TODO: call order on memloc rbNode & it's currentNodes...

}

/* ======== These all need to be in the runtime system=============
  MemoryTree * memTree;
  memTree = RBTreeCreat(MemComp, MemDest, InfoDest, MemPrint, InfoPrint);

//!Tree functions necessary for creating RBTree
void MemDest(void* a) {free(a);}
int MemComp(const void* a,const void* b) {
  if( a > b) return(1); if( a < b) return(-1); return(0);
}
void MemPrint(const void* a) {printf("%p\n",a);}
void InfoPrint(void* a) {;}
void InfoDest(void *a){;}

*/
