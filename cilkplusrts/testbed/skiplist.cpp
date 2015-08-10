#include <unistd.h>
#include <cassert>
#include <cstdio>
#include <algorithm>

#include "skiplist.h"
SkipList slist;

void print_info(int iteration,
                size_t original_size, size_t numElements, int* level,
                Node** new_nodes, Node** new_nodes_back,
                int* prev_nodes, int maxInsertLevel)
{
  int i,j;
  Node* temp;
  printf("New nodes:");
  for (i = 0; i < numElements; i++) {
    printf(" %d->(", new_nodes[i]->data);
    for (j = 0; j <= level[i]; j++) {
      temp = new_nodes[i]->forward[j];
      if (temp == NULL || i > iteration) printf("NULL,");
      else printf("%d,", new_nodes[i]->forward[j]->data);
    }
    printf("),");
  }
  printf("\nNew nodes back:");
  for (i = 0; i < numElements; i++) {
    printf(" %d->(", new_nodes_back[i]->data);
    for (j = 0; j <= level[i]; j++) {
      temp = new_nodes_back[i]->forward[j];
      if (temp == NULL || i > iteration) printf("NULL,");
      else printf("%d,", new_nodes_back[i]->forward[j]->data);
    }
    printf("),");
  }

  printf("\n");
  printf("levels:");
  for (i = 0; i < numElements; i++) {
    printf(" %d,", level[i]);
  }
  printf("\n");
  printf("prevnodes:");
  for (i = 0; i < (maxInsertLevel+1) * numElements; i++) {
    printf(" %d,", prev_nodes[i]);
  }
  printf("\n");
  printf("max Insert Level: %d\n", maxInsertLevel);
  return;
}

void batch_createNodes(int idx, void* info)
{
  void** data  = (void**) info;
  //data is {&maxInsertLevel, level, data, new_nodes,new_nodes_back}
  // {int*,int*,int*,Node**}
  int i;
  Node* y;
  Node* yback;

  //Old way of using rand.
  for (i=0; rand() < RAND_MAX/2 && i < MAXLEVEL; i++); // Use the RNG;
  /*   for (i=0; rands[temp++] && i < MAXLEVEL; i++) { */
  /*   	if (temp>=randsize-1){ */
  /*   temp=0; */
  /* } */
  /* } */

  ((int*)(data[1]))[idx] = i; //Sets the level in the level array.
  if (*((int*)(data[0]))<i) {
    *((int*)(data[0]))=i;  //Changes max level currently.
  }
  if ((y = (Node*) malloc(sizeof(Node) + i*sizeof(Node*)))==0){
    printf ("memory allocation error createNodes\n");
    exit(1);
  }
  ((Node**)(data[3]))[idx]=y;

  if ((yback = (Node*) malloc(sizeof(Node) + i*sizeof(Node*)))==0){
    printf ("memory allocation error createNodes\n");
    exit(1);
  }
  ((Node**)(data[4]))[idx]=yback;

  // For using Operations
  //  y->data = ((int*)(data[2]))[2*idx+1];

  // For using regular ints
  y->data=((int*)(data[2]))[idx];
  yback->data=((int*)(data[2]))[idx];
}


void batch_relateNodes(int idx, void* infos)
{
  //Here info is {new_nodes,level,size,prev_nodes, maxInsertLevel, new_nodes_back}
  void** info = (void**) infos;
  Node** new_nodes = (Node**)info[0];
  int* level = ((int*)info[1]);
  int size = *((int*)(info[2]));
  int* prev_node_indices = ((int*)info[3]);
  int maxlevel = *((int*)(info[4]));
  Node** new_nodes_back = (Node**)info[5];
  Node* y = new_nodes[idx];
  Node* z;
  int check = idx+1;
  int i,j;


  for (i=0; i <= level[idx]; i++){

    y->forward[i] = NIL;

    for (j = check; j < size; j++){

      if (level[j] >= i) {

        y->forward[i] = new_nodes[j]; //sets the forward of the (idx)th node at the ith level to node j.
        new_nodes_back[j]->forward[i] = y;

        prev_node_indices[(maxlevel+1)*j+i] = idx; //sets the previous pointer
        check = j;

        break;
      }
    }
  }
}

