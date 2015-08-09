#include <stdio.h>

#include <check.h>

#define TEST
#include "om.c"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

om* g_om = NULL;
void setup(void) { g_om = om_new(); }
void teardown(void) { om_free(g_om); g_om = NULL; }

START_TEST(test_create)
{
  om* t = om_new();

  ck_assert_ptr_ne(t, NULL);
  ck_assert_ptr_ne(t->root, NULL);
  ck_assert_ptr_eq(t->head, NULL); ck_assert_ptr_eq(t->tail, NULL);

  ck_assert(t->root->size == 1);
  ck_assert_ptr_eq(t->root->left, NULL); ck_assert_ptr_eq(t->root->right, NULL);

  om_free(t);
}
END_TEST

START_TEST(test_insert_single)
{
  om* t = om_new();

  bl_node* n = om_insert_initial(t);

  ck_assert_ptr_ne(n, NULL);
  ck_assert_ptr_ne(n->list, NULL);
  ck_assert(n->list->size == 1);
  ck_assert_msg(n->label == 0, "Label is %lu, should be 0.\n", n->label);

  //  ck_assert(t->size == 1);
  ck_assert_ptr_ne(t->head, NULL);
  ck_assert_ptr_ne(t->tail, NULL);

  om_free(t);
}
END_TEST

START_TEST(test_insert_several)
{
  om* t = om_new();

  bl_node* n0 = om_insert_initial(t);
  bl_node* n2 = om_insert(t, n0);
  bl_node* n1 = om_insert(t, n0);

  ck_assert_ptr_ne(n0, NULL);
  ck_assert_ptr_ne(n1, NULL);
  ck_assert_ptr_ne(n2, NULL);

  ck_assert(om_precedes(n0, n1) == true);
  ck_assert(om_precedes(n0, n2) == true);
  ck_assert(om_precedes(n1, n2) == true);
  ck_assert(om_precedes(n1, n0) == false);
  
  om_free(t);
}
END_TEST

START_TEST(test_insert_overflow)
{
  MAX_LABEL = (label_t) 3;
  MAX_LEVEL = (label_t) 2;
  NODE_INTERVAL = (label_t)2; // 2 / log 1
  SUBLIST_SIZE = (label_t)1;

  om* t = om_new();

  bl_node* n0 = om_insert_initial(t); // 0
  bl_node* n3 = om_insert(t, n0); // 2
  bl_node* n2 = om_insert(t, n0); // 1
  bl_node* n1 = om_insert(t, n0); // fail
  
  ck_assert_ptr_ne(n0, NULL);
  ck_assert_ptr_eq(n1, NULL);
  ck_assert_ptr_ne(n2, NULL);
  ck_assert_ptr_ne(n3, NULL);

  ck_assert(om_precedes(n0, n2) == true);
  ck_assert(om_precedes(n0, n3) == true);
  ck_assert(om_precedes(n2, n3) == true);

  ck_assert_ptr_eq(n0->list, n2->list);
  ck_assert_ptr_eq(n2->list, n3->list);
  ck_assert_ptr_eq(n3->list, n0->list);

  om_free(t);
}
END_TEST

START_TEST(test_relabel_simple)
{
  MAX_LABEL = (label_t) 3;
  MAX_LEVEL = (label_t) 2;
  NODE_INTERVAL = (label_t) 2; // 2 / log 1
  SUBLIST_SIZE = (label_t) 1;

  om* t = om_new();

  bl_node* n0 = om_insert_initial(t); // 0
  bl_node* n3 = om_insert(t, n0); // 2
  bl_node* n2 = om_insert(t, n0); // 1

  g_marked_array_size = 1;
  g_marked_array = malloc(sizeof(tl_node*));
  g_marked_array[0] = n0->list->above;

  relabel(t, g_marked_array, g_marked_array_size);
  bl_node* n1 = om_insert(t, n0);

  ck_assert(om_precedes(n0, n1) == true);
  ck_assert(om_precedes(n0, n2) == true);
  ck_assert(om_precedes(n1, n2) == true);
  ck_assert(om_precedes(n0, n3) == true);
  ck_assert(om_precedes(n2, n3) == true);

  ck_assert_ptr_eq(n0->list, n1->list);
  ck_assert_ptr_ne(n0->list, n2->list);
  ck_assert_ptr_ne(n2->list, n3->list);
  ck_assert_ptr_ne(n3->list, n0->list);

  om_free(t);
}
END_TEST

START_TEST(test_relabel_overflow )
{
  MAX_LABEL = (label_t) 3;
  MAX_LEVEL = (label_t) 2;
  NODE_INTERVAL = (label_t) 2; // 2 / log 1
  SUBLIST_SIZE = (label_t) 1;
}
END_TEST

START_TEST(test_rebalance)
{
  MAX_LABEL = (label_t) 3;
  MAX_LEVEL = (label_t) 2;
  NODE_INTERVAL = (label_t) 2; // 2 / log 1
  SUBLIST_SIZE = (label_t) 1;
}
END_TEST

Suite* om_suite(void)
{
  Suite* s = suite_create("Om (top list) tests");

  TCase* tc_basic = tcase_create("OM creation and destruction");
  tcase_add_test(tc_basic, test_create);
  suite_add_tcase(s, tc_basic);

  TCase* tc_insert = tcase_create("OM insertion");
  tcase_add_test(tc_insert, test_insert_single);
  tcase_add_test(tc_insert, test_insert_several);
  tcase_add_test(tc_insert, test_insert_overflow);
  tcase_add_test(tc_insert, test_relabel_simple);
  tcase_add_test(tc_insert, test_relabel_overflow);
  tcase_add_test(tc_insert, test_rebalance);
  suite_add_tcase(s, tc_insert);

  //  tcase_add_checked_fixture(tc_inserts, setup_inserts, teardown_inserts);
  
  return s;
}

int main(void)
{
  int num_failed = 0;
  Suite* s = om_suite();
  SRunner* sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  num_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

