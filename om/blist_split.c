static size_t split(blist* self, tl_node*** lists)
{
  size_t original_size = bl_size(self);
  assert(original_size > SUBLIST_SIZE);
  size_t array_size = original_size / SUBLIST_SIZE;
  if (original_size % SUBLIST_SIZE != 0)
    array_size++;
  size_t num_lists = 0;
  *lists = malloc(array_size * sizeof(tl_node*));

  node* current_node = self->head;
  while (current_node) {
    tl_node* n = tl_node_new();
    (*lists)[num_lists] = n;
    blist* list = bl_new();
    n->below = list;
    list->above = n;
    
    current_node->prev = NULL;
    label_t current_label = 0;
    size_t list_size = 0;
    while (current_node && list_size < SUBLIST_SIZE) {
      node* next = current_node->next;
      current_node->label = current_label;
      insert_internal(list, current_node->prev, current_node);

      current_node = next;
      current_label += NODE_INTERVAL;
      list_size++;
    }
    num_lists++;
  }

  // Free the sublist itself, but not its nodes!
  self->head = self->tail = NULL;
  free(self);

  assert(num_lists == array_size);
  return num_lists;
}
