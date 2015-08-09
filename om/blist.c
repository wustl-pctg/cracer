/** Lower-level linked-list for order-maintenance. */
#include <stdlib.h> // malloc
#include <assert.h>

#include "blist.h"
#include "om_common.h"

struct blist_s;
struct tl_node_s;

struct blist_node_s {
  label_t label;
  struct blist_node_s* next;
  struct blist_node_s* prev;
  struct blist_s* list; // Needed for node comparison
};

typedef bl_node node;

struct blist_s {
  node* head;
  node* tail;
  size_t size;

  // Needed for interacting with top list.
  struct tl_node_s* above;
};

/** # Private methods */
static size_t split(blist* self, blist*** lists) __attribute__((unused));

static label_t get_new_label(blist* self, node* base)
{
  label_t next_label = (base->next) ? base->next->label : MAX_LABEL;
  label_t lab = (base->label >> 1) + (next_label >> 1);

  // Correction for average two odd integers (integer division rounding)
  if ((base->label & next_label & 0x1) == 0x1) lab++;

  // Correction for adding to the end.
  if (!base->next) lab++;
  
  /// I think it could only match the base's label...
  return (base->label == lab) ? 0 : lab;
}

/// Node allocated, have ptr to base node, already have label
static void insert_internal(blist* self, node* base, node* n)
{
  if (!base) {
    self->tail = self->head = n;
    n->next = n->prev = NULL;
    n->label = 0;
  } else {
    assert(n->label > base->label);
    if (base == self->tail) { // i.e. insert at end
      self->tail = n;
      n->next = NULL;      
    } else {
      n->next = base->next;
      base->next->prev = n;
    }
    base->next = n;
    n->prev = base;
  }

  // Finalize
  self->size++;
  n->list = self;
}

static size_t split(blist* self, blist*** lists)
{
  size_t array_size = self->size / SUBLIST_SIZE;
  if (self->size < SUBLIST_SIZE && self->size % NODE_INTERVAL != 0)
    array_size++;
  *lists = malloc(array_size * sizeof(blist*));

  node* current_node = self->head;
  for (int list_index = 0; list_index < array_size; list_index++) {
    blist* list = (*lists)[list_index] = bl_new();
    
    node* next;

    current_node->prev = NULL;
    label_t current_label = 0;
    while (current_node) {
      next = current_node->next;
      current_node->label = current_label;
      insert_internal(list, current_node->prev, current_node);

      current_node = next;
      current_label += NODE_INTERVAL;
      if (list->size == SUBLIST_SIZE) break;
    }
  }

  // Free the sublist itself, but not its nodes!
  self->head = self->tail = NULL;
  self->size = 0;
  free(self);

  return array_size;
}

/** Allocate a new, uninitialized node. */
static inline node* node_new(){ return malloc(sizeof(node)); }

/** # Public methods */

blist* bl_new()
{
  blist* self = malloc(sizeof(*self));
  bl_create(self);
  return self;
}

void bl_create(blist* self)
{
  self->head = self->tail = NULL;
  self->size = 0;
}

void bl_destroy(blist* self)
{
  if (!self->head) return;
  for (node* it = self->head->next; it != NULL; it = it->next) {
    free(it->prev);
  }
  free(self->tail);
}

void bl_free(blist* self)
{
  if (self->head) bl_destroy(self);
  free(self);
}

node* bl_insert_initial(blist* self)
{
  assert(!self->head);
  node* n = node_new();
  insert_internal(self, NULL, n);
  return n;
}

node* bl_insert(blist* self, node* base)
{
  assert(base);
  label_t lab = get_new_label(self, base);
  if (lab == 0) return NULL; // Only the initial insert has label 0.
  node* n = node_new();
  n->label = lab;
  insert_internal(self, base, n);
  return n;
}

bool bl_precedes(const node* x, const node* y) { return x->label < y->label; }

void bl_verify(const blist* self)
{
  /// @todo
}
void bl_fprint(const blist* self, FILE* out)
{
  /// @todo
}

void bl_print(const blist* self) { bl_fprint(self, stdout); }
