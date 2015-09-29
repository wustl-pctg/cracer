#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "om.h"
#include "om_common.h"

unsigned int g_num_malloc_calls;
double g_malloc_begin;
#include "blist.c"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char flag_t;

struct tl_node_s {
  label_t label;
  size_t level;

  union {
    size_t size;
    size_t num_leaves;
  };
  
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
  /** Note:
   *  Left/right pointers are only used for relabeling, and ONLY valid
   *  after a split operation. It would be extra work to splice the
   *  split nodes into the list of leaves while rebuilding (without a
   *  race condition on freeing the old nodes), so I'm not going to
   *  implement it unless we have a reason to always have a list of leaves.
   */

  union {
    blist* below;
    tl_node* split_nodes;
  };
  flag_t needs_rebalance;
};

struct om_s {
  struct tl_node_s* root;
  struct tl_node_s* head;
  struct tl_node_s* tail;
  size_t height;
};

tl_node* tl_node_new() { return (tl_node*)malloc(1 * sizeof(tl_node)); }

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
  root->below->above = root;

  // fast version
  root->level = 0;
  root->num_leaves = 0;
  root->label = MAX_LABEL;

  // slow but almost assuredly correct version:
  /* root->level = MAX_LEVEL; */
  /* root->num_leaves = 1; */
  /* root->label = 0; */

  root->needs_rebalance = 0;
  root->parent = root->left = root->right = NULL;

  /// The label of an internal node is the highest possible leaf label
  /// it could possibly contain.

  self->root = root;
  self->head = self->tail = NULL;
  self->height = 0;
}

om* om_new()
{
  om* self = (om*) malloc(sizeof(*self));
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

tl_node* om_get_tl(node* n)
{
  assert(n->list->above->level == MAX_LEVEL);
  return n->list->above;
}

void om_destroy(om* self) { destroy(self->root); }

node* om_insert_initial(om* self)
{
  assert(self->root->num_leaves == 0);

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
  self->root->num_leaves++;

  // Now create new bottom list and insert
  t->below = bl_new();
  t->below->above = t;

  //  node* n = bl_insert_initial(self->root->below);
  node* n = bl_insert_initial(t->below);
  assert(n->list == t->below);
  return n;
}

node* om_insert(om* self, node* base)
{
  assert(base->list->above->level == MAX_LEVEL);
  assert(base->list->above->below == base->list);
  node* n = bl_insert(base->list, base);
  if (n) assert(n->label > base->label);
  return n;
}

bool om_precedes(const node* x, const node* y)
{
  if (x == NULL) return true;
  if (y == NULL) return false;
  if (x->list == y->list) return x->label < y->label;
  return x->list->above->label < y->list->above->label;
}

int om_verify(const om* self)
{
  return 1; /// @todo [#C]
}

void om_fprint(const om* self, FILE* out)
{
  if (!om_verify(self)) fprintf(out, "Warning: OM is not valid!\n");

  size_t num_leaves = 1 << MAX_LEVEL;

  tl_node** future = (tl_node**) calloc(num_leaves, sizeof(tl_node*));
  tl_node** current = (tl_node**) calloc(num_leaves, sizeof(tl_node*));
  future[0] = self->root;

  for (int level = 0; level < MAX_LEVEL; ++level) {

    tl_node** tmp = current;
    current = future;
    future = tmp;

    size_t num_nodes = 1 << level;
    size_t spacing = 1 << (MAX_LEVEL - level);
    for (int i = 0; i < num_nodes; ++i) {
      tl_node* n = current[i];


      for (int j = 0; j < spacing; ++j) fprintf(out, "\t");
      if (!n || n->level != level) fprintf(out, "-");
      else fprintf(out, "%lu", n->label);
      for (int j = 0; j < spacing; ++j) fprintf(out, "\t");

      tl_node *left, *right;
      if (!n) {
        left = right = NULL;
      } else if (n->level == MAX_LEVEL) {
        left = n;
        right = NULL;
      } else {
        left = n->left;
        right = n->right;
      }
      future[i * 2] = left;
      future[i * 2 + 1] = right;
    }
    fprintf(out, "\n");
  }
  for (int i = 0; i < num_leaves; ++i) { // leaves
    tl_node* leaf = future[i];
    fprintf(out, "\t");
    if (leaf) fprintf(out, "%lu", leaf->label);
    else fprintf(out, "-");
    fprintf(out, "\t");
  }
  fprintf(out, "\n");
  free(future);
  free(current);
}

void om_print(const om* self) { om_fprint(self, stdout); }


#ifdef __cplusplus
} // extern C
#endif
