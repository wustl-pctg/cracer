#ifndef _OM_COMMON_H
#define _OM_COMMON_H

typedef unsigned long label_t;
typedef size_t label_t;

#include <limits.h>
//#define MAX_LABEL ULONG_MAX
static label_t MAX_LABEL = ULONG_MAX;
static label_t NODE_INTERVAL = (((label_t)1) << 58); // N / log_2 N = 2^58
static label_t SUBLIST_SIZE = ((label_t)64); // log_2 N = 64

#if defined _OM_H || defined _TRIE_H
static label_t MAX_LEVEL = (sizeof(label_t) * 8);
#endif


#endif // #ifndef _OM_COMMON_H
