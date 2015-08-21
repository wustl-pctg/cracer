// The relabel-specific code for order-maintenance.

#include <string.h> // memcpy

#include "blist_split.c"

#define FAST_LOG2(x) (sizeof(label_t)*8 - 1 - __builtin_clzl((label_t)(x)))
#define FAST_LOG2_CEIL(x) (((x) - (1 << FAST_LOG2(x))) ? FAST_LOG2(x) + 1 : FAST_LOG2(x))
#define parfor for
#define spawn
#define sync

/// @todo remove after testing
//extern node* g_base;

static inline label_t range_right(tl_node* n) { return n->label; }
static inline label_t range_left(tl_node* n)
{
  // Need signed to do arithmetic shift
  long lab = 1 << (MAX_LEVEL - 1);
  lab >>= (n->level - 1);
  return ((label_t)lab) & range_right(n);
}


static inline double density(tl_node* n)
{
  /// @todo I've changed the size to mean the number of leaves...does
  /// this change this formula?
  return n->size / (double)(1 << (MAX_LEVEL - n->level));
}

static inline size_t capacity(tl_node* n)
{
  return range_right(n) - range_left(n) + 1;
}

int too_heavy(tl_node* n, size_t height)
{
  double threshold = 0.75 + 0.25 * (n->level / (double)height );
  return density(n) >= threshold;
}

tl_node* rebuild_recursive(tl_node* n, tl_node** array,
                           size_t lindex, size_t rindex,
                           label_t llab, label_t rlab);

void build_array_of_leaves(tl_node* n, tl_node** array,
                           size_t left_index, size_t right_index)
{
  if (!n) return; ///@todo assert?

  if (n->level == MAX_LEVEL) {
    size_t index = left_index;
    if (n->size == 1) {
      array[index] = n;
      assert(index + 1 == right_index);
    } else {
      tl_node* current = n->split_nodes;

      while (current) {
        
        assert(current->level == 42);
        current->level = 43;
        array[index] = current;

        if (current->next) assert(current->next->prev == current);
        if (current->prev) current->prev->next = NULL;
        current->prev = NULL;
        current = current->next;

        index++;
      }
      assert(index == right_index);
    }
  } else {
    assert(n->left);
    size_t mid = left_index + n->left->size;
    spawn build_array_of_leaves(n->left, array, left_index, mid);
    build_array_of_leaves(n->right, array, mid, right_index);
    sync;
    n->left = n->right = NULL;
  }

  // We don't want to free the root of this subtree...
  if (n->needs_rebalance) tl_node_free(n);
  return;
}

// Rebuild a subtree rooted at node n.
void rebuild_subtree(tl_node* n)
{
  assert(n->level < MAX_LEVEL);
  assert(n->needs_rebalance == 0);
  size_t array_size = n->size;
  tl_node** array = (tl_node**)malloc(array_size * sizeof(tl_node*));
  
  // will remove extra scaffolding
  build_array_of_leaves(n, array, 0, array_size);
  //  assert(g_base->list->above->level == 43); /// @todo remove
  n->left = n->right = NULL;

  label_t llab = range_left(n);
  label_t rlab = n->label;
  label_t spacing = (rlab - llab + 1) / array_size;
  assert(spacing > 0);
  
  size_t mindex = array_size / 2;
  label_t mlab = llab + spacing * mindex - 1;

  n->left = spawn rebuild_recursive(n, array, 0, mindex,
                                    llab, mlab);
  n->right = rebuild_recursive(n, array, mindex, array_size,
                               mlab + 1, rlab);
  sync;
  free(array);
}

tl_node* rebuild_recursive(tl_node* parent, tl_node** array,
                           size_t lindex, size_t rindex,
                           label_t llab, label_t rlab)
{
  size_t size = rindex - lindex;
  tl_node* n = (size == 1) ? array[lindex] : tl_node_new();
  n->size = size;
  n->parent = parent;
  n->needs_rebalance = 0;

  assert(llab <= rlab);

  if (size == 1) {
    assert(array[lindex] == n);
    n->label = llab;
    n->level = MAX_LEVEL;
    n->left = n->right = NULL;
    /* assert(array[left_index]->next = NULL); */
    /* assert(array[left_index]->prev = NULL); */
    /* if (left_index > 0) { */
    /*   assert(n->label > 0); */
    /*   n->prev = array[left_index - 1]; */
    /*   array[left_index - 1]->next = n; */
    /* } */
  } else {
    n->level = n->parent->level + 1;

    size_t mindex = lindex + size / 2;
    label_t spacing = (rlab - llab + 1) / size;
    label_t mlab = llab + spacing * (mindex - lindex) - 1;

    n->left = spawn rebuild_recursive(n, array, lindex, mindex, llab, mlab);
    n->right = rebuild_recursive(n, array, mindex, rindex, mlab + 1, rlab);
    sync;
    
    n->label = array[rindex - 1]->label;
  }
  
  return n;
}

