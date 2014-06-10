/*! This is the header for the test-run of Alex and Shane's
 * race_detect function idea
 */

#ifndef RACE_DETECT_H
#define RACE_DETECT_H

#include <stdlib.h>
#include <stdio.h>
#include "../testbed/ds/linkedList.h"

typedef struct {
  
  void * memloc; //The memory location where the read/write occurs
  Node * left_r; //leftmost node that is reading
  Node * right_r; //rightmost node that is reading
  Node * left_w; //leftmost node that is writing
  Node * right_w; //rightmost node that is writing

} MemADT;

void race_detect(void * memloc);


#endif //RACE_DETECT_H
