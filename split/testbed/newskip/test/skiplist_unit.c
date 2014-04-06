#include <stdlib.h>
#include <check.h>

#include "../skiplist.c"

Skiplist* core_list;
// checked (run after fork, for every unit test)
void core_setup(void)
{
  core_list = skiplist_create();
}

// checked (run after fork, for every unit test)
void core_teardown(void)
{
  skiplist_free(core_list);
}

START_TEST (test_skiplist_create)
{
  Skiplist *s = core_list;
  ck_assert_uint_eq(skiplist_size(s), 0);
  ck_assert_int_eq(s->level, 0); // ?
  ck_assert(s->hdr != NULL);
}
END_TEST


START_TEST (test_skiplist_find)
{
  int d = 42;
  Skiplist *s = core_list;

  // Manual insert
  struct Node* n = malloc(sizeof(struct Node));
  n->data = d;
  s->hdr->forward[0] = n;

  struct Node* t = skiplist_find(s, d);
  ck_assert(t == n);
  ck_assert_uint_eq(t->data, d);
}
END_TEST

START_TEST (test_skiplist_insert)
{
  int d = 42;
  Skiplist *s = core_list;

  skiplist_insert(s, d);
  ck_assert_uint_eq(skiplist_size(s), 1);
  ck_assert(skiplist_find(s, d) != NULL);
}
END_TEST

/* START_TEST (test_skiplist_find_absent) */
/* { */

/* } */
/* END_TEST */

/* START_TEST (test_skiplist_delete) */
/* { */
/*   return; */
/* } */
/* END_TEST */


Suite* skiplist_suite(void)
{
  Suite* s = suite_create("Skiplist");

  TCase *tc_core = tcase_create("Core");
  tcase_add_checked_fixture(tc_core, core_setup, core_teardown);
  tcase_add_test(tc_core, test_skiplist_create);
  tcase_add_test(tc_core, test_skiplist_insert);
  tcase_add_test(tc_core, test_skiplist_find);

  suite_add_tcase(s, tc_core);

  return s;
}

int main (void)
{
  int number_failed;
  Suite *s = skiplist_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_ENV);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
