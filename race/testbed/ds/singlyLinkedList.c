#include "singlyLinkedList.h"

/*! Allocate the memory and pointers for a new
  singly-linked list
*/
SinLinkList * makeList() {

  SinLinkList * sll;

  sll = (SinLinkList*)malloc(sizeof(SinLinkList));
  
  sll->head = NULL;
  sll->tail = NULL;
  
  sll->size = 0;

  return sll;
}

/*! Free all dynamically allocated memory
  \param sll the list to free (including its nodes)
*/
void destroyList(SinLinkList * sll) {

  s_node * temp;
  s_node * current;

  current = sll->head;
  while(current != sll->tail)
  {
    temp = current;
    free(current);
    current = temp->next;
  }

  free(current);
  free(sll);
}

/*! Insert node at the end of the list containing data
  \param sll the list being inserted into
  \param data the data the inserted node will contain
*/
void insert(SinLinkList * sll, void * data) {
  
  s_node * node;
  node = (s_node*)malloc(sizeof(s_node));
  
  node->data = data;

  //If the list is empty, must assign the head
  if(sll->size == 0)
  {
    sll->head = sll->tail = node;
    sll->head->next = sll->tail;
    sll->size = sll->size + 1;
    return;
  }

  //Change tail to point to new node, make new node tail
  sll->tail->next = node;
  sll->tail = node;
  sll->size = sll->size + 1;

}

/*! Find the node with matching data if it exists
  \param sll The list to be searched
  \param data The data sought
  \return s_node* a pointer to the node with the desired data
*/
s_node * find(SinLinkList * sll, void * data) {
  
  s_node * current;
  
  current = sll->head;
  while(current->data != data) 
  {   
    current = current->next;
  }

  if(current->data == data)
    return current;

  return NULL;

}
