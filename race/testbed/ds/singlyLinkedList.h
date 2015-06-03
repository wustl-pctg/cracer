#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#include <stdlib.h>
#include <stdio.h>

/*! A very simple singly-linked list
to be used as an aspect of each node in the 
red-black trees utilized in race-detect applications

Author: Shane Deiley
*/

typedef struct s_node {

  struct s_node * next;
  void * data;

} s_node;

typedef struct {
  s_node * head;
  s_node * tail;
  int size;
} SinLinkList;

SinLinkList * makeList();
void destroyList(SinLinkList * sll);
void insert(SinLinkList * sll, void * data);
s_node * find(SinLinkList * sll, void * data);

#endif //SINGLYLINKEDLIST_H
