#ifndef _Bottom_List_H
#define _Bottom_List_H

struct Bottom_List_s;

typedef struct OM_Node_s{
	struct OM_Node_s *next_e, *next_h;
	struct OM_Node_s *prev_e, *prev_h;
	int id;
	unsigned long tag_e, tag_h;
	struct Bottom_List_s * ds_h, * ds_e;

} OM_Node;

typedef struct Bottom_List_s {
	OM_Node *head,*tail;
	int size_e, size_h;
	int Reorder_flag_e, Reorder_flag_h;
	struct Bottom_List_s *next_e, *next_h;
	struct Bottom_List_s *prev_e, *prev_h;
	int id;

	unsigned long int tag_e, tag_h;


} Bottom_List;

typedef struct InsertRecord_s{
	Bottom_List *ds;
	OM_Node *x;
	OM_Node *y;
	int ID;

} InsertRecord;

#endif
