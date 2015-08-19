// The relabel-specific code for order-maintenance.

#include <string.h> // memcpy

#include "blist_split.c"

#define FAST_LOG2(x) (sizeof(label_t)*8 - 1 - __builtin_clzl((label_t)(x)))
#define FAST_LOG2_CEIL(x) (((x) - (1 << FAST_LOG2(x))) ? FAST_LOG2(x) + 1 : FAST_LOG2(x))
#define parfor for
#define spawn
#define sync

static inline double density(tl_node* n)
{
  /// @todo I've changed the size to mean the number of leaves...does
  /// this change this formula?
  return n->size / (double)(1 << (MAX_LEVEL - n->level));
}

int too_heavy(tl_node* n, size_t height)
{
  double threshold = 0.75 + 0.25 * (n->level / (double)height );
  return density(n) >= threshold;
}

tl_node* rebuild_recursive(tl_node* n, tl_node** array,
                           size_t left_index, size_t right_index,
                           label_t left_lab, label_t right_lab);

label_t build_array_of_leaves(tl_node* n, tl_node** array,
                           size_t left_index, size_t right_index)
{
  if (!n) return 0;
  label_t left_most_label;

  if (n->level == MAX_LEVEL) {
    left_most_label = n->label;
    size_t index = left_index;
    if (n->size == 1) {
      array[index] = n;
      assert(index + 1 == right_index);
    } else {
      tl_node* current = n->split_nodes;

      while (current) {
        array[index] = current;
        current = current->next;
        index++;
      }
      assert(index == right_index);
    }
  } else {
    assert(n->left);
    size_t mid = left_index + n->left->size;
    left_most_label = spawn build_array_of_leaves(n->left, array,
                                                  left_index, mid);
    build_array_of_leaves(n->right, array, mid, right_index);
    sync;
    n->left = n->right = NULL;
  }

  // We don't want to free the root of this subtree...
  if (n->needs_rebalance) tl_node_free(n);
  return left_most_label;
}

// Rebuild a subtree rooted at node n.
void rebuild_subtree(tl_node* n)
{
  assert(n->level < MAX_LEVEL);
  assert(n->needs_rebalance == 0);
  size_t array_size = n->size;
  tl_node** array = malloc(array_size * sizeof(tl_node*));
  
  // will remove extra scaffolding
  label_t left_lab = build_array_of_leaves(n, array, 0, array_size);
  n->left = n->right = NULL;
  
  label_t right_lab = n->label + 1;
  label_t spacing = (right_lab - left_lab) / array_size;
  
  size_t mid_index = array_size / 2;
  label_t mid_lab = left_lab + spacing * mid_index;

  n->left = spawn rebuild_recursive(n, array, 0, mid_index,
                                    left_lab, mid_lab);
  n->right = rebuild_recursive(n, array, mid_index, array_size,
                               mid_lab, right_lab);
  sync;
  free(array);
}

tl_node* rebuild_recursive(tl_node* parent, tl_node** array,
                           size_t left_index, size_t right_index,
                           label_t left_lab, label_t right_lab)
{
  int size = right_index - left_index;
  tl_node* n = (size == 1) ? array[left_index] : tl_node_new();
  n->size = size;
  n->parent = parent;
  n->needs_rebalance = 0;

  if (size == 1) {
    assert(left_lab < right_lab);
    n->label = left_lab;
    n->level = MAX_LEVEL;
    if (left_index > 0) {
      assert(n->label > 0);
      n->prev = array[left_index - 1];
      array[left_index - 1]->next = n;
    }
  } else {
    n->level = n->parent->level + 1;;

    size_t mid_index = left_index + size / 2;
    label_t spacing = (right_lab - left_lab) / size;
    label_t mid_lab = left_lab + spacing * (mid_index - left_index);
    n->left = spawn rebuild_recursive(n, array, left_index, mid_index,
                                            left_lab, mid_lab);
    n->right = rebuild_recursive(n, array, mid_index, right_index,
                                       mid_lab, right_lab);
    sync;
    
    n->label = array[right_index - 1]->label;
  }
  
  return n;
}

void rebuild_leaf(tl_node* n)
{
  assert(n->level == MAX_LEVEL && n->parent && n->size > 1);
  assert(n->left == NULL && n->right == NULL);
  label_t left_lab = n->label;
  label_t right_lab;
  if (n->parent->right == n) right_lab = n->parent->label;
  else {
    right_lab = left_lab + ((n->parent->label - left_lab) / 2);
  }
  assert(n->size == right_lab - left_lab + 1);

  size_t array_size = n->size;
  tl_node** array = malloc(array_size * sizeof(tl_node*));
  
  // will remove extra scaffolding
  assert(left_lab == build_array_of_leaves(n, array, 0, array_size));
  
  // Change this node to be an internal node.
  n->label = right_lab;
  n->level = n->parent->level + 1;
  
  right_lab++; // we pass in a higher label than we will use.
  label_t spacing = (right_lab - left_lab) / n->size;
  size_t mid_index = n->size / 2;
  label_t mid_lab = left_lab + spacing * mid_index;

  n->left = spawn rebuild_recursive(n, array, 0, mid_index,
                                    left_lab, mid_lab);
  n->right = rebuild_recursive(n, array, mid_index, n->size,
                               mid_lab + spacing, right_lab + 1);
  sync;

}

void recursive_rebalance(tl_node* n, size_t height)
{
  if (!n || !n->needs_rebalance) return; // stop here
  n->needs_rebalance = 0;

  if (n->level == MAX_LEVEL) return rebuild_leaf(n);
  assert(n->left);
  if (too_heavy(n, height)) return rebuild_subtree(n);
  
  spawn recursive_rebalance(n->left, height);
  recursive_rebalance(n->right, height);
}


static inline void rebalance(om* self)
{
  recursive_rebalance(self->root, self->height);
}

void om_relabel(om* self, tl_node** heavy_lists, size_t num_heavy_lists)
{
  parfor (int i = 0; i < num_heavy_lists; ++i) {
    tl_node* current = heavy_lists[i];
    
    // Split into several sublists.
    size_t num_split_lists = split(current->below);//, &new_lists);
    
    // We can tell a node is split by its level being MAX_LEVEL and it
    // needs rebalance
    assert(current->level == MAX_LEVEL);

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
  }
  rebalance(self);
}
