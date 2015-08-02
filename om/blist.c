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
  struct blist_s* list; // Needed for rebalancing.
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
static size_t split(blist* self, blist*** lists);

/// Node allocated, have ptr to base node
static void insert_internal(blist* self, node* base, node* n)
{
  if (base == self->tail) { // i.e. insert at end
    if (!base) { // i.e. empty list
      n->label = 0;
      self->head = n;
    } else {
      n->label = base->label + NODE_INTERVAL;
      /// @todo check for overflow
      base->next = n;
    }
    self->tail = n;
    n->next = NULL;      

  } else {
    n->label = (base->label >> 1) + (base->next->label >> 1);
    // Correction for average two odd integers (integer division rounding)
    if ((base->label & base->next->label & 0x1) == 0x1) n->label++;

    /// I think it could only match the base's label...

    if (base->label == n->label) split(self, NULL);
    /** @todo go straight to starting split? Eventually the
        upper-level structure will handle the big split, so I don't
        thnk this is a good idea. Instead, we should return null and
        let someone else decide what to do.
    */

    n->next = base->next;
    base->next->prev = n;
    base->next = n;
  }

  // Finalize
  n->prev = base;
  self->size++;
}

static size_t split(blist* self, blist*** lists)
{
  size_t array_size = self->size / SUBLIST_SIZE;
  if (self->size % NODE_INTERVAL != 0) array_size++;
  *lists = malloc(array_size * sizeof(blist*));

  node* current_node = self->head;
  for (int list_index = 0; list_index < array_size; list_index++) {
    blist* list = (*lists)[list_index] = bl_new();
    
    node* next;

    current_node->prev = NULL;
    while (current_node) {
      next = current_node->next;
      insert_internal(list, current_node->prev, current_node);
      //      printf("added node %p to %p\n", current_node, list);
      current_node = next;
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
  self->tail = self->head = n;
  n->next = n->prev = NULL;
  n->list = NULL;
  n->label = 0;
  n->list = self;
  self->size = 1;
  return n;
}

node* bl_insert(blist* self, node* base)
{

  /// @todo this is not very good design. Should only allocate the
  /// node when we know we need it.
  node* n = node_new();
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
