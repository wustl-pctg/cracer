/*! This is the header for the test-run of Alex and Shane's
 * race_detct function idea
 */

#ifndef RACE_DETECT
#define RACE_DETECT

#include "../download/red_black_tree.h"
#include "../testbed/ds/linkedList.h"

typedef rb_red_blk_tree MemoryTree;

void race_detect(MemoryTree *, void *, Node *);


#endif //RACE_DETECT
