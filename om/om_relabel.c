// The relabel-specific code for order-maintenance.

int too_heavy(tl_node* n)
{
  long height = 64; // @todo How to actually get? We don't normally store
                    // this, and it could have changed with the
                    // inserts from the lower-level tries...
  double threshold = 0.75 + 0.25 * (n->level / (double)height );
  double density = n->size / (1 << (MAX_LEVEL - n->level));
  return density <= threshold;
}

void rebuild_recursive(tl_node* n, tl_node** array, int left, int right);

void build_array_of_leaves(tl_node* n, tl_node** array,
                           size_t left_index, size_t right_index,
                           label_t left_label, label_t right_label)
{
  size_t max_size = 1 << n->level;
  label_t spacing = max_size / n->size;
  assert(n->left);
  
  if (n->left->level < MAX_LEVEL) {
    size_t index_end = (n->right) ? right_index - n->right->size : right_index;
    label_t label_end = (n->right) ? right_label - (n->right->size * spacing) : right_label;
    build_array_of_leaves(n->left, array,
                          left_index, index_end,
                          left_label, label_end);
    tl_node_free(n->left);
  } else {
    array[left_index] = n->left;
    n->left->label = left_label;
  }

  if (!n->right) return;

  if (n->right->level < MAX_LEVEL) {
    size_t index_start = left_index + n->left->size + 1;
    label_t label_start = left_index + (n->left->size * spacing) + 1;
    build_array_of_leaves(n->right, array,
                          index_start, right_index,
                          label_start, right_label);
    tl_node_free(n->right);
  } else {
    array[right_index] = n->right;
    n->right->label = right_label;
  }
}

// Rebuild a subtree rooted at node n.
void rebuild(tl_node* n)
{
  tl_node** array = malloc(n->size * sizeof(tl_node*));

  // will remove extra scaffolding
  build_array_of_leaves(n, array, 0, n->size,
                        0, MAX_LABEL);

  // Now we can either build bottom up or top down.
  // Top down
  rebuild_recursive(n, array, 0, n->size);
}

void rebuild_recursive(tl_node* n, tl_node** array, int left, int right)
{
  assert(n->left == NULL && n->right == NULL);

  int range = right - left;
  if (range > 2) {
    n->left = tl_node_new(); /// @todo do these need initialization?
    n->left->needs_rebalance = 0;
    n->right = tl_node_new();
    n->right->needs_rebalance = 0;
    
    rebuild_recursive(n->left, array, left, right / 2);
    rebuild_recursive(n->right, array, (right / 2) + 1, right);
  } else {
    n->left = array[left];
    if (left != right) n->right = array[right];
  }

}

  // 3. Walk down again in parallel until we find that density is bad
  //    Then rebalance, similar to the lower-level rebalance
  //    Parallel prefix (to get ptr to leaf) on leaves (we have an
  //    array of these)
void recursive_rebalance(tl_node* n)
{
  if (n->level < MAX_LEVEL && n->needs_rebalance) {
    n->needs_rebalance = 0;
    if (n->left->needs_rebalance || n->right->needs_rebalance) {
      /*spawn*/ recursive_rebalance(n->left);
      recursive_rebalance(n->right);
      /*sync;*/
    } else {
      rebuild(n);
    }
  }
}


// Assume lower-level tries have been split and already inserted into
// the top-level trie
void rebalance(om* self)
{
  // Theoretical version:
  // 1. Walk up the tree, marking that paths have changed (not exact
  // sizes -- would need some lock free protocol)
  // 2. Walk down the marked paths in parallel, getting correct counts
  //
  // Practical version:
  // 1&2: Walk up tree, updating sizes

  int num_split = g_marked_array_size;
  /*parallel*/ for (int i = 0; i < num_split; ++i) {
    tl_node* current = g_marked_array[i];
    size_t additional_size = current->size - 1;
    
    current = current->parent;
    while (current) {
      __sync_fetch_and_add(&current->size, additional_size);
      if (too_heavy(current)) current->needs_rebalance = 1;
      current = current->parent;
    }
  }


  // 3. Walk down again in parallel until we find that density is bad
  //    Then rebalance, similar to the lower-level rebalance
  //    Parallel prefix (to get ptr to leaf) on leaves (we have an
  //    array of these)
  recursive_rebalance(self->root);
}
