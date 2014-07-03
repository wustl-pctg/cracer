#ifndef _OM_DS_H
#define _OM_DS_H

typedef struct OM_Node_s{
	struct OM_Node_s *next_e, *next_h;
	struct OM_Node_s *prev_e, *prev_h;
	int id;
	unsigned int tag_e, tag_h;

} OM_Node;

typedef struct OM_DS_s {
	OM_Node *head,*tail;
	int size;
	int Reorder_flag;	
	struct OM_DS_s *next_e, *next_h;
	struct OM_DS_s *prev_e, *prev_h;
	int id;

	unsigned long int tag_e, tag_h;


} OM_DS;

typedef struct InsertRecord_s{
	OM_DS *ds;
	OM_Node *x;
	OM_Node *y;
	int ID;

} InsertRecord;

#endif