void batch_mergeLists(int idx, void* infos)
{
  int i;
  void** info = (void**) infos;
  Node** new_nodes = ((Node**)info[0]);
  int* level = ((int*)info[1]);
  int maxlevel = *((int*)(info[2]));
  int* prev_nodes = ((int*)info[4]);
  Node** new_nodes_back = ((Node**)info[5]);
  int prev_node;

  Node* x = slist.hdr;
  Node* y = new_nodes[idx];
  Node* yback=new_nodes_back[idx];
  Node *update[MAXLEVEL+1];
  Node* z;
  assert(y->data == yback->data);

  for (i = slist.listLevel; i >= 0; i--) {
    while (x->forward[i] != NIL
           && compLT(x->forward[i]->data, y->data)) {
      x=x->forward[i];
    }
    //No duplicates!
    if (x != NIL
        && x->forward[i]!=NIL && compEQ(x->forward[i]->data, y->data)) {
      return;
    }

    // If the node is still less than us, there's nothing to change.
    update[i]=x;

  }

  for (i=0; i<=level[idx];i++){
    if (y->forward[i] == NIL
        || compLT(update[i]->forward[i]->data, y->forward[i]->data)
        || compEQ(update[i]->forward[i]->data, y->forward[i]->data)) {
      if (update[i]->forward[i] != NIL) {
        y->forward[i] = update[i]->forward[i];
      }
    }
    prev_node = prev_nodes[(maxlevel+1)*idx+i];
    /* if (prev_node!=-1){ */
    /*   z = new_nodes[prev_node]; */
    /*   assert(yback->forward[i]->data==z->data); */
    /*   assert(yback->forward[i]==z); */
    /*   assert(level[prev_node] >= i); */
    /* } */
    if (prev_node == -1 ||  compLT(new_nodes[prev_node]->data, update[i]->data)|| compEQ(new_nodes[prev_node]->data, update[i]->data)) {
      yback->forward[i] = update[i];
      prev_nodes[(maxlevel+1)*idx+i] = -2;
    }

  }
}

//int g_touched[1000];

void parallel_foo(int* data, int i, size_t size)
{
  int j = 42 * 55 + 86;
  j += i;
  data[i % size] = j;
  //  g_touched[i] = -1;
}

int bar(int x)
{
  for (int i = 0; i < 100; ++i) {
    x += i;
    x *= 2;
    x /= 42;
  }
  return x;
}

void batch_insert_par(void* dataStruct, void* data,
                      size_t numElements, void* results)
{
  //  memset(g_touched, 0, sizeof(int)*1000);
  int* d = (int*) data;
  cilk_for(int i = 0; i < 500 * numElements; ++i) {
    parallel_foo(d, i, numElements);
  }
  //  d = (int*)bar(42);
}


void batch_insert_par2(void* dataStruct, void* data,
                       size_t numElements, void* results)
{
  dataStruct = (void*) &slist;
  int i,j,k,check;
  int* level = (int*) calloc(numElements,sizeof(int));
  int original_size = get_size();
  int size;
  int maxInsertLevel = 0;
  int currMaxLevel;
  //Node* x, *y;
  Node** new_nodes = (Node**) calloc(numElements,sizeof(Node*)); // ***
  Node** new_nodes_back = (Node**) calloc(numElements,sizeof(Node*)); // ***
  int* prev_nodes;
  //Par_for to create nodes
  void* createNodeArgs[5] = {&maxInsertLevel, level, data, new_nodes, new_nodes_back};
  void* relatedNodeArgs[6] = {new_nodes,level,&numElements,level,level,new_nodes_back};
  void* mergeListArgs[6] = {new_nodes,level, &numElements, data, level,new_nodes_back};

  Node* current = slist.hdr;
  Node* back_node;

  // static int g_total = 0;
  // g_total += numElements;
  // printf("Batch of size %i, total inserted %i\n", numElements,
  //        g_total);
  // if (g_total % 100 == 0) fprintf(stderr, ".");
  // if (g_total % 1000 == 0) fprintf(stderr, "%i\n", g_total);

  std::sort((int*)data, ((int*)data) + numElements);

  //remove duplicates
  i = 0;
  if ( numElements > 1) {
    for (j = 1; j < numElements; j++) {
      if ( ((int*)data)[j] != ((int*)data)[i]) {
        ((int*)data)[++i] = ((int*)data)[j];
      }
    }
    numElements = i+1;
  }

  currMaxLevel=((SkipList*)dataStruct)->listLevel;

  // // @TODO parallelize
  for (i = 0; i < numElements; i++) {
    batch_createNodes(i, createNodeArgs);
  }

  prev_nodes = (int*) calloc((maxInsertLevel+1)*numElements,sizeof(int)); //Makes a prev array.
  //Fill in rest of info.
  for (i=0; i<(maxInsertLevel+1)*numElements; i++){
    prev_nodes[i] = -1;
  }

  relatedNodeArgs[3] = prev_nodes;
  relatedNodeArgs[4] = &maxInsertLevel;
  mergeListArgs[4] = prev_nodes;
  mergeListArgs[2] = &maxInsertLevel;

  //Switch max level!
  if (maxInsertLevel>currMaxLevel) {
    ((SkipList*)dataStruct)->listLevel=maxInsertLevel;
  }

  // @TODO parallelize
  for (i = 0; i < numElements; i++) {
    batch_relateNodes(i, relatedNodeArgs);
  }

  //Find the place for all the elements to merge list
  cilk_for(int i = 0; i < numElements; ++i) {
    batch_mergeLists(i, mergeListArgs);
  }

  //Do an actual merge
  for(i = 0; i < numElements; i++) {
    for(j = 0; j <= level[i]; j++){
      if (prev_nodes[(maxInsertLevel+1)*i+j] == -2) {
        back_node = new_nodes_back[i]->forward[j];
        back_node->forward[j] = new_nodes[i];
      }
    }
  }

  // size = get_size();
  // if (size < original_size + numElements) {
  //   print_info(numElements, original_size, numElements, level, new_nodes, new_nodes_back,
  //              prev_nodes, maxInsertLevel);
  //   printf("Old size: %u\n", original_size);
  //   printf("New size: %u\n", size);

  //   current = slist.hdr;
  //   printf("\nNew Skip List: ");
  //   for (i = 0; i <= get_size(); i++) {
  //     printf("->%d", current->data);
  //     current = current->forward[0];
  //   }
  //   printf("\n");

  //   assert(get_size() == original_size + numElements);
  // }


  free(new_nodes_back);
  free(level);
  free(prev_nodes);
}

