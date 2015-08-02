#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "om.h"
#include "om_common.h"
#include "blist.c"

struct tl_node_s {
  label_t label;
  size_t level;
  size_t size;
  
  
  struct tl_node_s* parent;

  /// Leaves don't need left/right pointers
  /// Internal nodes don't need prev/next pointers
  union {
    struct tl_node_s* left;
    struct tl_node_s* prev;
  };
  union {
    struct tl_node_s* right;
    struct tl_node_s* next;
  };

  // @todo this could be unioned with something else, but probably
  // isn't necessary. Actually, I'm not sure this is necessary. We may
  // be able to get away with an "upper" pointer from the bottom list only.
  blist* below;
  short needs_rebalance;
};

struct om_s {
  struct tl_node_s* root;
  struct tl_node_s* head;
  struct tl_node_s* tail;
  //  size_t size;
};


tl_node* tl_node_new() { return malloc(sizeof(tl_node)); }

void tl_node_free(tl_node* n)
{
  if (n->level < MAX_LEVEL) {
    if (n->left) tl_node_free(n->left);
    if (n->right) tl_node_free(n->right);
  }
  free(n);
}

// The relabel functions are separated to make the code easier to read.
#include "om_relabel.c"

void om_create(om* self)
{
  tl_node* root = tl_node_new();
  root->below = bl_new();
  root->level = 0;
  root->size = 1;
  root->parent = root->left = root->right = NULL;

  /// The label of an internal node is the highest possible leaf label
  /// it could possibly contain.
  root->label = MAX_LABEL;

  self->root = root;
  self->head = self->tail = NULL;
}

om* om_new()
{
  om* self = malloc(sizeof(*self));
  om_create(self);
  return self;
}

void om_free(om* self)
{
  om_destroy(self);
  free(self);
}

/// Internal, recursive destroy
void destroy(tl_node* n)
{
  if (!n) return;
  if (n->level < MAX_LEVEL) {
    destroy(n->left);
    destroy(n->right);
  } else {
    bl_free(n->below);
  }
  free(n);
}

void om_destroy(om* self) { destroy(self->root); }

node* om_insert_initial(om* self)
{
  assert(self->root->size == 1);

  // Make upper-level node first
  tl_node* t = tl_node_new();
  t->label = 0;
  t->level = MAX_LEVEL;
  t->size = 1;
  t->parent = self->root;
  t->prev = t->next = NULL;
  t->needs_rebalance = 0;

  self->root->left = t;
  self->head = self->tail = t;
  self->root->size++;

  // Now create new bottom list and insert
  t->below = bl_new();
  t->below->above = t;

  node* n = bl_insert_initial(t->below);
  assert(n->list == t->below);
  return n;
}

node* om_insert(om* self, node* base)
{
  assert(base->list->above->below == base->list);
  return bl_insert(base->list, base);
}

bool om_precedes(const node* x, const node* y)
{
  if (x->list == y->list) return x->label < y->label;
  return x->list->above->label < y->list->above->label;
}

void om_verify(const om* self)
{
  /// @todo [#C]
}

void om_fprint(const om* self, FILE* out)
{
  /// @todo [#C]
  fprintf(out, "Print not implemented.");
}

void om_print(const om* self) { om_fprint(self, stdout); }
