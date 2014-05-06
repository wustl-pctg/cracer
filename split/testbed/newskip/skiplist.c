#ifndef SKIPLIST_C
#define SKIPLIST_C

#include <stdlib.h>

#include "skiplist.h"

#define NIL this->hdr

struct Node {
  int data;
  struct Node* forward[1];
};

typedef struct {
  struct Node* hdr;
  int level;
  size_t size;
} Skiplist;

static inline struct Node* init_hdr()
{
  struct Node* hdr = malloc(sizeof(struct Node));
  //  hdr->data = 0;
  hdr->forward[0] = hdr;
  hdr->forward[1] = hdr;

  return hdr;
}

static inline struct Node* init_node(Skiplist* this)
{
  struct Node* n = malloc(sizeof(struct Node));
  // @todo check for sufficient memory
  n->forward[0] = NIL;
  n->forward[1] = NIL;

  return n;
}
Skiplist* skiplist_create()
{
  Skiplist *s = malloc(sizeof(Skiplist));
  s->hdr = init_hdr();
  s->size = 0;
  s->level = 0;

  return s;
}

struct Node* skiplist_find_location(Skiplist* this, int data)
{
  int i;
  struct Node* current = this->hdr;

  for(i = this->level; i >= 0; i--) {
    while (current->forward[i] != NIL
           && current->forward[i]->data < data) {
      current = current->forward[i];
    }
  }

  return current;
}

struct Node* skiplist_find(Skiplist *this, int data)
{
  struct Node* location = skiplist_find_location(this, data)->forward[0];
  if (location->data == data) return location;
  return NULL;
}

struct Node* skiplist_insert(Skiplist* this, int data)
{
  struct Node* new_node;
  struct Node* location = skiplist_find_location(this, data);
  if (location->data == data) return location;

  // @todo determine level

  new_node = init_node(this);
  new_node->data = data;

  // @todo Update ALL forward links, not just forward[0]
  new_node->forward[0] = location->forward[0];
  location->forward[0] = new_node;

  this->size++;
  return new_node;
}

void skiplist_free(Skiplist* this)
{
  free(this);
}

size_t skiplist_size(Skiplist* this)
{
  return this->size;
}

unsigned skiplist_verify_size(Skiplist* this)
{
  size_t actual_size = 0;

  return actual_size == this->size;
}

#endif /* SKIPLIST_C */