void SkipList_insert(int val)
{
  cilk_batchify(&batch_insert_par, NULL, val, sizeof(int));
}

Node *insertNode(T data)
{
  int i, newLevel;
  Node *update[MAXLEVEL+1];
  Node *x;

  /***********************************************
   *  allocate node for data and insert in list  *
   ***********************************************/

  /* find where data belongs */
  x = slist.hdr;
  for (i = slist.listLevel; i >= 0; i--) {
    while (x->forward[i] != NIL
           && compLT(x->forward[i]->data, data))
      x = x->forward[i];
    update[i] = x;
  }
  x = x->forward[0];
  if (x != NIL && compEQ(x->data, data)) {
    /* printf("%d already in skiplist. %i was originally responsible.\n", */
    /*        x->data, resp[x->data]); */
    return(x);
  }

  /* determine level */
  for (newLevel = 0; rand() < RAND_MAX/2 && newLevel < MAXLEVEL; newLevel++);

  if (newLevel > slist.listLevel) {
    for (i = slist.listLevel + 1; i <= newLevel; i++)
      update[i] = NIL;
    slist.listLevel = newLevel;
  }

  /* make new node */
  if ((x = (Node*) malloc(sizeof(Node) +
                  newLevel*sizeof(Node *))) == 0) {
    printf ("insufficient memory (insertNode)\n");
    exit(1);
  }
  x->data = data;

  /* update forward links */
  for (i = 0; i <= newLevel; i++) {
    x->forward[i] = update[i]->forward[i];
    update[i]->forward[i] = x;
  }
  return(x);
}

void deleteNode(T data) {
  int i;
  Node *update[MAXLEVEL+1], *x;

  /*******************************************
   *  delete node containing data from list  *
   *******************************************/

  /* find where data belongs */
  x = slist.hdr;
  for (i = slist.listLevel; i >= 0; i--) {
    while (x->forward[i] != NIL
           && compLT(x->forward[i]->data, data))
      x = x->forward[i];
    update[i] = x;
  }
  x = x->forward[0];
  if (x == NIL || !compEQ(x->data, data)) return;

  /* adjust forward pointers */
  for (i = 0; i <= slist.listLevel; i++) {
    if (update[i]->forward[i] != x) break;
    update[i]->forward[i] = x->forward[i];
  }

  free (x);

  /* adjust header level */
  while ((slist.listLevel > 0)
         && (slist.hdr->forward[slist.listLevel] == NIL))
    slist.listLevel--;
}

Node *findNode(T data) {
  int i;
  Node *x = slist.hdr;

  /*******************************
   *  find node containing data  *
   *******************************/

  for (i = slist.listLevel; i >= 0; i--) {
    while (x->forward[i] != NIL
           && compLT(x->forward[i]->data, data))
      x = x->forward[i];
  }
  x = x->forward[0];
  if (x != NIL && compEQ(x->data, data)) return (x);
  return(0);
}

void initList() {
  int i;
  /**************************
   *  initialize skip list  *
   **************************/
  /* rands=calloc(randsize,sizeof(int)); */
  /* for (i=0;i<randsize;i++){ */
  /*     rands[i]=rand()%2; */
  /* } */

  if ((slist.hdr = (Node*) malloc(sizeof(Node) + MAXLEVEL*sizeof(Node *))) == 0) {
    printf ("insufficient memory (initList)\n");
    exit(1);
  }
  for (i = 0; i <= MAXLEVEL; i++)
    slist.hdr->forward[i] = NIL;
  slist.listLevel = 0;
}

int get_size() {
  int size = 0;
  Node* pCur = slist.hdr;

  while (pCur->forward[0] != NIL) {
    ++size;
    pCur = pCur->forward[0];
  }

  return size;
}
