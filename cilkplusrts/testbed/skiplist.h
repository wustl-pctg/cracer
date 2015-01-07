#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memcpy

#include <cilk/cilk.h>
#include <cilk/batcher.h>

#define spawn cilk_spawn
#define sync cilk_sync

typedef int T;                  /* type of item to be stored */
#define compLT(a,b) (a < b)
#define compEQ(a,b) (a == b)
#define compGT(a,b) (a > b)
#define swap(a, b, t) t = a; a = b; b = t

/* levels range from (0 .. MAXLEVEL) */
#define MAXLEVEL 30
#define INSERT_CHUNK 512

typedef struct Node_ {
  T data;                     /* user's data */
  struct Node_ *forward[1];   /* skip list forward pointer */
} Node;

typedef struct {
  Node *hdr;                  /* list Header */
  int listLevel;              /* current level of list */
} SkipList;

extern SkipList slist;                  /* skip list information */

#define NIL slist.hdr

Node* insertNode(T data);
void deleteNode(T data);
Node *findNode(T data);
void initList();
int get_size();

void batch_createNodes(int index, void* info);
void batch_relateNodes(int index, void* infos);
void batch_mergeLists(int index, void* infos);
void batch_insert_par2(void* dataStruct, void* data,
                            size_t numElements, void* results);
void SkipList_insert(int val);
