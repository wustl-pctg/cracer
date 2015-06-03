
/* This is the translated c code from the OM_DS_TREE.cilk file */

#include "OM_DS_TREE.h"

static unsigned MAX_NUMBER= ~0;
static int INT_BIT_SIZE= 32;
static int HALF_INT_BIT_SIZE= 16;
static int lg_HALF_INT_BIT_SIZE= 4;
static double OVERFLOW_CONSTANT= 1.30;
static unsigned rebuild_tree_count= 0;
static unsigned PARALLEL_THRESHOLD= 25000;


void print_rebuild_tree(void)
{
 printf ( "Rebuld tree count: %i\n", rebuild_tree_count);
}

struct _cilk_check_subtree_correctness_frame{CilkStackFrame header;struct{Internal_Node*x;}scope0;};struct _cilk_check_subtree_correctness_args{Internal_Node*x;};static void _cilk_check_subtree_correctness_slow(CilkWorkerState*const _cilk_ws,struct _cilk_check_subtree_correctness_frame*_cilk_frame);static CilkProcInfo _cilk_check_subtree_correctness_sig[]={{0,sizeof(struct _cilk_check_subtree_correctness_frame),_cilk_check_subtree_correctness_slow,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
# 39 "OM_DS_TREE.cilk"
void check_subtree_correctness (CilkWorkerState*const _cilk_ws,Internal_Node*x){struct _cilk_check_subtree_correctness_frame*_cilk_frame;{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_check_subtree_correctness_frame), _cilk_check_subtree_correctness_sig); };{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };{

 if (x->lvl == 0)
 {
  ((x->bl!= ((void *)0))? (void) (0) : __assert_fail ("x->bl != ((void *)0)", "OM_DS_TREE.cilk", 43, __PRETTY_FUNCTION__));
  ((x->bl->internal== x)? (void) (0) : __assert_fail ("x->bl->internal == x", "OM_DS_TREE.cilk", 44, __PRETTY_FUNCTION__));
  ((x->left== ((void *)0))? (void) (0) : __assert_fail ("x->left == ((void *)0)", "OM_DS_TREE.cilk", 45, __PRETTY_FUNCTION__));
  ((x->right== ((void *)0))? (void) (0) : __assert_fail ("x->right == ((void *)0)", "OM_DS_TREE.cilk", 46, __PRETTY_FUNCTION__));
  ((x->base<= x->parent->right->base)? (void) (0) : __assert_fail ("x->base <= x->parent->right->base", "OM_DS_TREE.cilk", 47, __PRETTY_FUNCTION__));
  ((x->base>= x->parent->left->base)? (void) (0) : __assert_fail ("x->base >= x->parent->left->base", "OM_DS_TREE.cilk", 48, __PRETTY_FUNCTION__));
  ((x->bl->tag== x->base)? (void) (0) : __assert_fail ("x->bl->tag == x->base", "OM_DS_TREE.cilk", 49, __PRETTY_FUNCTION__));
  {{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
 }
 else
 {
  ((x->left)? (void) (0) : __assert_fail ("x->left", "OM_DS_TREE.cilk", 54, __PRETTY_FUNCTION__));
  ((x->right)? (void) (0) : __assert_fail ("x->right", "OM_DS_TREE.cilk", 55, __PRETTY_FUNCTION__));

  ((x->left->base== x->base)? (void) (0) : __assert_fail ("x->left->base == x->base", "OM_DS_TREE.cilk", 57, __PRETTY_FUNCTION__));
  ((x->right->base> x->base)? (void) (0) : __assert_fail ("x->right->base > x->base", "OM_DS_TREE.cilk", 58, __PRETTY_FUNCTION__));
  ((x->left->parent== x)? (void) (0) : __assert_fail ("x->left->parent == x", "OM_DS_TREE.cilk", 59, __PRETTY_FUNCTION__));
  ((x->right->parent== x)? (void) (0) : __assert_fail ("x->right->parent == x", "OM_DS_TREE.cilk", 60, __PRETTY_FUNCTION__));
  if (x->num_children == 2)
  {
   ((x->left->lvl== 0)? (void) (0) : __assert_fail ("x->left->lvl == 0", "OM_DS_TREE.cilk", 63, __PRETTY_FUNCTION__));
   ((x->right->lvl== 0)? (void) (0) : __assert_fail ("x->right->lvl == 0", "OM_DS_TREE.cilk", 64, __PRETTY_FUNCTION__));
  }
  if (x->left->lvl != 0 && x->right->lvl != 0)
  {
   ((x->left->num_children + x->right->num_children== x->num_children)? (void) (0) : __assert_fail ("x->left->num_children + x->right->num_children == x->num_children", "OM_DS_TREE.cilk", 68, __PRETTY_FUNCTION__));
  }
  else if ((x->left->lvl!= 0) ^ (x->right->lvl!= 0))
  {
   if (x->left->lvl == 0)
   {
    ((x->right->num_children + 1== x->num_children)? (void) (0) : __assert_fail ("x->right->num_children + 1 == x->num_children", "OM_DS_TREE.cilk", 74, __PRETTY_FUNCTION__));
   }
   if (x->right->lvl == 0)
   {
    ((x->left->num_children + 1== x->num_children)? (void) (0) : __assert_fail ("x->left->num_children + 1 == x->num_children", "OM_DS_TREE.cilk", 78, __PRETTY_FUNCTION__));
   }
  }
  ((x->num_children!= 1)? (void) (0) : __assert_fail ("x->num_children != 1", "OM_DS_TREE.cilk", 81, __PRETTY_FUNCTION__));
# 85 "OM_DS_TREE.cilk"
  if (x->num_children> PARALLEL_THRESHOLD)
   {_cilk_frame->header.entry=1;_cilk_frame->scope0.x=x;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };check_subtree_correctness(_cilk_ws,x->left);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  else
  {
   { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
  }
  {_cilk_frame->header.entry=3;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };check_subtree_correctness(_cilk_ws,x->right);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
  {{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
# 100 "OM_DS_TREE.cilk"
  {{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
 }
}}
# 39 "OM_DS_TREE.cilk"
static void _cilk_check_subtree_correctness_slow(CilkWorkerState*const _cilk_ws,struct _cilk_check_subtree_correctness_frame*_cilk_frame){Internal_Node*x;{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header)); };switch (_cilk_frame->header.entry) {case 1: goto _cilk_sync1;case 2: goto _cilk_sync2;case 3: goto _cilk_sync3;case 4: goto _cilk_sync4;}x=_cilk_frame->scope0.x;{

 if (x->lvl == 0)
 {
  ((x->bl!= ((void *)0))? (void) (0) : __assert_fail ("x->bl != ((void *)0)", "OM_DS_TREE.cilk", 43, __PRETTY_FUNCTION__));
  ((x->bl->internal== x)? (void) (0) : __assert_fail ("x->bl->internal == x", "OM_DS_TREE.cilk", 44, __PRETTY_FUNCTION__));
  ((x->left== ((void *)0))? (void) (0) : __assert_fail ("x->left == ((void *)0)", "OM_DS_TREE.cilk", 45, __PRETTY_FUNCTION__));
  ((x->right== ((void *)0))? (void) (0) : __assert_fail ("x->right == ((void *)0)", "OM_DS_TREE.cilk", 46, __PRETTY_FUNCTION__));
  ((x->base<= x->parent->right->base)? (void) (0) : __assert_fail ("x->base <= x->parent->right->base", "OM_DS_TREE.cilk", 47, __PRETTY_FUNCTION__));
  ((x->base>= x->parent->left->base)? (void) (0) : __assert_fail ("x->base >= x->parent->left->base", "OM_DS_TREE.cilk", 48, __PRETTY_FUNCTION__));
  ((x->bl->tag== x->base)? (void) (0) : __assert_fail ("x->bl->tag == x->base", "OM_DS_TREE.cilk", 49, __PRETTY_FUNCTION__));
  {{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
 }
 else
 {
  ((x->left)? (void) (0) : __assert_fail ("x->left", "OM_DS_TREE.cilk", 54, __PRETTY_FUNCTION__));
  ((x->right)? (void) (0) : __assert_fail ("x->right", "OM_DS_TREE.cilk", 55, __PRETTY_FUNCTION__));

  ((x->left->base== x->base)? (void) (0) : __assert_fail ("x->left->base == x->base", "OM_DS_TREE.cilk", 57, __PRETTY_FUNCTION__));
  ((x->right->base> x->base)? (void) (0) : __assert_fail ("x->right->base > x->base", "OM_DS_TREE.cilk", 58, __PRETTY_FUNCTION__));
  ((x->left->parent== x)? (void) (0) : __assert_fail ("x->left->parent == x", "OM_DS_TREE.cilk", 59, __PRETTY_FUNCTION__));
  ((x->right->parent== x)? (void) (0) : __assert_fail ("x->right->parent == x", "OM_DS_TREE.cilk", 60, __PRETTY_FUNCTION__));
  if (x->num_children == 2)
  {
   ((x->left->lvl== 0)? (void) (0) : __assert_fail ("x->left->lvl == 0", "OM_DS_TREE.cilk", 63, __PRETTY_FUNCTION__));
   ((x->right->lvl== 0)? (void) (0) : __assert_fail ("x->right->lvl == 0", "OM_DS_TREE.cilk", 64, __PRETTY_FUNCTION__));
  }
  if (x->left->lvl != 0 && x->right->lvl != 0)
  {
   ((x->left->num_children + x->right->num_children== x->num_children)? (void) (0) : __assert_fail ("x->left->num_children + x->right->num_children == x->num_children", "OM_DS_TREE.cilk", 68, __PRETTY_FUNCTION__));
  }
  else if ((x->left->lvl!= 0) ^ (x->right->lvl!= 0))
  {
   if (x->left->lvl == 0)
   {
    ((x->right->num_children + 1== x->num_children)? (void) (0) : __assert_fail ("x->right->num_children + 1 == x->num_children", "OM_DS_TREE.cilk", 74, __PRETTY_FUNCTION__));
   }
   if (x->right->lvl == 0)
   {
    ((x->left->num_children + 1== x->num_children)? (void) (0) : __assert_fail ("x->left->num_children + 1 == x->num_children", "OM_DS_TREE.cilk", 78, __PRETTY_FUNCTION__));
   }
  }
  ((x->num_children!= 1)? (void) (0) : __assert_fail ("x->num_children != 1", "OM_DS_TREE.cilk", 81, __PRETTY_FUNCTION__));
# 85 "OM_DS_TREE.cilk"
  if (x->num_children> PARALLEL_THRESHOLD)
   {_cilk_frame->header.entry=1;_cilk_frame->scope0.x=x;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };check_subtree_correctness(_cilk_ws,x->left);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync1:x=_cilk_frame->scope0.x;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  else
  {
   {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=2;_cilk_frame->scope0.x=x;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync2:x=_cilk_frame->scope0.x;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  }
  {_cilk_frame->header.entry=3;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };check_subtree_correctness(_cilk_ws,x->right);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync3:;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=4;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync4:;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  {{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
# 100 "OM_DS_TREE.cilk"
  {{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
 }
}}
# 39 "OM_DS_TREE.cilk"
static void _cilk_check_subtree_correctness_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v){(void)_cilk_ws;(void)_cilk_procargs_v;check_subtree_correctness(_cilk_ws,((struct _cilk_check_subtree_correctness_args*)_cilk_procargs_v)->x);
# 102 "OM_DS_TREE.cilk"
}
# 39 "OM_DS_TREE.cilk"
void mt_check_subtree_correctness(CilkContext*const context,Internal_Node*x){struct _cilk_check_subtree_correctness_args*_cilk_procargs;_cilk_procargs=(struct _cilk_check_subtree_correctness_args*)Cilk_malloc_fixed(sizeof(struct _cilk_check_subtree_correctness_args));_cilk_procargs->x=x;Cilk_start(context,_cilk_check_subtree_correctness_import,_cilk_procargs,0);Cilk_free(_cilk_procargs);
# 102 "OM_DS_TREE.cilk"
}

struct _cilk_check_tree_correctness_frame{CilkStackFrame header;struct{Internal_Node*x;}scope0;};struct _cilk_check_tree_correctness_args{Internal_Node*x;};static void _cilk_check_tree_correctness_slow(CilkWorkerState*const _cilk_ws,struct _cilk_check_tree_correctness_frame*_cilk_frame);static CilkProcInfo _cilk_check_tree_correctness_sig[]={{0,sizeof(struct _cilk_check_tree_correctness_frame),_cilk_check_tree_correctness_slow,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
# 104 "OM_DS_TREE.cilk"
void check_tree_correctness (CilkWorkerState*const _cilk_ws,Internal_Node*x){struct _cilk_check_tree_correctness_frame*_cilk_frame;{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_check_tree_correctness_frame), _cilk_check_tree_correctness_sig); };{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };{
 while (x->parent)
 {
  x = x->parent;
 }
 if (x->lvl == 0)
 {
  ((x->bl!= ((void *)0))? (void) (0) : __assert_fail ("x->bl != ((void *)0)", "OM_DS_TREE.cilk", 111, __PRETTY_FUNCTION__));
  ((x->left== ((void *)0))? (void) (0) : __assert_fail ("x->left == ((void *)0)", "OM_DS_TREE.cilk", 112, __PRETTY_FUNCTION__));
  ((x->right== ((void *)0))? (void) (0) : __assert_fail ("x->right == ((void *)0)", "OM_DS_TREE.cilk", 113, __PRETTY_FUNCTION__));
  ((x->base<= x->parent->right->base)? (void) (0) : __assert_fail ("x->base <= x->parent->right->base", "OM_DS_TREE.cilk", 114, __PRETTY_FUNCTION__));
  ((x->base>= x->parent->left->base)? (void) (0) : __assert_fail ("x->base >= x->parent->left->base", "OM_DS_TREE.cilk", 115, __PRETTY_FUNCTION__));
  ((x->bl->tag== x->base)? (void) (0) : __assert_fail ("x->bl->tag == x->base", "OM_DS_TREE.cilk", 116, __PRETTY_FUNCTION__));
  {{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
 }
 ((x->left)? (void) (0) : __assert_fail ("x->left", "OM_DS_TREE.cilk", 119, __PRETTY_FUNCTION__));
 ((x->right)? (void) (0) : __assert_fail ("x->right", "OM_DS_TREE.cilk", 120, __PRETTY_FUNCTION__));
 ((x->left->base== x->base)? (void) (0) : __assert_fail ("x->left->base == x->base", "OM_DS_TREE.cilk", 121, __PRETTY_FUNCTION__));
 ((x->left->parent== x)? (void) (0) : __assert_fail ("x->left->parent ==x", "OM_DS_TREE.cilk", 122, __PRETTY_FUNCTION__));
 ((x->right->parent== x)? (void) (0) : __assert_fail ("x->right->parent == x", "OM_DS_TREE.cilk", 123, __PRETTY_FUNCTION__));
 if (x->num_children == 2)
 {
  ((x->left->lvl== 0)? (void) (0) : __assert_fail ("x->left->lvl == 0", "OM_DS_TREE.cilk", 126, __PRETTY_FUNCTION__));
  ((x->right->lvl== 0)? (void) (0) : __assert_fail ("x->right->lvl == 0", "OM_DS_TREE.cilk", 127, __PRETTY_FUNCTION__));
 }
 if (x->left->lvl != 0 && x->right->lvl != 0)
 {
  ((x->left->num_children + x->right->num_children== x->num_children)? (void) (0) : __assert_fail ("x->left->num_children + x->right->num_children == x->num_children", "OM_DS_TREE.cilk", 131, __PRETTY_FUNCTION__));
 }
 else if ((x->left->lvl!= 0) ^ (x->right->lvl!= 0))
 {
  if (x->left->lvl == 0)
  {
   ((x->right->num_children + 1== x->num_children)? (void) (0) : __assert_fail ("x->right->num_children + 1 == x->num_children", "OM_DS_TREE.cilk", 137, __PRETTY_FUNCTION__));
  }
  if (x->right->lvl == 0)
  {
   ((x->left->num_children + 1== x->num_children)? (void) (0) : __assert_fail ("x->left->num_children + 1 == x->num_children", "OM_DS_TREE.cilk", 141, __PRETTY_FUNCTION__));
  }
 }
 ((x->num_children!= 1)? (void) (0) : __assert_fail ("x->num_children != 1", "OM_DS_TREE.cilk", 144, __PRETTY_FUNCTION__));
# 148 "OM_DS_TREE.cilk"
 if (x->num_children> PARALLEL_THRESHOLD)
  {_cilk_frame->header.entry=1;_cilk_frame->scope0.x=x;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };check_subtree_correctness(_cilk_ws,x->left);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
 else
 {
  { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
 }
 {_cilk_frame->header.entry=3;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };check_subtree_correctness(_cilk_ws,x->right);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
 { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
 {{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
# 163 "OM_DS_TREE.cilk"
 {{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
}}
# 104 "OM_DS_TREE.cilk"
static void _cilk_check_tree_correctness_slow(CilkWorkerState*const _cilk_ws,struct _cilk_check_tree_correctness_frame*_cilk_frame){Internal_Node*x;{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header)); };switch (_cilk_frame->header.entry) {case 1: goto _cilk_sync1;case 2: goto _cilk_sync2;case 3: goto _cilk_sync3;case 4: goto _cilk_sync4;}x=_cilk_frame->scope0.x;{
 while (x->parent)
 {
  x = x->parent;
 }
 if (x->lvl == 0)
 {
  ((x->bl!= ((void *)0))? (void) (0) : __assert_fail ("x->bl != ((void *)0)", "OM_DS_TREE.cilk", 111, __PRETTY_FUNCTION__));
  ((x->left== ((void *)0))? (void) (0) : __assert_fail ("x->left == ((void *)0)", "OM_DS_TREE.cilk", 112, __PRETTY_FUNCTION__));
  ((x->right== ((void *)0))? (void) (0) : __assert_fail ("x->right == ((void *)0)", "OM_DS_TREE.cilk", 113, __PRETTY_FUNCTION__));
  ((x->base<= x->parent->right->base)? (void) (0) : __assert_fail ("x->base <= x->parent->right->base", "OM_DS_TREE.cilk", 114, __PRETTY_FUNCTION__));
  ((x->base>= x->parent->left->base)? (void) (0) : __assert_fail ("x->base >= x->parent->left->base", "OM_DS_TREE.cilk", 115, __PRETTY_FUNCTION__));
  ((x->bl->tag== x->base)? (void) (0) : __assert_fail ("x->bl->tag == x->base", "OM_DS_TREE.cilk", 116, __PRETTY_FUNCTION__));
  {{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
 }
 ((x->left)? (void) (0) : __assert_fail ("x->left", "OM_DS_TREE.cilk", 119, __PRETTY_FUNCTION__));
 ((x->right)? (void) (0) : __assert_fail ("x->right", "OM_DS_TREE.cilk", 120, __PRETTY_FUNCTION__));
 ((x->left->base== x->base)? (void) (0) : __assert_fail ("x->left->base == x->base", "OM_DS_TREE.cilk", 121, __PRETTY_FUNCTION__));
 ((x->left->parent== x)? (void) (0) : __assert_fail ("x->left->parent ==x", "OM_DS_TREE.cilk", 122, __PRETTY_FUNCTION__));
 ((x->right->parent== x)? (void) (0) : __assert_fail ("x->right->parent == x", "OM_DS_TREE.cilk", 123, __PRETTY_FUNCTION__));
 if (x->num_children == 2)
 {
  ((x->left->lvl== 0)? (void) (0) : __assert_fail ("x->left->lvl == 0", "OM_DS_TREE.cilk", 126, __PRETTY_FUNCTION__));
  ((x->right->lvl== 0)? (void) (0) : __assert_fail ("x->right->lvl == 0", "OM_DS_TREE.cilk", 127, __PRETTY_FUNCTION__));
 }
 if (x->left->lvl != 0 && x->right->lvl != 0)
 {
  ((x->left->num_children + x->right->num_children== x->num_children)? (void) (0) : __assert_fail ("x->left->num_children + x->right->num_children == x->num_children", "OM_DS_TREE.cilk", 131, __PRETTY_FUNCTION__));
 }
 else if ((x->left->lvl!= 0) ^ (x->right->lvl!= 0))
 {
  if (x->left->lvl == 0)
  {
   ((x->right->num_children + 1== x->num_children)? (void) (0) : __assert_fail ("x->right->num_children + 1 == x->num_children", "OM_DS_TREE.cilk", 137, __PRETTY_FUNCTION__));
  }
  if (x->right->lvl == 0)
  {
   ((x->left->num_children + 1== x->num_children)? (void) (0) : __assert_fail ("x->left->num_children + 1 == x->num_children", "OM_DS_TREE.cilk", 141, __PRETTY_FUNCTION__));
  }
 }
 ((x->num_children!= 1)? (void) (0) : __assert_fail ("x->num_children != 1", "OM_DS_TREE.cilk", 144, __PRETTY_FUNCTION__));
# 148 "OM_DS_TREE.cilk"
 if (x->num_children> PARALLEL_THRESHOLD)
  {_cilk_frame->header.entry=1;_cilk_frame->scope0.x=x;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };check_subtree_correctness(_cilk_ws,x->left);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync1:x=_cilk_frame->scope0.x;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
 else
 {
  {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=2;_cilk_frame->scope0.x=x;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync2:x=_cilk_frame->scope0.x;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
 }
 {_cilk_frame->header.entry=3;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };check_subtree_correctness(_cilk_ws,x->right);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync3:;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
 {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=4;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync4:;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
 {{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
# 163 "OM_DS_TREE.cilk"
 {{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
}}
# 104 "OM_DS_TREE.cilk"
static void _cilk_check_tree_correctness_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v){(void)_cilk_ws;(void)_cilk_procargs_v;check_tree_correctness(_cilk_ws,((struct _cilk_check_tree_correctness_args*)_cilk_procargs_v)->x);
# 164 "OM_DS_TREE.cilk"
}
# 104 "OM_DS_TREE.cilk"
void mt_check_tree_correctness(CilkContext*const context,Internal_Node*x){struct _cilk_check_tree_correctness_args*_cilk_procargs;_cilk_procargs=(struct _cilk_check_tree_correctness_args*)Cilk_malloc_fixed(sizeof(struct _cilk_check_tree_correctness_args));_cilk_procargs->x=x;Cilk_start(context,_cilk_check_tree_correctness_import,_cilk_procargs,0);Cilk_free(_cilk_procargs);
# 164 "OM_DS_TREE.cilk"
}

static double scaffolding_total_time= 0;
# 167 "OM_DS_TREE.cilk"
void print_scaffolding_timing(void)
{
 printf ( "Scaffolding total time(s):%f \n", (scaffolding_total_time/ 1000000l));
}


void create_scaffolding (Internal_Node *x, Internal_Node *y)
{
    unsigned xtag=
         x->bl->tag;
# 175 "OM_DS_TREE.cilk"
    unsigned ytag=

         y->bl->tag;
# 175 "OM_DS_TREE.cilk"
    unsigned ztag=


         ytag;
# 175 "OM_DS_TREE.cilk"
    unsigned lvl_count=
# 179 "OM_DS_TREE.cilk"
              INT_BIT_SIZE;
# 175 "OM_DS_TREE.cilk"
    unsigned bit_counter=
# 180 "OM_DS_TREE.cilk"
                (0x1) << ( INT_BIT_SIZE- 1);
    Internal_Node *new_parent;Internal_Node*iter_node;
# 189 "OM_DS_TREE.cilk"
    if ((y->bl->next))
    {
  ztag = y->bl->next->tag;
  while (bit_counter != 0)
  {

      if ( (( ztag^ ytag)& bit_counter) == bit_counter)
      {
    while ( ((~(xtag^ ytag))& bit_counter) == bit_counter)
    {

        lvl_count--;
        bit_counter = bit_counter >> 1;
    }
    break;
      }

      else if ( (( xtag^ ytag)& bit_counter) == bit_counter)
      {
    while ( ((~(ztag^ ytag))& bit_counter) == bit_counter)
    {

        lvl_count--;
        bit_counter = bit_counter >> 1;
    }

    x = y->bl->next->internal;


    xtag = ztag;
    break;
      }


      lvl_count--;
      bit_counter = bit_counter >> 1;
  }
    }
    else
    {
  while (bit_counter != 0)
  {
      if ( (( xtag^ ytag)& bit_counter) == bit_counter)
    break;


      lvl_count--;
      bit_counter = bit_counter >> 1;
  }
    }


    if (x->parent->lvl < lvl_count)
    {
  new_parent = x->parent;

  while (new_parent->lvl < lvl_count)
  {

      iter_node = new_parent;
      new_parent = new_parent->parent;
  }
# 257 "OM_DS_TREE.cilk"
  if (xtag != ztag)
  {
      new_parent->right = y;
  }
  else
  {
      new_parent->left = y;
  }

  y->parent = new_parent;


  new_parent->lvl = lvl_count;
  new_parent->bl = ((void *)0);


   if (lvl_count == INT_BIT_SIZE)
    new_parent->base =0x0;
  else
   new_parent->base = (xtag>> lvl_count) << lvl_count;


  new_parent->num_children = iter_node->num_children + 1;
    }

    else if (x->parent->lvl > lvl_count)
    {
  new_parent = malloc(sizeof(Internal_Node));
  new_parent->bl = ((void *)0);


  new_parent->parent = x->parent;


  if (xtag != ztag)
  {
      new_parent->right = y;
      new_parent->left = x;
  }
  else
  {
      new_parent->left = y;
      new_parent->right = x;
  }


  if (x->parent->left == x)
      x->parent->left = new_parent;
  else if (x->parent->right == x)
      x->parent->right = new_parent;
  else
  {
      printf ( "Debug: Create scaffolding -x->parent has two children already, or no children.\n");
      ((0)? (void) (0) : __assert_fail ("0", "OM_DS_TREE.cilk", 310, __PRETTY_FUNCTION__));
  }


  x->parent = y->parent = new_parent;


  new_parent->lvl = lvl_count;


   if (lvl_count == INT_BIT_SIZE)
    new_parent->base =0x0;
  else
   new_parent->base = (xtag>> lvl_count) << lvl_count;


  new_parent->num_children = 2;
    }
 else if (lvl_count == INT_BIT_SIZE)
 {
  new_parent = x->parent;
  ((new_parent->lvl== INT_BIT_SIZE)? (void) (0) : __assert_fail ("new_parent->lvl == INT_BIT_SIZE", "OM_DS_TREE.cilk", 331, __PRETTY_FUNCTION__));
  new_parent->num_children = 2;
  new_parent->right = y;
  y->parent = new_parent;
 }
    else
    {
# 342 "OM_DS_TREE.cilk"
  new_parent = malloc(sizeof(Internal_Node));


   if (lvl_count == INT_BIT_SIZE)
    new_parent->base =0x0;
  else
   new_parent->base = (xtag>> lvl_count) << lvl_count;

  new_parent->parent = x->parent;
  new_parent->lvl = lvl_count;
  new_parent->num_children = 2;
  new_parent->bl = ((void *)0);


  if (x->parent->left == x)
      x->parent->left = new_parent;
  else if (x->parent->right == x)
      x->parent->right = new_parent;
  else
  {
      printf ( "Debug: Create scaffolding -x->parent has two children already, or no children.\n");
      ((0)? (void) (0) : __assert_fail ("0", "OM_DS_TREE.cilk", 363, __PRETTY_FUNCTION__));
  }


  if (xtag != ztag)
      new_parent->right = y;
  else
      new_parent->left = y;

  y->parent = x->parent = new_parent;
    }


    iter_node = new_parent->parent;

    while (iter_node != ((void *)0))
    {
  iter_node->num_children += 1;
  iter_node = iter_node->parent;
    }
# 394 "OM_DS_TREE.cilk"
}

struct _cilk_insert_frame{CilkStackFrame header;struct{OM_Node*x;OM_Node*y;}scope0;struct{OM_DS_BatchRecord*ir;}scope1;};struct _cilk_insert_args{OM_Node*x;OM_Node*y;};static void _cilk_insert_slow(CilkWorkerState*const _cilk_ws,struct _cilk_insert_frame*_cilk_frame);static CilkProcInfo _cilk_insert_sig[]={{0,sizeof(struct _cilk_insert_frame),_cilk_insert_slow,0,0}};
# 396 "OM_DS_TREE.cilk"
void insert (CilkWorkerState*const _cilk_ws,OM_Node*x,OM_Node*y){struct _cilk_insert_frame*_cilk_frame;{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_insert_frame), _cilk_insert_sig); };{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };{
 OM_DS_BatchRecord *ir= malloc(sizeof(OM_DS_BatchRecord));
 ir->x = x;
 ir->y = y;

 Cilk_batchify(_cilk_ws, &batchInsertOp, ((void *)0), ir, sizeof(OM_DS_BatchRecord), ((void *)0));


{{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 396 "OM_DS_TREE.cilk"
static void _cilk_insert_slow(CilkWorkerState*const _cilk_ws,struct _cilk_insert_frame*_cilk_frame){OM_Node*x;OM_Node*y;{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header)); };switch (_cilk_frame->header.entry) {}x=_cilk_frame->scope0.x;y=_cilk_frame->scope0.y;{
 OM_DS_BatchRecord *ir= malloc(sizeof(OM_DS_BatchRecord));
 ir->x = x;
 ir->y = y;

 Cilk_batchify(_cilk_ws, &batchInsertOp, ((void *)0), ir, sizeof(OM_DS_BatchRecord), ((void *)0));


{{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 396 "OM_DS_TREE.cilk"
static void _cilk_insert_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v){(void)_cilk_ws;(void)_cilk_procargs_v;insert(_cilk_ws,((struct _cilk_insert_args*)_cilk_procargs_v)->x,((struct _cilk_insert_args*)_cilk_procargs_v)->y);
# 404 "OM_DS_TREE.cilk"
}
# 396 "OM_DS_TREE.cilk"
void mt_insert(CilkContext*const context,OM_Node*x,OM_Node*y){struct _cilk_insert_args*_cilk_procargs;_cilk_procargs=(struct _cilk_insert_args*)Cilk_malloc_fixed(sizeof(struct _cilk_insert_args));_cilk_procargs->x=x;_cilk_procargs->y=y;Cilk_start(context,_cilk_insert_import,_cilk_procargs,0);Cilk_free(_cilk_procargs);
# 404 "OM_DS_TREE.cilk"
}


struct _cilk_batchInsertOp_frame{CilkStackFrame header;struct{void*dataStruct;void*data;size_t size;void*result;}scope0;struct{int i;OM_DS_BatchRecord*irArray;OM_DS_BatchRecord*ir;}scope1;};struct _cilk_batchInsertOp_args{void*dataStruct;void*data;size_t size;void*result;};static void _cilk_batchInsertOp_slow(CilkWorkerState*const _cilk_ws,struct _cilk_batchInsertOp_frame*_cilk_frame);static CilkProcInfo _cilk_batchInsertOp_sig[]={{0,sizeof(struct _cilk_batchInsertOp_frame),_cilk_batchInsertOp_slow,0,0},{0,0,0,0,0},{0,0,0,0,0}};
# 407 "OM_DS_TREE.cilk"
void batchInsertOp (CilkWorkerState*const _cilk_ws,void*dataStruct,void*data,size_t size,void*result){struct _cilk_batchInsertOp_frame*_cilk_frame;{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_batchInsertOp_frame), _cilk_batchInsertOp_sig); };{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
{
 int i= 0;
 OM_DS_BatchRecord *irArray= (OM_DS_BatchRecord *)data;OM_DS_BatchRecord*ir;

 for (; i< size; i++) {
  ir = &irArray[i];
  {_cilk_frame->header.entry=1;_cilk_frame->scope0.size=size;_cilk_frame->scope1.i=i;_cilk_frame->scope1.irArray=irArray;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };insert_internal(_cilk_ws,ir->x,ir->y);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
 }
{{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 407 "OM_DS_TREE.cilk"
static void _cilk_batchInsertOp_slow(CilkWorkerState*const _cilk_ws,struct _cilk_batchInsertOp_frame*_cilk_frame){void*dataStruct;void*data;size_t size;void*result;{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header)); };switch (_cilk_frame->header.entry) {case 1: goto _cilk_sync1;case 2: goto _cilk_sync2;}dataStruct=_cilk_frame->scope0.dataStruct;data=_cilk_frame->scope0.data;size=_cilk_frame->scope0.size;result=_cilk_frame->scope0.result;
{
 int i= 0;
 OM_DS_BatchRecord *irArray= (OM_DS_BatchRecord *)data;OM_DS_BatchRecord*ir;

 for (; i< size; i++) {
  ir = &irArray[i];
  {_cilk_frame->header.entry=1;_cilk_frame->scope0.size=size;_cilk_frame->scope1.i=i;_cilk_frame->scope1.irArray=irArray;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };insert_internal(_cilk_ws,ir->x,ir->y);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync1:size=_cilk_frame->scope0.size;i=_cilk_frame->scope1.i;irArray=_cilk_frame->scope1.irArray;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=2;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync2:size=_cilk_frame->scope0.size;i=_cilk_frame->scope1.i;irArray=_cilk_frame->scope1.irArray;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
 }
{{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 407 "OM_DS_TREE.cilk"
static void _cilk_batchInsertOp_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v)
{(void)_cilk_ws;(void)_cilk_procargs_v;batchInsertOp(_cilk_ws,((struct _cilk_batchInsertOp_args*)_cilk_procargs_v)->dataStruct,((struct _cilk_batchInsertOp_args*)_cilk_procargs_v)->data,((struct _cilk_batchInsertOp_args*)_cilk_procargs_v)->size,((struct _cilk_batchInsertOp_args*)_cilk_procargs_v)->result);
# 417 "OM_DS_TREE.cilk"
}
# 407 "OM_DS_TREE.cilk"
void mt_batchInsertOp(CilkContext*const context,void*dataStruct,void*data,size_t size,void*result)
{struct _cilk_batchInsertOp_args*_cilk_procargs;_cilk_procargs=(struct _cilk_batchInsertOp_args*)Cilk_malloc_fixed(sizeof(struct _cilk_batchInsertOp_args));_cilk_procargs->dataStruct=dataStruct;_cilk_procargs->data=data;_cilk_procargs->size=size;_cilk_procargs->result=result;Cilk_start(context,_cilk_batchInsertOp_import,_cilk_procargs,0);Cilk_free(_cilk_procargs);
# 417 "OM_DS_TREE.cilk"
}

struct _cilk_insert_internal_frame{CilkStackFrame header;struct{OM_Node*x;OM_Node*y;}scope0;struct{Bottom_List*ds;}scope1;};struct _cilk_insert_internal_args{OM_Node*x;OM_Node*y;};static void _cilk_insert_internal_slow(CilkWorkerState*const _cilk_ws,struct _cilk_insert_internal_frame*_cilk_frame);static CilkProcInfo _cilk_insert_internal_sig[]={{0,sizeof(struct _cilk_insert_internal_frame),_cilk_insert_internal_slow,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
# 419 "OM_DS_TREE.cilk"
void insert_internal (CilkWorkerState*const _cilk_ws,OM_Node*x,OM_Node*y){struct _cilk_insert_internal_frame*_cilk_frame;{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_insert_internal_frame), _cilk_insert_internal_sig); };{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
{
    Bottom_List *ds= x->ds;


    y->ds = ds;

    if (x == ds->tail)
    {

  y->tag = (x->tag>> 1) + (MAX_NUMBER>> 1);


  if ((x->tag& 0x1) == 0x1) y->tag += 1;


  if ((MAX_NUMBER- x->tag) <= 1)
  {
      {_cilk_frame->header.entry=1;_cilk_frame->scope0.x=x;_cilk_frame->scope0.y=y;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };split_bl(_cilk_ws,ds->parent,ds);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
   { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };

      {_cilk_frame->header.entry=3;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };insert_internal(_cilk_ws,x,y);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
   { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
      {{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
  }

  ds->tail = y;
    }
    else
    {

  y->tag = (x->tag>> 1) + (x->next->tag>> 1);


  if ((x->next->tag & x->tag& 0x1) == 0x1) y->tag += 1;
# 457 "OM_DS_TREE.cilk"
  if ((x->next->tag- x->tag) <= 1)
  {
      {_cilk_frame->header.entry=5;_cilk_frame->scope0.x=x;_cilk_frame->scope0.y=y;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };split_bl(_cilk_ws,ds->parent,ds);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
   { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };

      {_cilk_frame->header.entry=7;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };insert_internal(_cilk_ws,x,y);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
   { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
      {{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
  }

    }


    y->prev = x;
    y->next = x->next;
    x->next = y;
    if (y->next != ((void *)0)) y->next->prev = y;

    ds->size += 1;
{{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 419 "OM_DS_TREE.cilk"
static void _cilk_insert_internal_slow(CilkWorkerState*const _cilk_ws,struct _cilk_insert_internal_frame*_cilk_frame){OM_Node*x;OM_Node*y;{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header)); };switch (_cilk_frame->header.entry) {case 1: goto _cilk_sync1;case 2: goto _cilk_sync2;case 3: goto _cilk_sync3;case 4: goto _cilk_sync4;case 5: goto _cilk_sync5;case 6: goto _cilk_sync6;case 7: goto _cilk_sync7;case 8: goto _cilk_sync8;}x=_cilk_frame->scope0.x;y=_cilk_frame->scope0.y;
{
    Bottom_List *ds= x->ds;


    y->ds = ds;

    if (x == ds->tail)
    {

  y->tag = (x->tag>> 1) + (MAX_NUMBER>> 1);


  if ((x->tag& 0x1) == 0x1) y->tag += 1;


  if ((MAX_NUMBER- x->tag) <= 1)
  {
      {_cilk_frame->header.entry=1;_cilk_frame->scope0.x=x;_cilk_frame->scope0.y=y;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };split_bl(_cilk_ws,ds->parent,ds);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync1:x=_cilk_frame->scope0.x;y=_cilk_frame->scope0.y;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
   {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=2;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync2:x=_cilk_frame->scope0.x;y=_cilk_frame->scope0.y;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}

      {_cilk_frame->header.entry=3;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };insert_internal(_cilk_ws,x,y);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync3:;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
   {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=4;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync4:;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
      {{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
  }

  ds->tail = y;
    }
    else
    {

  y->tag = (x->tag>> 1) + (x->next->tag>> 1);


  if ((x->next->tag & x->tag& 0x1) == 0x1) y->tag += 1;
# 457 "OM_DS_TREE.cilk"
  if ((x->next->tag- x->tag) <= 1)
  {
      {_cilk_frame->header.entry=5;_cilk_frame->scope0.x=x;_cilk_frame->scope0.y=y;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };split_bl(_cilk_ws,ds->parent,ds);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync5:x=_cilk_frame->scope0.x;y=_cilk_frame->scope0.y;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
   {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=6;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync6:x=_cilk_frame->scope0.x;y=_cilk_frame->scope0.y;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}

      {_cilk_frame->header.entry=7;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };insert_internal(_cilk_ws,x,y);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync7:;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
   {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=8;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync8:;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
      {{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
  }

    }


    y->prev = x;
    y->next = x->next;
    x->next = y;
    if (y->next != ((void *)0)) y->next->prev = y;

    ds->size += 1;
{{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 419 "OM_DS_TREE.cilk"
static void _cilk_insert_internal_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v)
{(void)_cilk_ws;(void)_cilk_procargs_v;insert_internal(_cilk_ws,((struct _cilk_insert_internal_args*)_cilk_procargs_v)->x,((struct _cilk_insert_internal_args*)_cilk_procargs_v)->y);
# 476 "OM_DS_TREE.cilk"
}
# 419 "OM_DS_TREE.cilk"
void mt_insert_internal(CilkContext*const context,OM_Node*x,OM_Node*y)
{struct _cilk_insert_internal_args*_cilk_procargs;_cilk_procargs=(struct _cilk_insert_internal_args*)Cilk_malloc_fixed(sizeof(struct _cilk_insert_internal_args));_cilk_procargs->x=x;_cilk_procargs->y=y;Cilk_start(context,_cilk_insert_internal_import,_cilk_procargs,0);Cilk_free(_cilk_procargs);
# 476 "OM_DS_TREE.cilk"
}
# 484 "OM_DS_TREE.cilk"
Bottom_List *create_bl (void)
{

    Bottom_List *list= malloc(sizeof((*list)));


    list->head = list->tail = ((void *)0);


    list->size = list->reorder_flag = 0;


 list->internal = ((void *)0);

    return list;
}
# 507 "OM_DS_TREE.cilk"
Top_List *create_tl (void)
{

    Top_List *list= malloc(sizeof((*list)));

    Bottom_List *bl= create_bl();


    list->size = 0;


    list->head = list->tail = ((void *)0);


    first_insert_tl(list, bl);

    return list;
}
# 532 "OM_DS_TREE.cilk"
void first_insert_bl (Bottom_List *ds, OM_Node *y)
{
# 550 "OM_DS_TREE.cilk"
    y->ds = ds;
    ds->head = ds->tail = y;
    y->next = y->prev = ((void *)0);
    ds->size = 1;
    y->tag = 0;
}
# 563 "OM_DS_TREE.cilk"
void first_insert_tl (Top_List *list, Bottom_List *_y)
{
    Internal_Node *y;
    Internal_Node *root= malloc(sizeof(Internal_Node));
# 584 "OM_DS_TREE.cilk"
    _y->parent = list;
    list->head = list->tail = _y;
    _y->tag = 0;

    _y->reorder_flag = 0;
    _y->next = _y->prev = ((void *)0);
    list->size += 1;


    _y->internal = y = malloc(sizeof(Internal_Node));


    y->lvl = 0;


    y->num_children = y->base = 0;


    y->bl = _y;

    root->left = y;
    y->parent = root;
    root->lvl = INT_BIT_SIZE;
    root->num_children = 1;
}
# 616 "OM_DS_TREE.cilk"
void first_insert (Top_List *list, OM_Node *y)
{

    first_insert_bl(list->head, y);
}
# 628 "OM_DS_TREE.cilk"
struct _cilk_insert_tl_frame{CilkStackFrame header;struct{Bottom_List*x;Bottom_List*y;}scope0;struct{Top_List*list;}scope1;};struct _cilk_insert_tl_args{Bottom_List*x;Bottom_List*y;};static void _cilk_insert_tl_slow(CilkWorkerState*const _cilk_ws,struct _cilk_insert_tl_frame*_cilk_frame);static CilkProcInfo _cilk_insert_tl_sig[]={{0,sizeof(struct _cilk_insert_tl_frame),_cilk_insert_tl_slow,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
# 628 "OM_DS_TREE.cilk"
void insert_tl (CilkWorkerState*const _cilk_ws,Bottom_List*x,Bottom_List*y){struct _cilk_insert_tl_frame*_cilk_frame;{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_insert_tl_frame), _cilk_insert_tl_sig); };{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
{

    Top_List *list= x->parent;
# 637 "OM_DS_TREE.cilk"
    if (x == list->tail)
    {

  y->tag = (x->tag>> 1) + (1<< (INT_BIT_SIZE- 1));


  if (MAX_NUMBER - x->tag == 1)
      y->tag = MAX_NUMBER;

    }
    else
    {

  y->tag = (x->tag>> 1) + (x->next->tag>> 1);


  if ((x->next->tag & x->tag& 0x1) == 0x1) y->tag++;
    }


    if ( (x == list->tail&& (MAX_NUMBER== x->tag)) ||
      (x != list->tail&& (x->next->tag - x->tag<= 1)))
    {

  {_cilk_frame->header.entry=1;_cilk_frame->scope0.x=x;_cilk_frame->scope0.y=y;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };rebalance_tl(_cilk_ws,x);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };


  {_cilk_frame->header.entry=3;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };insert_tl(_cilk_ws,x,y);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
    }
    else
    {
# 673 "OM_DS_TREE.cilk"
  y->prev = x;
  y->next = x->next;
  x->next = y;
  if (y->next != ((void *)0))
      y->next->prev = y;
  else
      list->tail = y;


  y->parent = list;
  list->size += 1;

  y->internal = malloc(sizeof(Internal_Node));

  y->internal->lvl = 0;


  y->internal->num_children = 0;
  y->internal->base = y->tag;
  y->internal->parent = y->internal->left = y->internal->right = ((void *)0);


  y->internal->bl = y;

  create_scaffolding(x->internal, y->internal);
    }
{{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 628 "OM_DS_TREE.cilk"
static void _cilk_insert_tl_slow(CilkWorkerState*const _cilk_ws,struct _cilk_insert_tl_frame*_cilk_frame){Bottom_List*x;Bottom_List*y;{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header)); };switch (_cilk_frame->header.entry) {case 1: goto _cilk_sync1;case 2: goto _cilk_sync2;case 3: goto _cilk_sync3;case 4: goto _cilk_sync4;}x=_cilk_frame->scope0.x;y=_cilk_frame->scope0.y;
{

    Top_List *list= x->parent;
# 637 "OM_DS_TREE.cilk"
    if (x == list->tail)
    {

  y->tag = (x->tag>> 1) + (1<< (INT_BIT_SIZE- 1));


  if (MAX_NUMBER - x->tag == 1)
      y->tag = MAX_NUMBER;

    }
    else
    {

  y->tag = (x->tag>> 1) + (x->next->tag>> 1);


  if ((x->next->tag & x->tag& 0x1) == 0x1) y->tag++;
    }


    if ( (x == list->tail&& (MAX_NUMBER== x->tag)) ||
      (x != list->tail&& (x->next->tag - x->tag<= 1)))
    {

  {_cilk_frame->header.entry=1;_cilk_frame->scope0.x=x;_cilk_frame->scope0.y=y;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };rebalance_tl(_cilk_ws,x);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync1:x=_cilk_frame->scope0.x;y=_cilk_frame->scope0.y;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=2;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync2:x=_cilk_frame->scope0.x;y=_cilk_frame->scope0.y;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}


  {_cilk_frame->header.entry=3;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };insert_tl(_cilk_ws,x,y);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync3:;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=4;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync4:;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
    }
    else
    {
# 673 "OM_DS_TREE.cilk"
  y->prev = x;
  y->next = x->next;
  x->next = y;
  if (y->next != ((void *)0))
      y->next->prev = y;
  else
      list->tail = y;


  y->parent = list;
  list->size += 1;

  y->internal = malloc(sizeof(Internal_Node));

  y->internal->lvl = 0;


  y->internal->num_children = 0;
  y->internal->base = y->tag;
  y->internal->parent = y->internal->left = y->internal->right = ((void *)0);


  y->internal->bl = y;

  create_scaffolding(x->internal, y->internal);
    }
{{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 628 "OM_DS_TREE.cilk"
static void _cilk_insert_tl_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v)
{(void)_cilk_ws;(void)_cilk_procargs_v;insert_tl(_cilk_ws,((struct _cilk_insert_tl_args*)_cilk_procargs_v)->x,((struct _cilk_insert_tl_args*)_cilk_procargs_v)->y);
# 699 "OM_DS_TREE.cilk"
}
# 628 "OM_DS_TREE.cilk"
void mt_insert_tl(CilkContext*const context,Bottom_List*x,Bottom_List*y)
{struct _cilk_insert_tl_args*_cilk_procargs;_cilk_procargs=(struct _cilk_insert_tl_args*)Cilk_malloc_fixed(sizeof(struct _cilk_insert_tl_args));_cilk_procargs->x=x;_cilk_procargs->y=y;Cilk_start(context,_cilk_insert_tl_import,_cilk_procargs,0);Cilk_free(_cilk_procargs);
# 699 "OM_DS_TREE.cilk"
}

struct _cilk_orderBatchOp_frame{CilkStackFrame header;struct{void*dataStruct;void*data;size_t size;void*result;}scope0;struct{int i;OM_DS_BatchRecord*orArray;OM_DS_BatchRecord*or;int*resultArray;OM_Node*x;OM_Node*y;}scope1;};struct _cilk_orderBatchOp_args{void*dataStruct;void*data;size_t size;void*result;};static void _cilk_orderBatchOp_slow(CilkWorkerState*const _cilk_ws,struct _cilk_orderBatchOp_frame*_cilk_frame);static CilkProcInfo _cilk_orderBatchOp_sig[]={{0,sizeof(struct _cilk_orderBatchOp_frame),_cilk_orderBatchOp_slow,0,0}};
# 701 "OM_DS_TREE.cilk"
void orderBatchOp (CilkWorkerState*const _cilk_ws,void*dataStruct,void*data,size_t size,void*result){struct _cilk_orderBatchOp_frame*_cilk_frame;{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_orderBatchOp_frame), _cilk_orderBatchOp_sig); };{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };{
 int i= 0;
 OM_DS_BatchRecord *orArray= (OM_DS_BatchRecord *)data;
 OM_DS_BatchRecord *or;
 int *resultArray= (int *)result;
 OM_Node *x;OM_Node*y;
# 710 "OM_DS_TREE.cilk"
 for (; i< size; i++) {
  x = (&orArray[i])->x;
  y = (&orArray[i])->y;


  if (x->ds == y->ds)
  {
   if (x->tag < y->tag)
    resultArray[i] = 1;
   else
    resultArray[i] = 0;
  }

  else
  {
   if (x->ds->tag < y->ds->tag)
    resultArray[i] = 1;
   else
    resultArray[i] = 0;
  }
 }

{{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 701 "OM_DS_TREE.cilk"
static void _cilk_orderBatchOp_slow(CilkWorkerState*const _cilk_ws,struct _cilk_orderBatchOp_frame*_cilk_frame){void*dataStruct;void*data;size_t size;void*result;{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header)); };switch (_cilk_frame->header.entry) {}dataStruct=_cilk_frame->scope0.dataStruct;data=_cilk_frame->scope0.data;size=_cilk_frame->scope0.size;result=_cilk_frame->scope0.result;{
 int i= 0;
 OM_DS_BatchRecord *orArray= (OM_DS_BatchRecord *)data;
 OM_DS_BatchRecord *or;
 int *resultArray= (int *)result;
 OM_Node *x;OM_Node*y;
# 710 "OM_DS_TREE.cilk"
 for (; i< size; i++) {
  x = (&orArray[i])->x;
  y = (&orArray[i])->y;


  if (x->ds == y->ds)
  {
   if (x->tag < y->tag)
    resultArray[i] = 1;
   else
    resultArray[i] = 0;
  }

  else
  {
   if (x->ds->tag < y->ds->tag)
    resultArray[i] = 1;
   else
    resultArray[i] = 0;
  }
 }

{{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 701 "OM_DS_TREE.cilk"
static void _cilk_orderBatchOp_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v){(void)_cilk_ws;(void)_cilk_procargs_v;orderBatchOp(_cilk_ws,((struct _cilk_orderBatchOp_args*)_cilk_procargs_v)->dataStruct,((struct _cilk_orderBatchOp_args*)_cilk_procargs_v)->data,((struct _cilk_orderBatchOp_args*)_cilk_procargs_v)->size,((struct _cilk_orderBatchOp_args*)_cilk_procargs_v)->result);
# 732 "OM_DS_TREE.cilk"
}
# 701 "OM_DS_TREE.cilk"
void mt_orderBatchOp(CilkContext*const context,void*dataStruct,void*data,size_t size,void*result){struct _cilk_orderBatchOp_args*_cilk_procargs;_cilk_procargs=(struct _cilk_orderBatchOp_args*)Cilk_malloc_fixed(sizeof(struct _cilk_orderBatchOp_args));_cilk_procargs->dataStruct=dataStruct;_cilk_procargs->data=data;_cilk_procargs->size=size;_cilk_procargs->result=result;Cilk_start(context,_cilk_orderBatchOp_import,_cilk_procargs,0);Cilk_free(_cilk_procargs);
# 732 "OM_DS_TREE.cilk"
}
# 742 "OM_DS_TREE.cilk"
struct _cilk_order_frame{CilkStackFrame header;struct{OM_Node*x;OM_Node*y;int*result;}scope0;struct{OM_DS_BatchRecord*or;}scope1;};struct _cilk_order_args{OM_Node*x;OM_Node*y;int*result;};static void _cilk_order_slow(CilkWorkerState*const _cilk_ws,struct _cilk_order_frame*_cilk_frame);static CilkProcInfo _cilk_order_sig[]={{0,sizeof(struct _cilk_order_frame),_cilk_order_slow,0,0}};
# 742 "OM_DS_TREE.cilk"
void order (CilkWorkerState*const _cilk_ws,OM_Node*x,OM_Node*y,int*result){struct _cilk_order_frame*_cilk_frame;{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_order_frame), _cilk_order_sig); };{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
{
 OM_DS_BatchRecord *or= malloc(sizeof(OM_DS_BatchRecord));
 or->x = x;
 or->y = y;

 Cilk_batchify(_cilk_ws, &orderBatchOp, ((void *)0), or, sizeof(OM_DS_BatchRecord), result);

 free(or);
{{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 742 "OM_DS_TREE.cilk"
static void _cilk_order_slow(CilkWorkerState*const _cilk_ws,struct _cilk_order_frame*_cilk_frame){OM_Node*x;OM_Node*y;int*result;{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header)); };switch (_cilk_frame->header.entry) {}x=_cilk_frame->scope0.x;y=_cilk_frame->scope0.y;result=_cilk_frame->scope0.result;
{
 OM_DS_BatchRecord *or= malloc(sizeof(OM_DS_BatchRecord));
 or->x = x;
 or->y = y;

 Cilk_batchify(_cilk_ws, &orderBatchOp, ((void *)0), or, sizeof(OM_DS_BatchRecord), result);

 free(or);
{{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 742 "OM_DS_TREE.cilk"
static void _cilk_order_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v)
{(void)_cilk_ws;(void)_cilk_procargs_v;order(_cilk_ws,((struct _cilk_order_args*)_cilk_procargs_v)->x,((struct _cilk_order_args*)_cilk_procargs_v)->y,((struct _cilk_order_args*)_cilk_procargs_v)->result);
# 751 "OM_DS_TREE.cilk"
}
# 742 "OM_DS_TREE.cilk"
void mt_order(CilkContext*const context,OM_Node*x,OM_Node*y,int*result)
{struct _cilk_order_args*_cilk_procargs;_cilk_procargs=(struct _cilk_order_args*)Cilk_malloc_fixed(sizeof(struct _cilk_order_args));_cilk_procargs->x=x;_cilk_procargs->y=y;_cilk_procargs->result=result;Cilk_start(context,_cilk_order_import,_cilk_procargs,0);Cilk_free(_cilk_procargs);
# 751 "OM_DS_TREE.cilk"
}
# 760 "OM_DS_TREE.cilk"
struct _cilk_split_bl_frame{CilkStackFrame header;struct{Top_List*list;Bottom_List*list_to_split;}scope0;struct{OM_Node*current;OM_Node*transition_node;Bottom_List*to_add;Bottom_List*holder;int node_count;int list_count;int num_lists_needed;unsigned long skip_size;}scope1;};struct _cilk_split_bl_args{Top_List*list;Bottom_List*list_to_split;};static void _cilk_split_bl_slow(CilkWorkerState*const _cilk_ws,struct _cilk_split_bl_frame*_cilk_frame);static CilkProcInfo _cilk_split_bl_sig[]={{0,sizeof(struct _cilk_split_bl_frame),_cilk_split_bl_slow,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
# 760 "OM_DS_TREE.cilk"
void split_bl (CilkWorkerState*const _cilk_ws,Top_List*list,Bottom_List*list_to_split){struct _cilk_split_bl_frame*_cilk_frame;{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_split_bl_frame), _cilk_split_bl_sig); };{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
{
    OM_Node *current= list_to_split->head;OM_Node*transition_node;


    Bottom_List *to_add;Bottom_List*holder;


    int node_count= 1;int list_count=1;int num_lists_needed=(list_to_split->size>>lg_HALF_INT_BIT_SIZE);


    unsigned long skip_size= MAX_NUMBER >> lg_HALF_INT_BIT_SIZE;


    current->tag = 0;
    while (node_count < HALF_INT_BIT_SIZE && current->next != ((void *)0))
    {
  current->next->tag = current->tag + skip_size;
  current = current->next;
  ++node_count;
    }


    list_to_split->size = node_count;
    list_to_split->tail = current;
    transition_node = current->next;
    current->next = ((void *)0);
    list_to_split->reorder_flag = 0;


    holder = list_to_split;


    while (list_count < num_lists_needed)
    {

  node_count = 1;


  to_add = create_bl();


  current = transition_node;
  to_add->head = current;
  current->prev = ((void *)0);
  current->tag = 0;


  current->ds = to_add;


  while ((node_count< HALF_INT_BIT_SIZE) && (current->next!= ((void *)0)))
  {
      current = current->next;
      current->ds = to_add;
      current->tag = current->prev->tag + skip_size;
      ++node_count;
  }


  to_add->tail = current;
  transition_node = current->next;
  current->next = ((void *)0);
  to_add->reorder_flag = 0;
  to_add->size = node_count;
# 831 "OM_DS_TREE.cilk"
  {_cilk_frame->header.entry=1;_cilk_frame->scope1.transition_node=transition_node;_cilk_frame->scope1.to_add=to_add;_cilk_frame->scope1.list_count=list_count;_cilk_frame->scope1.num_lists_needed=num_lists_needed;_cilk_frame->scope1.skip_size=skip_size;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };insert_tl(_cilk_ws,holder,to_add);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };


  holder = to_add;
  to_add = ((void *)0);


  ++list_count;
    }


    if (transition_node != ((void *)0))
    {
  node_count = 1;


  to_add = create_bl();


  current = transition_node;
  to_add->head = current;
  current->prev = ((void *)0);
  current->tag = 0;


  current->ds = to_add;


  while (current->next != ((void *)0))
  {
      current = current->next;
      current->ds = to_add;
      current->tag = current->prev->tag + skip_size;
      ++node_count;
  }


  to_add->tail = current;
  current->next = ((void *)0);
  to_add->reorder_flag = 0;
  to_add->size = node_count;
# 878 "OM_DS_TREE.cilk"
  {_cilk_frame->header.entry=3;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };insert_tl(_cilk_ws,holder,to_add);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
    }
{{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 760 "OM_DS_TREE.cilk"
static void _cilk_split_bl_slow(CilkWorkerState*const _cilk_ws,struct _cilk_split_bl_frame*_cilk_frame){Top_List*list;Bottom_List*list_to_split;{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header)); };switch (_cilk_frame->header.entry) {case 1: goto _cilk_sync1;case 2: goto _cilk_sync2;case 3: goto _cilk_sync3;case 4: goto _cilk_sync4;}list=_cilk_frame->scope0.list;list_to_split=_cilk_frame->scope0.list_to_split;
{
    OM_Node *current= list_to_split->head;OM_Node*transition_node;


    Bottom_List *to_add;Bottom_List*holder;


    int node_count= 1;int list_count=1;int num_lists_needed=(list_to_split->size>>lg_HALF_INT_BIT_SIZE);


    unsigned long skip_size= MAX_NUMBER >> lg_HALF_INT_BIT_SIZE;


    current->tag = 0;
    while (node_count < HALF_INT_BIT_SIZE && current->next != ((void *)0))
    {
  current->next->tag = current->tag + skip_size;
  current = current->next;
  ++node_count;
    }


    list_to_split->size = node_count;
    list_to_split->tail = current;
    transition_node = current->next;
    current->next = ((void *)0);
    list_to_split->reorder_flag = 0;


    holder = list_to_split;


    while (list_count < num_lists_needed)
    {

  node_count = 1;


  to_add = create_bl();


  current = transition_node;
  to_add->head = current;
  current->prev = ((void *)0);
  current->tag = 0;


  current->ds = to_add;


  while ((node_count< HALF_INT_BIT_SIZE) && (current->next!= ((void *)0)))
  {
      current = current->next;
      current->ds = to_add;
      current->tag = current->prev->tag + skip_size;
      ++node_count;
  }


  to_add->tail = current;
  transition_node = current->next;
  current->next = ((void *)0);
  to_add->reorder_flag = 0;
  to_add->size = node_count;
# 831 "OM_DS_TREE.cilk"
  {_cilk_frame->header.entry=1;_cilk_frame->scope1.transition_node=transition_node;_cilk_frame->scope1.to_add=to_add;_cilk_frame->scope1.list_count=list_count;_cilk_frame->scope1.num_lists_needed=num_lists_needed;_cilk_frame->scope1.skip_size=skip_size;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };insert_tl(_cilk_ws,holder,to_add);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync1:transition_node=_cilk_frame->scope1.transition_node;to_add=_cilk_frame->scope1.to_add;list_count=_cilk_frame->scope1.list_count;num_lists_needed=_cilk_frame->scope1.num_lists_needed;skip_size=_cilk_frame->scope1.skip_size;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=2;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync2:transition_node=_cilk_frame->scope1.transition_node;to_add=_cilk_frame->scope1.to_add;list_count=_cilk_frame->scope1.list_count;num_lists_needed=_cilk_frame->scope1.num_lists_needed;skip_size=_cilk_frame->scope1.skip_size;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}


  holder = to_add;
  to_add = ((void *)0);


  ++list_count;
    }


    if (transition_node != ((void *)0))
    {
  node_count = 1;


  to_add = create_bl();


  current = transition_node;
  to_add->head = current;
  current->prev = ((void *)0);
  current->tag = 0;


  current->ds = to_add;


  while (current->next != ((void *)0))
  {
      current = current->next;
      current->ds = to_add;
      current->tag = current->prev->tag + skip_size;
      ++node_count;
  }


  to_add->tail = current;
  current->next = ((void *)0);
  to_add->reorder_flag = 0;
  to_add->size = node_count;
# 878 "OM_DS_TREE.cilk"
  {_cilk_frame->header.entry=3;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };insert_tl(_cilk_ws,holder,to_add);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync3:;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=4;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync4:;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
    }
{{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 760 "OM_DS_TREE.cilk"
static void _cilk_split_bl_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v)
{(void)_cilk_ws;(void)_cilk_procargs_v;split_bl(_cilk_ws,((struct _cilk_split_bl_args*)_cilk_procargs_v)->list,((struct _cilk_split_bl_args*)_cilk_procargs_v)->list_to_split);
# 881 "OM_DS_TREE.cilk"
}
# 760 "OM_DS_TREE.cilk"
void mt_split_bl(CilkContext*const context,Top_List*list,Bottom_List*list_to_split)
{struct _cilk_split_bl_args*_cilk_procargs;_cilk_procargs=(struct _cilk_split_bl_args*)Cilk_malloc_fixed(sizeof(struct _cilk_split_bl_args));_cilk_procargs->list=list;_cilk_procargs->list_to_split=list_to_split;Cilk_start(context,_cilk_split_bl_import,_cilk_procargs,0);Cilk_free(_cilk_procargs);
# 881 "OM_DS_TREE.cilk"
}

static unsigned rebuild_skip_count= 0;
# 884 "OM_DS_TREE.cilk"
void print_rebuild_count(void)
{
 printf ( "rebuild count: %i\n", rebuild_skip_count);
}
# 897 "OM_DS_TREE.cilk"
struct _cilk_par_build_array_from_rebalance_list_frame{CilkStackFrame header;struct{Internal_Node**buildArray;Internal_Node*current_node;int start;int end;}scope0;struct{int left_side_end;}scope1;};struct _cilk_par_build_array_from_rebalance_list_args{Internal_Node**buildArray;Internal_Node*current_node;int start;int end;};static void _cilk_par_build_array_from_rebalance_list_slow(CilkWorkerState*const _cilk_ws,struct _cilk_par_build_array_from_rebalance_list_frame*_cilk_frame);static CilkProcInfo _cilk_par_build_array_from_rebalance_list_sig[]={{0,sizeof(struct _cilk_par_build_array_from_rebalance_list_frame),_cilk_par_build_array_from_rebalance_list_slow,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
# 897 "OM_DS_TREE.cilk"
void par_build_array_from_rebalance_list (CilkWorkerState*const _cilk_ws,Internal_Node**buildArray,Internal_Node*current_node,int start,int end){struct _cilk_par_build_array_from_rebalance_list_frame*_cilk_frame;{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_par_build_array_from_rebalance_list_frame), _cilk_par_build_array_from_rebalance_list_sig); };{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
{
# 902 "OM_DS_TREE.cilk"
 int left_side_end;


 if (current_node->left->lvl != 0)
 {
  if (current_node->left->num_children < PARALLEL_THRESHOLD)
  {
   c_par_build_array_from_rebalance_list(buildArray, current_node->left, (start), (start + current_node->left->num_children- 1));
# 914 "OM_DS_TREE.cilk"
  }
  else
  {
   {_cilk_frame->header.entry=1;_cilk_frame->scope0.buildArray=buildArray;_cilk_frame->scope0.current_node=current_node;_cilk_frame->scope0.start=start;_cilk_frame->scope0.end=end;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };par_build_array_from_rebalance_list(_cilk_ws,buildArray,current_node->left,(start),(start+current_node->left->num_children-1));{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  }
  left_side_end = start + current_node->left->num_children;
 }
 else
 {
  buildArray[start] = current_node->left;
  left_side_end = start + 1;
 }

 if (current_node->right->lvl != 0)
 {
  if (current_node->right->num_children < PARALLEL_THRESHOLD)
  {
   c_par_build_array_from_rebalance_list(buildArray, current_node->right, left_side_end, end);
# 936 "OM_DS_TREE.cilk"
  }
  else
  {
   {_cilk_frame->header.entry=2;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };par_build_array_from_rebalance_list(_cilk_ws,buildArray,current_node->right,left_side_end,end);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  }
 }
 else
 {
  buildArray[end] = current_node->right;
 }
# 973 "OM_DS_TREE.cilk"
{ Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };{{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 897 "OM_DS_TREE.cilk"
static void _cilk_par_build_array_from_rebalance_list_slow(CilkWorkerState*const _cilk_ws,struct _cilk_par_build_array_from_rebalance_list_frame*_cilk_frame){Internal_Node**buildArray;Internal_Node*current_node;int start;int end;{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header)); };switch (_cilk_frame->header.entry) {case 1: goto _cilk_sync1;case 2: goto _cilk_sync2;case 3: goto _cilk_sync3;}buildArray=_cilk_frame->scope0.buildArray;current_node=_cilk_frame->scope0.current_node;start=_cilk_frame->scope0.start;end=_cilk_frame->scope0.end;
{
# 902 "OM_DS_TREE.cilk"
 int left_side_end;


 if (current_node->left->lvl != 0)
 {
  if (current_node->left->num_children < PARALLEL_THRESHOLD)
  {
   c_par_build_array_from_rebalance_list(buildArray, current_node->left, (start), (start + current_node->left->num_children- 1));
# 914 "OM_DS_TREE.cilk"
  }
  else
  {
   {_cilk_frame->header.entry=1;_cilk_frame->scope0.buildArray=buildArray;_cilk_frame->scope0.current_node=current_node;_cilk_frame->scope0.start=start;_cilk_frame->scope0.end=end;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };par_build_array_from_rebalance_list(_cilk_ws,buildArray,current_node->left,(start),(start+current_node->left->num_children-1));{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync1:buildArray=_cilk_frame->scope0.buildArray;current_node=_cilk_frame->scope0.current_node;start=_cilk_frame->scope0.start;end=_cilk_frame->scope0.end;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  }
  left_side_end = start + current_node->left->num_children;
 }
 else
 {
  buildArray[start] = current_node->left;
  left_side_end = start + 1;
 }

 if (current_node->right->lvl != 0)
 {
  if (current_node->right->num_children < PARALLEL_THRESHOLD)
  {
   c_par_build_array_from_rebalance_list(buildArray, current_node->right, left_side_end, end);
# 936 "OM_DS_TREE.cilk"
  }
  else
  {
   {_cilk_frame->header.entry=2;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };par_build_array_from_rebalance_list(_cilk_ws,buildArray,current_node->right,left_side_end,end);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync2:;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  }
 }
 else
 {
  buildArray[end] = current_node->right;
 }
# 973 "OM_DS_TREE.cilk"
{Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=3;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync3:;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}{{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 897 "OM_DS_TREE.cilk"
static void _cilk_par_build_array_from_rebalance_list_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v)
{(void)_cilk_ws;(void)_cilk_procargs_v;par_build_array_from_rebalance_list(_cilk_ws,((struct _cilk_par_build_array_from_rebalance_list_args*)_cilk_procargs_v)->buildArray,((struct _cilk_par_build_array_from_rebalance_list_args*)_cilk_procargs_v)->current_node,((struct _cilk_par_build_array_from_rebalance_list_args*)_cilk_procargs_v)->start,((struct _cilk_par_build_array_from_rebalance_list_args*)_cilk_procargs_v)->end);
# 973 "OM_DS_TREE.cilk"
}
# 897 "OM_DS_TREE.cilk"
void mt_par_build_array_from_rebalance_list(CilkContext*const context,Internal_Node**buildArray,Internal_Node*current_node,int start,int end)
{struct _cilk_par_build_array_from_rebalance_list_args*_cilk_procargs;_cilk_procargs=(struct _cilk_par_build_array_from_rebalance_list_args*)Cilk_malloc_fixed(sizeof(struct _cilk_par_build_array_from_rebalance_list_args));_cilk_procargs->buildArray=buildArray;_cilk_procargs->current_node=current_node;_cilk_procargs->start=start;_cilk_procargs->end=end;Cilk_start(context,_cilk_par_build_array_from_rebalance_list_import,_cilk_procargs,0);Cilk_free(_cilk_procargs);
# 973 "OM_DS_TREE.cilk"
}

static int rebalance_count= 0;
# 976 "OM_DS_TREE.cilk"
void print_rebalance_count(void)
{
 printf ( "Rebalance count:%i\n", rebalance_count);
}

static double rebalance_total_time= 0;
# 982 "OM_DS_TREE.cilk"
void print_rebalance_timing(void)
{
 printf ( "rebalance total time(s):%f \n", (rebalance_total_time/ 1000000l));
}
# 994 "OM_DS_TREE.cilk"
struct _cilk_rebalance_tl_frame{CilkStackFrame header;struct{Bottom_List*pivot;}scope0;struct{Internal_Node*current_node;Internal_Node**nodeArray;double overflow_density;double overflow_threshold;double i;unsigned current_tag_range;unsigned current_tree_lvl;unsigned lvl_dif;}scope1;};struct _cilk_rebalance_tl_args{Bottom_List*pivot;};static void _cilk_rebalance_tl_slow(CilkWorkerState*const _cilk_ws,struct _cilk_rebalance_tl_frame*_cilk_frame);static CilkProcInfo _cilk_rebalance_tl_sig[]={{0,sizeof(struct _cilk_rebalance_tl_frame),_cilk_rebalance_tl_slow,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
# 994 "OM_DS_TREE.cilk"
void rebalance_tl (CilkWorkerState*const _cilk_ws,Bottom_List*pivot){struct _cilk_rebalance_tl_frame*_cilk_frame;{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_rebalance_tl_frame), _cilk_rebalance_tl_sig); };{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };{
# 1001 "OM_DS_TREE.cilk"
    Internal_Node *current_node= pivot->internal;
 Internal_Node **nodeArray;


    double overflow_density;double overflow_threshold;double i=-1;
    unsigned current_tag_range= 1;unsigned current_tree_lvl=0;unsigned lvl_dif=0;

 rebalance_count++;
# 1016 "OM_DS_TREE.cilk"
    do
    {
# 1026 "OM_DS_TREE.cilk"
  if (current_node->lvl == INT_BIT_SIZE)
  {

      break;
  }


  current_node = current_node->parent;


  lvl_dif = current_node->lvl - current_tree_lvl;


  current_tree_lvl = current_node->lvl;


  current_tag_range = current_tag_range << lvl_dif;


  overflow_threshold = pow(OVERFLOW_CONSTANT, -1.0 * (current_tree_lvl));


  overflow_density = ((double)current_node->num_children+ 1) / ((double)current_tag_range);

    }
    while ( (overflow_density> overflow_threshold) && (current_node->lvl<= INT_BIT_SIZE));

 nodeArray = malloc(sizeof(Internal_Node *) * current_node->num_children);

 {_cilk_frame->header.entry=1;_cilk_frame->scope1.current_node=current_node;_cilk_frame->scope1.nodeArray=nodeArray;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };par_build_array_from_rebalance_list(_cilk_ws,nodeArray,current_node,0,current_node->num_children-1);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
 { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
# 1066 "OM_DS_TREE.cilk"
 i = rebuild_tree_count;
 {_cilk_frame->header.entry=3;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };rebuild_tree(_cilk_ws,current_node,nodeArray,0,current_node->num_children-1);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
 { Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
# 1076 "OM_DS_TREE.cilk"
 free(nodeArray);
# 1082 "OM_DS_TREE.cilk"
{{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 994 "OM_DS_TREE.cilk"
static void _cilk_rebalance_tl_slow(CilkWorkerState*const _cilk_ws,struct _cilk_rebalance_tl_frame*_cilk_frame){Bottom_List*pivot;{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header)); };switch (_cilk_frame->header.entry) {case 1: goto _cilk_sync1;case 2: goto _cilk_sync2;case 3: goto _cilk_sync3;case 4: goto _cilk_sync4;}pivot=_cilk_frame->scope0.pivot;{
# 1001 "OM_DS_TREE.cilk"
    Internal_Node *current_node= pivot->internal;
 Internal_Node **nodeArray;


    double overflow_density;double overflow_threshold;double i=-1;
    unsigned current_tag_range= 1;unsigned current_tree_lvl=0;unsigned lvl_dif=0;

 rebalance_count++;
# 1016 "OM_DS_TREE.cilk"
    do
    {
# 1026 "OM_DS_TREE.cilk"
  if (current_node->lvl == INT_BIT_SIZE)
  {

      break;
  }


  current_node = current_node->parent;


  lvl_dif = current_node->lvl - current_tree_lvl;


  current_tree_lvl = current_node->lvl;


  current_tag_range = current_tag_range << lvl_dif;


  overflow_threshold = pow(OVERFLOW_CONSTANT, -1.0 * (current_tree_lvl));


  overflow_density = ((double)current_node->num_children+ 1) / ((double)current_tag_range);

    }
    while ( (overflow_density> overflow_threshold) && (current_node->lvl<= INT_BIT_SIZE));

 nodeArray = malloc(sizeof(Internal_Node *) * current_node->num_children);

 {_cilk_frame->header.entry=1;_cilk_frame->scope1.current_node=current_node;_cilk_frame->scope1.nodeArray=nodeArray;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };par_build_array_from_rebalance_list(_cilk_ws,nodeArray,current_node,0,current_node->num_children-1);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync1:current_node=_cilk_frame->scope1.current_node;nodeArray=_cilk_frame->scope1.nodeArray;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
 {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=2;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync2:current_node=_cilk_frame->scope1.current_node;nodeArray=_cilk_frame->scope1.nodeArray;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
# 1066 "OM_DS_TREE.cilk"
 i = rebuild_tree_count;
 {_cilk_frame->header.entry=3;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };rebuild_tree(_cilk_ws,current_node,nodeArray,0,current_node->num_children-1);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync3:nodeArray=_cilk_frame->scope1.nodeArray;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
 {Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=4;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync4:nodeArray=_cilk_frame->scope1.nodeArray;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
# 1076 "OM_DS_TREE.cilk"
 free(nodeArray);
# 1082 "OM_DS_TREE.cilk"
{{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 994 "OM_DS_TREE.cilk"
static void _cilk_rebalance_tl_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v){(void)_cilk_ws;(void)_cilk_procargs_v;rebalance_tl(_cilk_ws,((struct _cilk_rebalance_tl_args*)_cilk_procargs_v)->pivot);
# 1082 "OM_DS_TREE.cilk"
}
# 994 "OM_DS_TREE.cilk"
void mt_rebalance_tl(CilkContext*const context,Bottom_List*pivot){struct _cilk_rebalance_tl_args*_cilk_procargs;_cilk_procargs=(struct _cilk_rebalance_tl_args*)Cilk_malloc_fixed(sizeof(struct _cilk_rebalance_tl_args));_cilk_procargs->pivot=pivot;Cilk_start(context,_cilk_rebalance_tl_import,_cilk_procargs,0);Cilk_free(_cilk_procargs);
# 1082 "OM_DS_TREE.cilk"
}

static unsigned remove_scaffolding_count= 0;
# 1085 "OM_DS_TREE.cilk"
void print_remove_count(void)
{
 printf ( "Remove scaffolding: %i\n", remove_scaffolding_count);
}
# 1090 "OM_DS_TREE.cilk"
void remove_scaffolding(Internal_Node *node)
{
 remove_scaffolding_count++;

 if (node->left && node->left->lvl > 0)
 {
  remove_scaffolding(node->left);
 }
 if (node->right && node->right->lvl > 0)
 {
  remove_scaffolding(node->right);
 }
 free(node);
}
# 1114 "OM_DS_TREE.cilk"
struct _cilk_rebuild_tree_frame{CilkStackFrame header;struct{Internal_Node*current_node;Internal_Node**nodeArray;int startIndex;int endIndex;}scope0;struct{Internal_Node*new_child;int num_children;}scope1;struct{int rebuild_left_flag;int rebuild_right_flag;int leftStart;int leftEnd;int rightStart;int rightEnd;int num_children_left;int num_children_right;}scope2;};struct _cilk_rebuild_tree_args{Internal_Node*current_node;Internal_Node**nodeArray;int startIndex;int endIndex;};static void _cilk_rebuild_tree_slow(CilkWorkerState*const _cilk_ws,struct _cilk_rebuild_tree_frame*_cilk_frame);static CilkProcInfo _cilk_rebuild_tree_sig[]={{0,sizeof(struct _cilk_rebuild_tree_frame),_cilk_rebuild_tree_slow,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
# 1114 "OM_DS_TREE.cilk"
void rebuild_tree (CilkWorkerState*const _cilk_ws,Internal_Node*current_node,Internal_Node**nodeArray,int startIndex,int endIndex){struct _cilk_rebuild_tree_frame*_cilk_frame;{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_rebuild_tree_frame), _cilk_rebuild_tree_sig); };{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
{
    Internal_Node *new_child;
    int num_children= (endIndex- startIndex) + 1;
    rebuild_tree_count++;


    if (num_children == 1)
    {
# 1128 "OM_DS_TREE.cilk"
  if (current_node->left && current_node->left->lvl > 0)
  {
   remove_scaffolding(current_node->left);
  }
  if (current_node->right && current_node->right->lvl > 0)
  {
   remove_scaffolding(current_node->right);
  }

  current_node->left = nodeArray[startIndex];


  current_node->left->parent = current_node;


  current_node->left->base = current_node->left->bl->tag = current_node->base;


  current_node->num_children = 1;


  current_node->right = ((void *)0);
    }
    else if (num_children == 2)
    {
# 1160 "OM_DS_TREE.cilk"
  if (current_node->left && current_node->left->lvl > 0)
  {
   remove_scaffolding(current_node->left);
  }
  if (current_node->right && current_node->right->lvl > 0)
  {
   remove_scaffolding(current_node->right);
  }

  current_node->left = nodeArray[startIndex];
  current_node->right = nodeArray[endIndex];


  current_node->left->parent = current_node;
  current_node->right->parent = current_node;


  current_node->left->base = current_node->left->bl->tag = current_node->base;


  if (current_node->lvl == INT_BIT_SIZE)
   current_node->right->base = current_node->right->bl->tag = (1<< (INT_BIT_SIZE- 1));
  else
   current_node->right->base = current_node->right->bl->tag = current_node->base | (1<< (current_node->lvl- 1));


  current_node->num_children = 2;
    }
    else
    {

     int rebuild_left_flag= 1;int rebuild_right_flag=1;


  int leftStart= startIndex;
  int leftEnd= startIndex + ((num_children- 1)>> 1);
  int rightStart= leftEnd + 1;
  int rightEnd= endIndex;

  int num_children_left= (leftEnd- leftStart) + 1;int num_children_right=(rightEnd- rightStart) + 1;


  current_node->num_children = num_children;
# 1209 "OM_DS_TREE.cilk"
  if (num_children_left == 1) {

   if (current_node->right && current_node->right->lvl > 0)
    remove_scaffolding(current_node->right);


   current_node->left = nodeArray[leftStart];


   current_node->left->parent = current_node;


   current_node->left->base = current_node->left->bl->tag = current_node->base;

   rebuild_left_flag = 0;
  }
  else if (current_node->left == ((void *)0))
  {
# 1232 "OM_DS_TREE.cilk"
   current_node->left = malloc(sizeof(Internal_Node));


   current_node->left->lvl = current_node->lvl -1;
   current_node->left->bl = ((void *)0);


   current_node->left->base = current_node->base;


   current_node->left->parent = current_node;

   current_node->left->left = current_node->left->right = ((void *)0);
  }

  else if ((current_node->lvl- current_node->left->lvl) != 1)
  {
      if (current_node->left->lvl == 0)
      {
    if (num_children_left== 1)
    {
        current_node->left = nodeArray[leftStart];
        current_node->left->parent = current_node;


        current_node->left->base = current_node->left->bl->tag = current_node->base;

     rebuild_left_flag = 0;
    }
    else
    {

        new_child = malloc(sizeof(Internal_Node));


        current_node->left = new_child;
        new_child->parent = current_node;
# 1273 "OM_DS_TREE.cilk"
        new_child->left = new_child->right = ((void *)0);
        new_child->bl = ((void *)0);


        new_child->lvl = current_node->lvl - 1;


        current_node->left->num_children = num_children_left;


        new_child->base = current_node->base;
    }
      }
      else
      {


    current_node->left->lvl = current_node->lvl - 1;
    current_node->left->base = current_node->base;
      }
  }
# 1297 "OM_DS_TREE.cilk"
  if (rebuild_left_flag)
  {
   if (num_children_left < PARALLEL_THRESHOLD)
   {
    c_rebuild_tree(current_node->left, nodeArray, leftStart, leftEnd);
# 1306 "OM_DS_TREE.cilk"
   }
   else
    {_cilk_frame->header.entry=1;_cilk_frame->scope0.current_node=current_node;_cilk_frame->scope0.nodeArray=nodeArray;_cilk_frame->scope2.rebuild_right_flag=rebuild_right_flag;_cilk_frame->scope2.rightStart=rightStart;_cilk_frame->scope2.rightEnd=rightEnd;_cilk_frame->scope2.num_children_right=num_children_right;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };rebuild_tree(_cilk_ws,current_node->left,nodeArray,leftStart,leftEnd);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  }
# 1315 "OM_DS_TREE.cilk"
  if (num_children_right == 1)
  {
   if (current_node->right && current_node->right->lvl > 0)
    remove_scaffolding(current_node->right);


   current_node->right = nodeArray[rightStart];


   current_node->right->parent = current_node;


   if (current_node->lvl == INT_BIT_SIZE) {
    current_node->right->base = current_node->right->bl->tag = (1<< (INT_BIT_SIZE- 1));
   }
   else
    current_node->right->base = current_node->right->bl->tag = current_node->base | (1<< (current_node->lvl-1));

   rebuild_right_flag = 0;
  }
  else if (current_node->right == ((void *)0))
  {
# 1342 "OM_DS_TREE.cilk"
   current_node->right = malloc(sizeof(Internal_Node));


   current_node->right->lvl = current_node->lvl -1;


   current_node->right->base = current_node->base | (1<< current_node->right->lvl);


   current_node->right->parent = current_node;

   current_node->right->right = current_node->right->left = ((void *)0);
   current_node->right->bl = ((void *)0);
  }

  else if ((current_node->lvl- current_node->right->lvl) != 1)
  {
      if (current_node->right->lvl == 0)
      {
    if (num_children_right== 1)
    {
        current_node->right = nodeArray[rightStart];
        current_node->right->parent = current_node;


     current_node->right->base = current_node->right->bl->tag = current_node->base | (1<< (current_node->lvl- 1));

     rebuild_right_flag = 0;
    }
    else
    {

        new_child = malloc(sizeof(Internal_Node));


        current_node->right = new_child;
        new_child->parent = current_node;


        new_child->left = new_child->right = ((void *)0);
        new_child->bl = ((void *)0);


        new_child->lvl = current_node->lvl - 1;


        new_child->num_children = num_children_right;


        new_child->base = current_node->base | (1<< new_child->lvl);
    }
      }
      else
      {


    current_node->right->lvl = current_node->lvl - 1;
    current_node->right->base = current_node->base | (1<< current_node->right->lvl);
      }
  }
# 1405 "OM_DS_TREE.cilk"
  if (rebuild_right_flag)
  {
   if (num_children_right < PARALLEL_THRESHOLD)
   {
    c_rebuild_tree(current_node->right, nodeArray, rightStart, rightEnd);
# 1414 "OM_DS_TREE.cilk"
   }
   else
    {_cilk_frame->header.entry=2;Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };rebuild_tree(_cilk_ws,current_node->right,nodeArray,rightStart,rightEnd);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };{ Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  }
# 1422 "OM_DS_TREE.cilk"
    }
{ Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };{{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 1114 "OM_DS_TREE.cilk"
static void _cilk_rebuild_tree_slow(CilkWorkerState*const _cilk_ws,struct _cilk_rebuild_tree_frame*_cilk_frame){Internal_Node*current_node;Internal_Node**nodeArray;int startIndex;int endIndex;{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header)); };switch (_cilk_frame->header.entry) {case 1: goto _cilk_sync1;case 2: goto _cilk_sync2;case 3: goto _cilk_sync3;}current_node=_cilk_frame->scope0.current_node;nodeArray=_cilk_frame->scope0.nodeArray;startIndex=_cilk_frame->scope0.startIndex;endIndex=_cilk_frame->scope0.endIndex;
{
    Internal_Node *new_child;
    int num_children= (endIndex- startIndex) + 1;
    rebuild_tree_count++;


    if (num_children == 1)
    {
# 1128 "OM_DS_TREE.cilk"
  if (current_node->left && current_node->left->lvl > 0)
  {
   remove_scaffolding(current_node->left);
  }
  if (current_node->right && current_node->right->lvl > 0)
  {
   remove_scaffolding(current_node->right);
  }

  current_node->left = nodeArray[startIndex];


  current_node->left->parent = current_node;


  current_node->left->base = current_node->left->bl->tag = current_node->base;


  current_node->num_children = 1;


  current_node->right = ((void *)0);
    }
    else if (num_children == 2)
    {
# 1160 "OM_DS_TREE.cilk"
  if (current_node->left && current_node->left->lvl > 0)
  {
   remove_scaffolding(current_node->left);
  }
  if (current_node->right && current_node->right->lvl > 0)
  {
   remove_scaffolding(current_node->right);
  }

  current_node->left = nodeArray[startIndex];
  current_node->right = nodeArray[endIndex];


  current_node->left->parent = current_node;
  current_node->right->parent = current_node;


  current_node->left->base = current_node->left->bl->tag = current_node->base;


  if (current_node->lvl == INT_BIT_SIZE)
   current_node->right->base = current_node->right->bl->tag = (1<< (INT_BIT_SIZE- 1));
  else
   current_node->right->base = current_node->right->bl->tag = current_node->base | (1<< (current_node->lvl- 1));


  current_node->num_children = 2;
    }
    else
    {

     int rebuild_left_flag= 1;int rebuild_right_flag=1;


  int leftStart= startIndex;
  int leftEnd= startIndex + ((num_children- 1)>> 1);
  int rightStart= leftEnd + 1;
  int rightEnd= endIndex;

  int num_children_left= (leftEnd- leftStart) + 1;int num_children_right=(rightEnd- rightStart) + 1;


  current_node->num_children = num_children;
# 1209 "OM_DS_TREE.cilk"
  if (num_children_left == 1) {

   if (current_node->right && current_node->right->lvl > 0)
    remove_scaffolding(current_node->right);


   current_node->left = nodeArray[leftStart];


   current_node->left->parent = current_node;


   current_node->left->base = current_node->left->bl->tag = current_node->base;

   rebuild_left_flag = 0;
  }
  else if (current_node->left == ((void *)0))
  {
# 1232 "OM_DS_TREE.cilk"
   current_node->left = malloc(sizeof(Internal_Node));


   current_node->left->lvl = current_node->lvl -1;
   current_node->left->bl = ((void *)0);


   current_node->left->base = current_node->base;


   current_node->left->parent = current_node;

   current_node->left->left = current_node->left->right = ((void *)0);
  }

  else if ((current_node->lvl- current_node->left->lvl) != 1)
  {
      if (current_node->left->lvl == 0)
      {
    if (num_children_left== 1)
    {
        current_node->left = nodeArray[leftStart];
        current_node->left->parent = current_node;


        current_node->left->base = current_node->left->bl->tag = current_node->base;

     rebuild_left_flag = 0;
    }
    else
    {

        new_child = malloc(sizeof(Internal_Node));


        current_node->left = new_child;
        new_child->parent = current_node;
# 1273 "OM_DS_TREE.cilk"
        new_child->left = new_child->right = ((void *)0);
        new_child->bl = ((void *)0);


        new_child->lvl = current_node->lvl - 1;


        current_node->left->num_children = num_children_left;


        new_child->base = current_node->base;
    }
      }
      else
      {


    current_node->left->lvl = current_node->lvl - 1;
    current_node->left->base = current_node->base;
      }
  }
# 1297 "OM_DS_TREE.cilk"
  if (rebuild_left_flag)
  {
   if (num_children_left < PARALLEL_THRESHOLD)
   {
    c_rebuild_tree(current_node->left, nodeArray, leftStart, leftEnd);
# 1306 "OM_DS_TREE.cilk"
   }
   else
    {_cilk_frame->header.entry=1;_cilk_frame->scope0.current_node=current_node;_cilk_frame->scope0.nodeArray=nodeArray;_cilk_frame->scope2.rebuild_right_flag=rebuild_right_flag;_cilk_frame->scope2.rightStart=rightStart;_cilk_frame->scope2.rightEnd=rightEnd;_cilk_frame->scope2.num_children_right=num_children_right;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };rebuild_tree(_cilk_ws,current_node->left,nodeArray,leftStart,leftEnd);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync1:current_node=_cilk_frame->scope0.current_node;nodeArray=_cilk_frame->scope0.nodeArray;rebuild_right_flag=_cilk_frame->scope2.rebuild_right_flag;rightStart=_cilk_frame->scope2.rightStart;rightEnd=_cilk_frame->scope2.rightEnd;num_children_right=_cilk_frame->scope2.num_children_right;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  }
# 1315 "OM_DS_TREE.cilk"
  if (num_children_right == 1)
  {
   if (current_node->right && current_node->right->lvl > 0)
    remove_scaffolding(current_node->right);


   current_node->right = nodeArray[rightStart];


   current_node->right->parent = current_node;


   if (current_node->lvl == INT_BIT_SIZE) {
    current_node->right->base = current_node->right->bl->tag = (1<< (INT_BIT_SIZE- 1));
   }
   else
    current_node->right->base = current_node->right->bl->tag = current_node->base | (1<< (current_node->lvl-1));

   rebuild_right_flag = 0;
  }
  else if (current_node->right == ((void *)0))
  {
# 1342 "OM_DS_TREE.cilk"
   current_node->right = malloc(sizeof(Internal_Node));


   current_node->right->lvl = current_node->lvl -1;


   current_node->right->base = current_node->base | (1<< current_node->right->lvl);


   current_node->right->parent = current_node;

   current_node->right->right = current_node->right->left = ((void *)0);
   current_node->right->bl = ((void *)0);
  }

  else if ((current_node->lvl- current_node->right->lvl) != 1)
  {
      if (current_node->right->lvl == 0)
      {
    if (num_children_right== 1)
    {
        current_node->right = nodeArray[rightStart];
        current_node->right->parent = current_node;


     current_node->right->base = current_node->right->bl->tag = current_node->base | (1<< (current_node->lvl- 1));

     rebuild_right_flag = 0;
    }
    else
    {

        new_child = malloc(sizeof(Internal_Node));


        current_node->right = new_child;
        new_child->parent = current_node;


        new_child->left = new_child->right = ((void *)0);
        new_child->bl = ((void *)0);


        new_child->lvl = current_node->lvl - 1;


        new_child->num_children = num_children_right;


        new_child->base = current_node->base | (1<< new_child->lvl);
    }
      }
      else
      {


    current_node->right->lvl = current_node->lvl - 1;
    current_node->right->base = current_node->base | (1<< current_node->right->lvl);
      }
  }
# 1405 "OM_DS_TREE.cilk"
  if (rebuild_right_flag)
  {
   if (num_children_right < PARALLEL_THRESHOLD)
   {
    c_rebuild_tree(current_node->right, nodeArray, rightStart, rightEnd);
# 1414 "OM_DS_TREE.cilk"
   }
   else
    {_cilk_frame->header.entry=2;Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header)); };rebuild_tree(_cilk_ws,current_node->right,nodeArray,rightStart,rightEnd);{ if (Cilk_cilk2c_pop_check(_cilk_ws)) { if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) { Cilk_cilk2c_pop(_cilk_ws); return ; } } };Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));if (0) {_cilk_sync2:;}{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}
  }
# 1422 "OM_DS_TREE.cilk"
    }
{Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));_cilk_frame->header.entry=3;if (Cilk_sync(_cilk_ws)) {return;_cilk_sync3:;}Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));{ Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };}{{ Cilk_set_result(_cilk_ws, (void *)0, 0); };{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}}}
# 1114 "OM_DS_TREE.cilk"
static void _cilk_rebuild_tree_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v)
{(void)_cilk_ws;(void)_cilk_procargs_v;rebuild_tree(_cilk_ws,((struct _cilk_rebuild_tree_args*)_cilk_procargs_v)->current_node,((struct _cilk_rebuild_tree_args*)_cilk_procargs_v)->nodeArray,((struct _cilk_rebuild_tree_args*)_cilk_procargs_v)->startIndex,((struct _cilk_rebuild_tree_args*)_cilk_procargs_v)->endIndex);
# 1423 "OM_DS_TREE.cilk"
}
# 1114 "OM_DS_TREE.cilk"
void mt_rebuild_tree(CilkContext*const context,Internal_Node*current_node,Internal_Node**nodeArray,int startIndex,int endIndex)
{struct _cilk_rebuild_tree_args*_cilk_procargs;_cilk_procargs=(struct _cilk_rebuild_tree_args*)Cilk_malloc_fixed(sizeof(struct _cilk_rebuild_tree_args));_cilk_procargs->current_node=current_node;_cilk_procargs->nodeArray=nodeArray;_cilk_procargs->startIndex=startIndex;_cilk_procargs->endIndex=endIndex;Cilk_start(context,_cilk_rebuild_tree_import,_cilk_procargs,0);Cilk_free(_cilk_procargs);
# 1423 "OM_DS_TREE.cilk"
}
# 1431 "OM_DS_TREE.cilk"
void print_tl (Top_List *list)
{
    Bottom_List *current= list->head;

    printf ( "(size = %i) Head->", list->size);
    while (current != ((void *)0))
    {
  printf ( "(%u)->", current->tag);
  current = current->next;
    }
    printf ( "Tail\n");
}
# 1450 "OM_DS_TREE.cilk"
void print_bl (Bottom_List *list)
{
    OM_Node *current= list->head;

    printf ( "(size = %i) Head->", list->size);

    printf ( "(size = %i) Head->", list->size);
    while (current != ((void *)0))
    {
  printf ( "(%i | %u)->", current->ID, current->tag);
  current = current->next;
    }
    printf ( "Tail\n");
}
# 1472 "OM_DS_TREE.cilk"
void free_bl (Bottom_List *list)
{
    OM_Node *next;OM_Node*current=list->head;


    while (current != ((void *)0)) {
  next = current->next;
  free(current);
  current = ((void *)0);
  current = next;
    }


    free(list);
    list = ((void *)0);
}
# 1495 "OM_DS_TREE.cilk"
void free_tl_helper (Internal_Node *node)
{
 if (node->left)
 {
  if (node->left->lvl > 0)
   free_tl_helper(node->left);
  else
  {
   free_bl(node->left->bl);
   free(node->left);
   node->left = ((void *)0);
  }
 }
 if (node->right)
 {
  if (node->right->lvl > 0)
   free_tl_helper(node->right);
  else
  {
   free_bl(node->right->bl);
   free(node->right);
   node->right = ((void *)0);
  }
 }
 free(node);
 node = ((void *)0);
}
# 1529 "OM_DS_TREE.cilk"
void free_tl (Top_List *list)
{
 Internal_Node *root= list->head->internal;


 while (root->parent != ((void *)0))
  root = root->parent;

 free_tl_helper(root);
 free(list);
 list = ((void *)0);
}
# 1548 "OM_DS_TREE.cilk"
void check_sub_correctness (Top_List *list)
{
    Bottom_List *current= list->head;
    OM_Node *cur_node;


    while (current != ((void *)0))
    {

  cur_node = current->head;
  while (cur_node!= current->tail)
  {
      if ( !(cur_node->tag< cur_node->next->tag))
    printf("Node tags are out of order; cur:[%p|%i] cur->next:[%p|%i]\n", cur_node, cur_node->tag, cur_node->next, cur_node->next->tag);
      cur_node = cur_node->next;
  }
  if (current->next && current->tag >=current->next->tag)
      printf ( "Bottom list tags are out of place; cur:[B:%p|I:%p|%i] cur->next:[B:%p|I:%p|%i]\n", current, current->internal, current->tag, current->next, current->next->internal, current->next->tag);
  current = current->next;
    }
}


void LNR (Internal_Node *current, int level)
{
    int temp= 9-level;
    if (current != ((void *)0))
    {
  LNR(current->left, level-1);
  while (temp > 0)
  {
      printf("\t");
      --temp;
  }
  printf("Level: %i [%i, mem: %p, L: %p, R: %p]\n", level, current->base, current, current->left, current->right);
  LNR(current->right, level-1);
    }
}
typedef struct print_node_s{
    Internal_Node *data;
    struct print_node_s *next;
}
# 1586 "OM_DS_TREE.cilk"
print_node;




int print_nodes_in_order(print_node *first, print_node *second){
    if (first->data->lvl < second->data->lvl)
  return 0;
    else if (first->data->lvl == second->data->lvl) {
     return first->data->base < second->data->base;
    }
    else
    {
  return 1;
    }
}
# 1602 "OM_DS_TREE.cilk"
void append_and_sort(print_node *current, print_node *to_add)
{
    print_node *trailing= current;

    while ((current!= ((void *)0)) && (print_nodes_in_order(current, to_add))) {
  trailing = current;
  current = current->next;
    }

    to_add->next = trailing->next;
    trailing->next = to_add;

}
# 1615 "OM_DS_TREE.cilk"
print_node *pop_print_node (print_node **head) {
    print_node *tmp= *head;
    if (!tmp)
  printf ( "No node to pop!\n");

    *head = (*head)->next;
    free(tmp);

    return *head;
}
# 1625 "OM_DS_TREE.cilk"
void print_tree (Top_List *list)
{
    print_node *head;print_node*current=malloc(sizeof(print_node));
    Internal_Node *iter;Internal_Node*trailing;

    int current_lvl;

    printf ( "Tree size: %i\n", list->size);
    iter = list->head->internal;
    while (iter!= ((void *)0)) {
  trailing = iter;
  iter = iter->parent;
    }
    current->data = trailing;
    current->next = ((void *)0);
    current_lvl = trailing->lvl;
    head = current;
# 1647 "OM_DS_TREE.cilk"
    printf ( "\n\nLevel: %i ", current_lvl);
    while (current)
    {
  if (current->data->left)
  {
      print_node *left= malloc(sizeof(Internal_Node));
      left->data = current->data->left;
      append_and_sort(current, left);
  }
  if (current->data->right)
  {
      print_node *right= malloc(sizeof(Internal_Node));
      right->data = current->data->right;
      append_and_sort(current, right);
  }

  printf(" --- (loc: %p|p: %p|lvl:%i|L:%p R:%p|base:%u) ---", current->data, current->data->parent, current->data->lvl, current->data->left, current->data->right, current->data->base);
  current = pop_print_node(&current);

  if (current == ((void *)0))
      break;
  if (current->data->lvl != current_lvl)
  {
      current_lvl = current->data->lvl;
      printf ( "\nLevel: %i", current_lvl);
  }

    }
# 1682 "OM_DS_TREE.cilk"
    printf ( "\n\nEND TREE\n");

}
# 1694 "OM_DS_TREE.cilk"
void c_rebuild_tree (Internal_Node *current_node, Internal_Node **nodeArray, int startIndex, int endIndex)
{
# 1701 "OM_DS_TREE.cilk"
    Internal_Node *new_child;
    int num_children= (endIndex- startIndex) + 1;
    rebuild_tree_count++;


    if (num_children == 1)
    {
# 1713 "OM_DS_TREE.cilk"
  if (current_node->left && current_node->left->lvl > 0)
  {
   remove_scaffolding(current_node->left);
  }
  if (current_node->right && current_node->right->lvl > 0)
  {
   remove_scaffolding(current_node->right);
  }

  current_node->left = nodeArray[startIndex];


  current_node->left->parent = current_node;


  current_node->left->base = current_node->left->bl->tag = current_node->base;


  current_node->num_children = 1;


  current_node->right = ((void *)0);
    }
    else if (num_children == 2)
    {
# 1745 "OM_DS_TREE.cilk"
  if (current_node->left && current_node->left->lvl > 0)
  {
   remove_scaffolding(current_node->left);
  }
  if (current_node->right && current_node->right->lvl > 0)
  {
   remove_scaffolding(current_node->right);
  }

  current_node->left = nodeArray[startIndex];
  current_node->right = nodeArray[endIndex];


  current_node->left->parent = current_node;
  current_node->right->parent = current_node;


  current_node->left->base = current_node->left->bl->tag = current_node->base;


  if (current_node->lvl == INT_BIT_SIZE)
   current_node->right->base = current_node->right->bl->tag = (1<< (INT_BIT_SIZE- 1));
  else
   current_node->right->base = current_node->right->bl->tag = current_node->base | (1<< (current_node->lvl- 1));


  current_node->num_children = 2;
    }
    else
    {

     int rebuild_left_flag= 1;int rebuild_right_flag=1;


  int leftStart= startIndex;
  int leftEnd= startIndex + ((num_children- 1)>> 1);
  int rightStart= leftEnd + 1;
  int rightEnd= endIndex;

  int num_children_left= (leftEnd- leftStart) + 1;int num_children_right=(rightEnd- rightStart) + 1;


  current_node->num_children = num_children;
# 1795 "OM_DS_TREE.cilk"
  if (num_children_left == 1) {

   if (current_node->right && current_node->right->lvl > 0)
    remove_scaffolding(current_node->right);


   current_node->left = nodeArray[leftStart];


   current_node->left->parent = current_node;


   current_node->left->base = current_node->left->bl->tag = current_node->base;

   rebuild_left_flag = 0;
  }
  else if (current_node->left == ((void *)0))
  {
# 1818 "OM_DS_TREE.cilk"
   current_node->left = malloc(sizeof(Internal_Node));


   current_node->left->lvl = current_node->lvl -1;
   current_node->left->bl = ((void *)0);


   current_node->left->base = current_node->base;


   current_node->left->parent = current_node;

   current_node->left->left = current_node->left->right = ((void *)0);
  }

  else if ((current_node->lvl- current_node->left->lvl) != 1)
  {
      if (current_node->left->lvl == 0)
      {
    if (num_children_left== 1)
    {
        current_node->left = nodeArray[leftStart];
        current_node->left->parent = current_node;


        current_node->left->base = current_node->left->bl->tag = current_node->base;

     rebuild_left_flag = 0;
    }
    else
    {

        new_child = malloc(sizeof(Internal_Node));


        current_node->left = new_child;
        new_child->parent = current_node;
# 1859 "OM_DS_TREE.cilk"
        new_child->left = new_child->right = ((void *)0);
        new_child->bl = ((void *)0);


        new_child->lvl = current_node->lvl - 1;


        current_node->left->num_children = num_children_left;


        new_child->base = current_node->base;
    }
      }
      else
      {


    current_node->left->lvl = current_node->lvl - 1;
    current_node->left->base = current_node->base;
      }
  }


  if (rebuild_left_flag)
    c_rebuild_tree(current_node->left, nodeArray, leftStart, leftEnd);


  if (num_children_right == 1) {

   if (current_node->right && current_node->right->lvl > 0)
    remove_scaffolding(current_node->right);


   current_node->right = nodeArray[rightStart];


   current_node->right->parent = current_node;


   if (current_node->lvl == INT_BIT_SIZE) {
    current_node->right->base = current_node->right->bl->tag = (1<< (INT_BIT_SIZE- 1));
   }
   else
    current_node->right->base = current_node->right->bl->tag = current_node->base | (1<< (current_node->lvl-1));

   rebuild_right_flag = 0;
  }
  else if (current_node->right == ((void *)0))
  {
# 1913 "OM_DS_TREE.cilk"
   current_node->right = malloc(sizeof(Internal_Node));


   current_node->right->lvl = current_node->lvl -1;


   current_node->right->base = current_node->base | (1<< current_node->right->lvl);


   current_node->right->parent = current_node;

   current_node->right->right = current_node->right->left = ((void *)0);
   current_node->right->bl = ((void *)0);
  }

  else if ((current_node->lvl- current_node->right->lvl) != 1)
  {
      if (current_node->right->lvl == 0)
      {
    if (num_children_right== 1)
    {
        current_node->right = nodeArray[rightStart];
        current_node->right->parent = current_node;


     current_node->right->base = current_node->right->bl->tag = current_node->base | (1<< (current_node->lvl- 1));

     rebuild_right_flag = 0;
    }
    else
    {

        new_child = malloc(sizeof(Internal_Node));


        current_node->right = new_child;
        new_child->parent = current_node;


        new_child->left = new_child->right = ((void *)0);
        new_child->bl = ((void *)0);


        new_child->lvl = current_node->lvl - 1;


        new_child->num_children = num_children_right;


        new_child->base = current_node->base | (1<< new_child->lvl);
    }
      }
      else
      {


    current_node->right->lvl = current_node->lvl - 1;
    current_node->right->base = current_node->base | (1<< current_node->right->lvl);
      }
  }


  if (rebuild_right_flag)
    c_rebuild_tree(current_node->right, nodeArray, rightStart, rightEnd);

    }
}
# 1987 "OM_DS_TREE.cilk"
void c_par_build_array_from_rebalance_list (Internal_Node **buildArray, Internal_Node *current_node, int start, int end)
{

 int left_side_end;


 if (current_node->left->lvl != 0)
 {
  c_par_build_array_from_rebalance_list(buildArray, current_node->left, (start), (start + current_node->left->num_children- 1));
  left_side_end = start + current_node->left->num_children;
 }
 else
 {
  buildArray[start] = current_node->left;
  left_side_end = start + 1;
 }

 if (current_node->right->lvl != 0)
 {
  c_par_build_array_from_rebalance_list(buildArray, current_node->right, left_side_end, end);
 }
 else
 {
  buildArray[end] = current_node->right;
 }
}