void rebuild_leaf(tl_node* n)
{
  assert(n->level == MAX_LEVEL && n->parent && n->size > 1);
  assert(n->left == NULL && n->right == NULL);

  size_t array_size = n->size;
  tl_node** array = (tl_node**)malloc(array_size * sizeof(tl_node*));
  
  // will remove extra scaffolding
  n->level = MAX_LEVEL; // reset for build_array_of_leaves
  build_array_of_leaves(n, array, 0, array_size);

  // Change this node to be an internal node.
  if (n->parent->right == n) { // is right child
    n->label = n->parent->label;
  } else { // is left child
    n->label = (range_right(n->parent) - range_left(n->parent)) / 2;
  }
  n->level = n->parent->level + 1;
  label_t llab = range_left(n);
  label_t rlab = range_right(n);
  
  //  right_lab++; // we pass in a higher label than we will use.
  assert(rlab % 2 == 1);
  label_t spacing = (rlab - llab + 1) / n->size;
  size_t mindex = n->size / 2;
  label_t mlab = llab + spacing * mindex - 1;

  n->left = spawn rebuild_recursive(n, array, 0, mindex, llab, mlab);
  n->right = rebuild_recursive(n, array, mindex, n->size, mlab + 1, rlab);
  sync;

}

void recursive_rebalance(tl_node* n, size_t height)
{
  if (!n || !n->needs_rebalance) return; // stop here
  n->needs_rebalance = 0;

  if (n->level == MAX_LEVEL) return rebuild_leaf(n);
  assert(n->left);
  assert(capacity(n) >= n->size);
  if (too_heavy(n->left, height) || (n->right && too_heavy(n->right, height))) {
    rebuild_subtree(n);
    /* assert(!too_heavy(n, height) || n->parent == NULL); */
    /* assert(!too_heavy(n->left, height)); */
    /* assert(n->right); */
    /* assert(!too_heavy(n->right, height)); */
    /* assert(capacity(n) > n->size); */
  }


  
  spawn recursive_rebalance(n->left, height);
  recursive_rebalance(n->right, height);
}


static inline void rebalance(om* self)
{
  recursive_rebalance(self->root, self->height);
}

void om_relabel(om* self, tl_node** heavy_lists, size_t num_heavy_lists)
{
  size_t old_size = self->root->num_leaves; // to check for overflow
  parfor (int i = 0; i < num_heavy_lists; ++i) {
    tl_node* current = heavy_lists[i];

    assert(current->level == MAX_LEVEL);
    
    // Split into several sublists.
    size_t num_split_lists = split(current->below);//, &new_lists);

    if (num_split_lists > 1) {

      // Update sizes up the tree, while also calculating the height
      // from the current node.
    
      // -1 because we increment it below for the current node
      size_t height = FAST_LOG2_CEIL(num_split_lists) - 1;
      while (current) {
        __sync_fetch_and_add(&current->size, num_split_lists - 1);
        current->needs_rebalance = 1;
        height++;
        current = current->parent;
      }

      // Update the height, if necessary.
      size_t old_height = self->height;
      while (height > old_height) {
        old_height = __sync_val_compare_and_swap(&self->height, old_height, height);
      }
    } else { // split into just 1 list! This occasionally happens
             // because SUBLIST_SIZE is not exact
      current->needs_rebalance = 0;
      blist* list = current->split_nodes->below;
      current->split_nodes->level = MAX_LEVEL;
      tl_node_free(current->split_nodes);
      current->below = list;
    }
  }
  size_t new_size = self->root->num_leaves;
  if (new_size < old_size // overflow
      || ((MAX_LABEL + 1 != 0) && new_size > MAX_LABEL + 1)) {
    fprintf(stderr, "OM data structure is full!\n");
    exit(1);
  } else {
    rebalance(self);
    //    printf("Base's tl level: %zu\n", g_base->list->above->level);
  }
}
