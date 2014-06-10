#include "race_detect.h"

/*! This is the race-detection function.
  \param memloc the memory location being checked
  \param currentNode The data structure node in eng-heb OM-DS
 */
void race_detect(void * memloc) {

  MemADT * detecter;
  detecter = (MemADT *)malloc(sizeof(MemADT));

  detecter = //RBInsert would return node found in tree using mem as key
    //The tree was ordered by the key and each key had data (list)
    // We have a pointer to the key directly, and so we wouldn't 
    // need find/search
    // 



  // Conceptual steps:
  // 1. Access four things pointed to by memloc
  // 2. check current node  against leftmost write, leftmost read,
  // rightmost write, and rightmost read nodes
  // 3. If race-condition, report it
  // 4. If more left than leftmost or more right than rightmost of
  // same type, re-assign pointer to current node


  
}
