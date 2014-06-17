#ifndef RD_H
#define RD_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct OM_Node_s{

	struct OM_Node_s *next_english, *next_hebrew;
	int id; //needed?

} OM_Node;

typedef struct OM_DS_s {

	OM_Node *head,*tail;
	int size;

} OM_DS;

typedef struct 
{
	
	void * data;
	size_t size;
	OM_Node * left_r; //leftmost node that is reading
	OM_Node * right_r; //rightmost node that is reading
	OM_Node * left_w; //leftmost node that is writing
	OM_Node * right_w; //rightmost node that is writing


} MemoryStruct;

typedef struct
{
	int member;
	char * name;

} IntStruct;


void OM_DS_insert(void *, void*, void*, int);

int OM_DS_order(void*,void*,void*,int);

void * create(size_t size);

void * ReadTest(void *);

void WriteTest(void *, const void *);









#endif /* RD_H */
