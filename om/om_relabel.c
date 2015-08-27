// The relabel-specific code for order-maintenance.

#include <string.h> // memcpy
#include "blist_split.c"

#define FAST_LOG2(x) (sizeof(label_t)*8 - 1 - __builtin_clzl((label_t)(x)))
#define FAST_LOG2_CEIL(x) (((x) - (1 << FAST_LOG2(x))) ? FAST_LOG2(x) + 1 : FAST_LOG2(x))
#define parfor for
#define spawn
#define sync

static inline int is_leaf(tl_node* n)
{
  return n->level == MAX_LEVEL || (n->prev == NULL && n->right == NULL);
}

static inline label_t range_right(tl_node* n) { return n->label; }
static inline label_t range_left(tl_node* n)
{
  // Need signed to do arithmetic shift
  if (!n->parent) return 0;
  long lab = (long)MAX_LABEL; // all 1s
  lab <<= (MAX_LEVEL - 1);
  lab >>= (n->level - 1);
  return ((label_t)lab) & range_right(n);
}

static inline double density(tl_node* n)
{
  return n->size / (double)(1 << (MAX_LEVEL - n->level));
}

static inline size_t capacity(tl_node* n)
{
  if (!n->parent) return MAX_LABEL;
  return range_right(n) - range_left(n) + 1;
}

int too_heavy(tl_node* n, size_t height)
{
  if (!n->needs_rebalance) return 0;
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

  if (is_leaf(n)) {
    size_t index = left_index;
    if (n->size == 1) {
      array[index] = n;
      assert(index + 1 == right_index);
    } else {
      tl_node* current = n->split_nodes;

      while (current) {
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
void rebuild(tl_node* n)
{
  //  RDTOOL_INTERVAL_BEGIN(OM_REBUILD);
  size_t array_size = n->size;
  //  RDTOOL_INTERVAL_BEGIN(OM_REBUILD_MALLOC);
  tl_node** array = (tl_node**)malloc(array_size * sizeof(tl_node*));
  //  RDTOOL_INTERVAL_END(OM_REBUILD_MALLOC);
  
  // will remove extra scaffolding
  //RDTOOL_INTERVAL_BEGIN(OM_BUILD_ARRAY_OF_LEAVES);
  build_array_of_leaves(n, array, 0, array_size);
  //  RDTOOL_INTERVAL_END(OM_BUILD_ARRAY_OF_LEAVES);

  if (is_leaf(n) && n->parent) {
    // Change this node to be an internal node.
    if (n->parent->right == n) { // is right child
      n->label = n->parent->label;
    } else { // is left child
      n->label = (range_right(n->parent) - range_left(n->parent)) / 2;
    }
    n->level = n->parent->level + 1;
  }
  n->left = n->right = NULL;

  label_t llab = range_left(n);
  label_t rlab = n->label;
  assert(rlab % 2 == 1);

  size_t mindex = array_size / 2;
  label_t mlab = llab + ((rlab - llab) / 2 + 1) - 1;

  n->left = spawn rebuild_recursive(n, array, 0, mindex,
                                    llab, mlab);
  n->right = rebuild_recursive(n, array, mindex, array_size,
                               mlab + 1, rlab);
  sync;
  free(array);
  //  RDTOOL_INTERVAL_END(OM_REBUILD);
}

tl_node* rebuild_recursive(tl_node* parent, tl_node** array,
                           size_t lindex, size_t rindex,
                           label_t llab, label_t rlab)
{
  size_t size = rindex - lindex;
  //  RDTOOL_INTERVAL_BEGIN(OM_REBUILD_MALLOC);
  tl_node* n = (size == 1) ? array[lindex] : tl_node_new();
  //  RDTOOL_INTERVAL_END(OM_REBUILD_MALLOC);
  n->size = size;
  n->parent = parent;
  n->needs_rebalance = 0;

  assert(llab <= rlab);

  if (size == 1) {
    assert(array[lindex] == n);
    n->label = llab;
    n->level = MAX_LEVEL;
    n->left = n->right = NULL;
  } else {
    n->level = n->parent->level + 1;

    size_t mindex = lindex + size / 2;
    label_t mlab = llab + ((rlab - llab) / 2 + 1) - 1;

    n->left = spawn rebuild_recursive(n, array, lindex, mindex, llab, mlab);
    n->right = rebuild_recursive(n, array, mindex, rindex, mlab + 1, rlab);
    sync;
    
    //    n->label = array[rindex - 1]->label;
    n->label = rlab;
    assert(rlab % 2 == 1);
  }
  
  return n;
}

void rebalance(tl_node* n, size_t height)
{
  if (!n || !n->needs_rebalance) return; // stop here
  n->needs_rebalance = 0;

  if (is_leaf(n)) return rebuild(n);
  assert(capacity(n) >= n->size);
  if (too_heavy(n->left, height) || (n->right && too_heavy(n->right, height))) {
    return rebuild(n);
  }
  
  spawn rebalance(n->left, height);
  rebalance(n->right, height);
}


static inline void om_rebalance(om* self)
{
  rebalance(self->root, self->height);
}

void om_relabel(om* self, tl_node** heavy_lists, size_t num_heavy_lists)
{
  //RDTOOL_INTERVAL_BEGIN(OM_RELABEL);
  size_t old_size = self->root->num_leaves; // to check for overflow
  printf("Splitting %zu lists.\n", num_heavy_lists);
  parfor (int i = 0; i < num_heavy_lists; ++i) {
    tl_node* current = heavy_lists[i];

    assert(current->level == MAX_LEVEL);
    assert(current->parent);
    
    // Split into several sublists.
    //    RDTOOL_INTERVAL_BEGIN(BLIST_SPLIT);
    size_t num_split_lists = split(current->below);
    //    RDTOOL_INTERVAL_END(BLIST_SPLIT);

    //    RDTOOL_INTERVAL_BEGIN(OM_UPDATE_SIZES);
    if (num_split_lists > 1) {

      current->level = current->parent->level + 1;
      current->next = current->prev = NULL; /// @todo is this okay?

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
      list->above = current;
    }
    //RDTOOL_INTERVAL_END(OM_UPDATE_SIZES);
  }

  size_t new_size = self->root->num_leaves;
  if (new_size < old_size // overflow
      || ((MAX_LABEL + 1 != 0) && new_size > MAX_LABEL + 1)) {
    fprintf(stderr, "OM data structure is full!\n");
    exit(1);
  } else {
    //RDTOOL_INTERVAL_BEGIN(OM_REBALANCE);
    om_rebalance(self);
    //    RDTOOL_INTERVAL_END(OM_REBALANCE);
    //    printf("Base's tl level: %zu\n", g_base->list->above->level);
  }
  //  RDTOOL_INTERVAL_END(OM_RELABEL);
}
