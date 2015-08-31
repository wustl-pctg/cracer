/** Lower-level linked-list for order-maintenance. */
#include <stdlib.h> // malloc
#include <assert.h>

#include "blist.h"
#include "om_common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct blist_s;
struct tl_node_s;

typedef bl_node node;

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
  n->list = self;
}

/** Allocate a new, uninitialized node. */
static inline node* node_new()
{ 
  //  g_num_malloc_calls++;
  //  RDTOOL_INTERVAL_BEGIN(OM_REBUILD_MALLOC);
  node* n = (node*)malloc(sizeof(node)); 
  //  RDTOOL_INTERVAL_END(OM_REBUILD_MALLOC);
  return n;
}

/** # Public methods */

blist* bl_new()
{
  blist* self = (blist*)malloc(sizeof(struct blist_s));
  bl_create(self);
  return self;
}

void bl_create(blist* self)
{
  self->head = self->tail = NULL;
  self->above = NULL;
  self->half_full = 0;
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

bool bl_precedes(const node* x, const node* y)
{
  assert(x->list == y->list);
  return x->label < y->label;
}

size_t bl_size(const blist* self)
{
#ifdef BL_HAS_SIZE_FIELD
  return self->size;
#else
  size_t count = 0;
  node* current = self->head;
  while (current) {
    count++;
    current = current->next;
  }
  return count;
#endif // BL_HAS_SIZE_FIELD  
}

int bl_verify(const blist* self)
{
  /// @todo
  return 0;
}
void bl_fprint(const blist* self, FILE* out)
{
  node* current = self->head;
  size_t size = 0;
  fprintf(out, "Blist at %p\n", self);
  while (current) {
    size++;
    fprintf(out, "->%zu", current->label);
    current = current->next;
  }
  fprintf(out, "\nSize: %zu\n", size);
}

void bl_print(const blist* self) { bl_fprint(self, stdout); }

#ifdef __cplusplus
} // extern "C"
#endif
