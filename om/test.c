#include <stdio.h>
#include <assert.h>

#define TEST
#include "om.c"

#define ck_assert assert
#define ck_assert_ptr_eq(x,y) assert((x) == (y))
#define ck_assert_ptr_ne(x,y) assert((x) != (y))

size_t g_marked_array_size;
tl_node** g_marked_array;

int main(void)
{
  MAX_LABEL = (label_t) 7;
  MAX_LEVEL = (label_t) 3;
  NODE_INTERVAL = (label_t) 2; // 2 / log 1
  SUBLIST_SIZE = (label_t) 3;

  om* t = om_new();

  bl_node* n0 = om_insert_initial(t);
  bl_node* n2 = om_insert(t, n0);
  g_marked_array_size = 1;
  g_marked_array = malloc(sizeof(tl_node*));
  g_marked_array[0] = n0->list->above;
  om_relabel(t, g_marked_array, g_marked_array_size);

  ck_assert(n0->list->above->parent == n2->list->above->parent);
  ck_assert(n0->list->above->parent->parent == t->root);
  ck_assert(n0->list != n2->list);
  ck_assert(n0->list->above != n2->list->above);

  bl_node* n1 = om_insert(t, n0);
  ck_assert(n1);
  g_marked_array_size = 1;
  g_marked_array[0] = n0->list->above;
  om_relabel(t, g_marked_array, g_marked_array_size);

  /* ck_assert(om_precedes(n0, n1) == true); */
  /* ck_assert(om_precedes(n0, n2) == true); */
  /* ck_assert(om_precedes(n1, n2) == true); */
  
  /* ck_assert_ptr_ne(n0->list, n1->list); */
  /* ck_assert_ptr_ne(n1->list, n2->list); */

  /* tl_node* top_parent0 = n0->list->above->parent; */
  /* tl_node* top_parent1 = n1->list->above->parent; */
  /* tl_node* top_parent2 = n2->list->above->parent; */

  /* ck_assert_ptr_eq(top_parent0, top_parent1); */
  /* ck_assert_ptr_eq(top_parent0->parent, t->root); */

  om_free(t);
  free(g_marked_array);
  return 0;
}
