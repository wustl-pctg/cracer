#include "OM_DS.h"
static unsigned long MAX_NUMBER=      ~0;
static int INT_BIT_SIZE=  64;
static double OVERFLOW_CONSTANT=  1.5;
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 33
Bottom_List *create_bl  (void)
{

 Bottom_List *list=   malloc(sizeof((*list)));


 list->head = list->tail = ((void *)0);


 list->size = list->reorder_flag = 0;
# 47
 return list;
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 56
Top_List *create_tl  (void)
{

 Top_List *list=   malloc(sizeof((*list)));

 Bottom_List *bl;

 list->size = 0;


 list->head = list->tail = ((void *)0);
# 71
 bl = create_bl();

 first_insert_tl(list, bl);

 return list;
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 84
void first_insert_bl (Bottom_List *ds,  OM_Node *y)
{
# 101
 y->ds = ds;
 ds->head = ds->tail = y;
 y->next = y->prev = ((void *)0);
 ds->size = 1;
 y->tag = 0;
}


struct _cilk_insert_frame{CilkStackFrame header;struct{OM_Node*x;OM_Node*y;}scope0;struct{InsertRecord*ir;}scope1;};struct _cilk_insert_args{OM_Node*x;OM_Node*y;};static void _cilk_insert_slow(CilkWorkerState*const _cilk_ws,struct _cilk_insert_frame*_cilk_frame);static CilkProcInfo _cilk_insert_sig[]={{0,sizeof(struct _cilk_insert_frame),_cilk_insert_slow,0,0}};
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_FAST_PROCEDURE
# 109
void insert     (CilkWorkerState*const _cilk_ws,OM_Node*x,OM_Node*y){struct _cilk_insert_frame*_cilk_frame;CILK2C_INIT_FRAME(_cilk_frame,sizeof(struct _cilk_insert_frame),_cilk_insert_sig);CILK2C_START_THREAD_FAST();{
 InsertRecord *ir=  (InsertRecord *)malloc(sizeof(InsertRecord));
 ir->x = x;
 ir->y = y;

 Cilk_batchify(_cilk_ws, batchInsertOp, ((void *)0), ir, sizeof(InsertRecord), ((void *)0));
{CILK2C_BEFORE_RETURN_FAST();return;}}}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_SLOW_PROCEDURE
# 109
static void _cilk_insert_slow(CilkWorkerState*const _cilk_ws,struct _cilk_insert_frame*_cilk_frame){OM_Node*x;OM_Node*y;CILK2C_START_THREAD_SLOW();switch (_cilk_frame->header.entry) {}x=_cilk_frame->scope0.x;y=_cilk_frame->scope0.y;{
 InsertRecord *ir=  (InsertRecord *)malloc(sizeof(InsertRecord));
 ir->x = x;
 ir->y = y;

 Cilk_batchify(_cilk_ws, batchInsertOp, ((void *)0), ir, sizeof(InsertRecord), ((void *)0));
{CILK2C_SET_NORESULT();CILK2C_BEFORE_RETURN_SLOW();return;}}}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 109
static void _cilk_insert_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v){(void)_cilk_ws;(void)_cilk_procargs_v;insert(_cilk_ws,((struct _cilk_insert_args*)_cilk_procargs_v)->x,((struct _cilk_insert_args*)_cilk_procargs_v)->y);
# 115
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 109
void mt_insert(CilkContext*const context,OM_Node*x,OM_Node*y){struct _cilk_insert_args*_cilk_procargs;_cilk_procargs=(struct _cilk_insert_args*)Cilk_malloc_fixed(sizeof(struct _cilk_insert_args));_cilk_procargs->x=x;_cilk_procargs->y=y;Cilk_start(context,_cilk_insert_import,_cilk_procargs,0);Cilk_free(_cilk_procargs);
# 115
}

struct _cilk_batchInsertOp_frame{CilkStackFrame header;struct{void*dataStruct;void*data;size_t size;void*result;}scope0;struct{int i;InsertRecord*ir;InsertRecord*ir_next;}scope1;};struct _cilk_batchInsertOp_args{void*dataStruct;void*data;size_t size;void*result;};static void _cilk_batchInsertOp_slow(CilkWorkerState*const _cilk_ws,struct _cilk_batchInsertOp_frame*_cilk_frame);static CilkProcInfo _cilk_batchInsertOp_sig[]={{0,sizeof(struct _cilk_batchInsertOp_frame),_cilk_batchInsertOp_slow,0,0}};
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_FAST_PROCEDURE
# 117
void batchInsertOp      (CilkWorkerState*const _cilk_ws,void*dataStruct,void*data,size_t size,void*result){struct _cilk_batchInsertOp_frame*_cilk_frame;CILK2C_INIT_FRAME(_cilk_frame,sizeof(struct _cilk_batchInsertOp_frame),_cilk_batchInsertOp_sig);CILK2C_START_THREAD_FAST();
{
 int i=  0;
 InsertRecord *ir=   (InsertRecord *)data;InsertRecord*ir_next;

 for (; i< size; i++) {
  insert_internal(ir->x, ir->y);
  ir_next = ir + sizeof(InsertRecord *);

  ir = ir_next;
 }

{CILK2C_BEFORE_RETURN_FAST();return;}}}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_SLOW_PROCEDURE
# 117
static void _cilk_batchInsertOp_slow(CilkWorkerState*const _cilk_ws,struct _cilk_batchInsertOp_frame*_cilk_frame){void*dataStruct;void*data;size_t size;void*result;CILK2C_START_THREAD_SLOW();switch (_cilk_frame->header.entry) {}dataStruct=_cilk_frame->scope0.dataStruct;data=_cilk_frame->scope0.data;size=_cilk_frame->scope0.size;result=_cilk_frame->scope0.result;
{
 int i=  0;
 InsertRecord *ir=   (InsertRecord *)data;InsertRecord*ir_next;

 for (; i< size; i++) {
  insert_internal(ir->x, ir->y);
  ir_next = ir + sizeof(InsertRecord *);

  ir = ir_next;
 }

{CILK2C_SET_NORESULT();CILK2C_BEFORE_RETURN_SLOW();return;}}}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 117
static void _cilk_batchInsertOp_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v)
{(void)_cilk_ws;(void)_cilk_procargs_v;batchInsertOp(_cilk_ws,((struct _cilk_batchInsertOp_args*)_cilk_procargs_v)->dataStruct,((struct _cilk_batchInsertOp_args*)_cilk_procargs_v)->data,((struct _cilk_batchInsertOp_args*)_cilk_procargs_v)->size,((struct _cilk_batchInsertOp_args*)_cilk_procargs_v)->result);
# 129
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 117
void mt_batchInsertOp(CilkContext*const context,void*dataStruct,void*data,size_t size,void*result)
{struct _cilk_batchInsertOp_args*_cilk_procargs;_cilk_procargs=(struct _cilk_batchInsertOp_args*)Cilk_malloc_fixed(sizeof(struct _cilk_batchInsertOp_args));_cilk_procargs->dataStruct=dataStruct;_cilk_procargs->data=data;_cilk_procargs->size=size;_cilk_procargs->result=result;Cilk_start(context,_cilk_batchInsertOp_import,_cilk_procargs,0);Cilk_free(_cilk_procargs);
# 129
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 137
void insert_internal(OM_Node *x,  OM_Node *y)

{

 Bottom_List *ds=   x->ds;
# 153
 y->ds = ds;

 if (x == ds->tail) 
 {

  y->tag = (x->tag>>  1) + (MAX_NUMBER>>  1);


  if (x->tag & 0x1 == 0x1) y->tag += 1;


  if ((MAX_NUMBER-  x->tag) <= 1) 
  {
# 183
   split_bl(ds->parent, ds);
   insert_internal(x, y);
   return;
  }

  ds->tail = y;
 }
 else 
 {

  y->tag = (x->tag>>  1) + (x->next->tag>>  1);


  if (x->next->tag & x->tag & 0x1 == 0x1) y->tag += 1;
# 200
  if ((x->next->tag-  x->tag) <= 1) 
  {
# 221
   split_bl(ds->parent, ds);
   insert_internal(x, y);
   return;
  }

 }


 y->prev = x;
 y->next = x->next;
 x->next = y;
 if (y->next != ((void *)0)) y->next->prev = y;

 ds->size += 1;
# 245
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 253
void first_insert_tl (Top_List *list,  Bottom_List *y)
{
# 272
 y->parent = list;
 list->head = list->tail = y;
 y->tag = 0;

 y->reorder_flag = 0;
 y->next = y->prev = ((void *)0);
 list->size += 1;
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 287
void first_insert (Top_List *list,  OM_Node *y)
{

 first_insert_bl(list->head, y);
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 299
void insert_tl (Bottom_List *x, Bottom_List *y)
{

 Top_List *list=   x->parent;
# 308
 if (x == list->tail) 
 {

  y->tag = (x->tag>>  1) + (MAX_NUMBER>>  1);


  if (x->tag & 0x1 == 0x1) y->tag++;
 }
 else 
 {

  y->tag = (x->tag>>  1) + (x->next->tag>>  1);


  if (x->next->tag & x->tag & 0x1 == 0x1) y->tag++;
 }


 if ( (x == list->tail&&  (MAX_NUMBER - x->tag<=  1))  ||
   (x != list->tail&&  (x->next->tag - x->tag<=  1))) 
 {

  rebalance_tl(list, x);


  insert_tl(x, y);
 }
 else 
 {
# 340
  y->prev = x;
  y->next = x->next;
  x->next = y;
  if (y->next != ((void *)0)) 
   y->next->prev = y;
  else 
   list->tail = y;


  y->parent = list;
  list->size += 1;
 }

}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 365
int order (OM_Node *x,  OM_Node *y)
{

 if (x->ds == y->ds) 
 {
  if (x->tag < y->tag) 
   return 1;
  else 
   return 0;
 }


 else 
 {
  if (x->ds->tag < y->ds->tag) 
   return 1;
  else 
   return 0;
 }
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 393
void split_bl (Top_List *list,  Bottom_List *list_to_split)
{
 OM_Node *current=   list_to_split->head;OM_Node*middle_node;


 Bottom_List *to_add=   create_bl();
# 402
 int node_count=  1;
# 406
 unsigned long skip_size=      MAX_NUMBER / ((list_to_split->size>>  1)+  2);
# 410
 while (node_count < (list_to_split->size>>  1)) 
 {
  current->next->tag = current->tag + skip_size;
  current = current->next;
  node_count++;
 }


 to_add->size = list_to_split->size - node_count;
 list_to_split->size = node_count;


 list_to_split->tail = current;
 middle_node = current->next;
 current->next = ((void *)0);


 current = middle_node;


 to_add->head = middle_node;


 middle_node->prev = ((void *)0);


 current->tag = 0;


 current->ds = to_add;


 while (current->next != ((void *)0)) 
 {
  current = current->next;
  current->ds = to_add;
  current->tag = current->prev->tag + skip_size;
 }


 to_add->tail = current;
 current->next = ((void *)0);


 list_to_split->reorder_flag = to_add->reorder_flag = 0;
# 458
 insert_tl(list_to_split, to_add);
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 468
void rebalance_tl (Top_List *list,  Bottom_List *pivot)
{

 Bottom_List *lList=  pivot;Bottom_List*rList=pivot;


 double overflow_density;double overflow_threshold;double i=-1;
 unsigned long enclosing_tag_range;unsigned long lTag;unsigned long rTag;unsigned long num_elements_in_sublist=2;unsigned long skip_size;
# 496
 do
 {

  if (lList->prev) 
  {
   num_elements_in_sublist++;
   lList = lList->prev;
   lTag = lList->tag;
  }
  if (rList->next) 
  {
   num_elements_in_sublist++;
   rList = rList->next;
   rTag = rList->tag;
  }
  else 
  {

   rTag = MAX_NUMBER;
  }


  enclosing_tag_range = rTag - lTag;

  i = ceil( log2((double)enclosing_tag_range));

  overflow_threshold = pow(OVERFLOW_CONSTANT, -1.0 * i);

  overflow_density = (num_elements_in_sublist) / (double)enclosing_tag_range;
 }
 while ( (enclosing_tag_range == 0||  overflow_density > overflow_threshold)  && enclosing_tag_range != MAX_NUMBER);


 skip_size = (unsigned long)     ( enclosing_tag_range/  (num_elements_in_sublist+  1));
# 539
 relabel_tl_tag_range(lList, rList, skip_size);
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 548
void relabel_tl_tag_range (Bottom_List *start, Bottom_List *end, const long skip_size)
{
 Bottom_List *current=   start;
 current->tag = start->tag;


 do
 {
  current->next->tag = current->tag + skip_size;
  current = current->next;
 }
 while (current != end);
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 568
void rebalance_bls (Top_List *list)
{

 Bottom_List *current;
 current = list->head;

 while (current!= ((void *)0)) 
 {
  if (current->reorder_flag== 1) 
   split_bl(list, current);
  current = current->next;
 }
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 589
void print_tl (Top_List *list)
{
 Bottom_List *current=   list->head;

 printf ( "(size = %i) Head->", list->size);
 while (current != ((void *)0)) 
 {
  printf ( "(%lu)->", current->tag);
  current = current->next;
 }
 printf ( "Tail\n");
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 608
void print_bl (Bottom_List *list)
{
 OM_Node *current=   list->head;

 printf ( "(size = %i) Head->", list->size);
 while (current != ((void *)0)) 
 {
  printf ( "(%i | %lu)->", current->ID, current->tag);
  current = current->next;
 }
 printf ( "Tail\n");
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 628
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
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 651
void free_tl (Top_List *list)
{
 Bottom_List *next;Bottom_List*current=list->head;


 while (current != ((void *)0)) {
  next = current->next;
  free_bl(current);
  current = ((void *)0);
  current = next;
 }


 free(list);
 list = ((void *)0);
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 674
void check_sub_correctness (Top_List *list)
{
 Bottom_List *current=   list->head;
 OM_Node *cur_node;


 while (current != ((void *)0)) 
 {

  cur_node = current->head;
  while (cur_node!= current->tail) 
  {
   if ( !(cur_node->tag<  cur_node->next->tag)) 
    printf("Node tags are out of order\n");
   cur_node = cur_node->next;
  }
  current = current->next;
 }
}
