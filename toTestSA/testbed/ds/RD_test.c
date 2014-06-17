#include "RD.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
void Int_test(){
	int temp = 5;
	void * x_int = create(sizeof(int));
	TestStruct * manualCast = (TestStruct*)x_int;
	printf("Data before write: %p\n", &(manualCast->data) );
	WriteTest(x_int, (void*)&temp);
	printf("Data after write: %p\n", &(manualCast->data) );
	int y = *(int *)ReadTest(x_int);
	printf("Y should be 5: %i\n", y);}
void Double_test(){
	double temp = 6.33;
	void * x_double = create(sizeof(double));
	TestStruct * manualCast = (TestStruct*)x_double;
	printf("Data before write of double: %p\n",&(manualCast->data) );
	WriteTest(x_double, (void*)&temp);
	printf("Data after write: %p\n", &(manualCast->data) );
    double y = *(double *)ReadTest(x_double);
	printf("Y should be 6.33: %f\n", y);}
void CharPtr_test(){
	char * temp = "Hello World";
	void * x_charPtr = create(11*sizeof(char));
	TestStruct * manualCast = (TestStruct*)x_charPtr;
	printf("Data before write: %p\n",&(manualCast->data) );
	WriteTest(x_charPtr, (void*)temp);
	printf("Data after write: %p\n",&(manualCast->data) );
	char * y = (char*)ReadTest(x_charPtr);
	printf("Y should be \"Hello World\": %s\n", y);}
void Struct_test(){
	IntStruct * temp = (IntStruct*)malloc(sizeof(IntStruct));
	IntStruct * temp2 = (IntStruct*)malloc(sizeof(IntStruct));
	int tempInt = 8;
	char* tempCP = "AAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	temp->member = 3;
	temp->name = "This is my really name"; //22 characters
	void * x_intStruct = create(sizeof(IntStruct));
	TestStruct * manualCast = (TestStruct*)x_intStruct;
	WriteTest(x_intStruct, (void*)temp);
	IntStruct * y = (IntStruct*)ReadTest(x_intStruct);
	printf("Y->member should be 3: %i\n", y->member);
	printf("Y->name should be \"This is my really name\": %s\n", y->name);
	printf("\nNow a test where the writes are done on a by-member basis\n");
	temp2->member;
	(char*)ReadTest(temp2_member);
	void * temp2_member = create(sizeof(int));
	WriteTest(z_int, (void*)&tempInt);
	temp2->member = *(int*)ReadTest(z_int);
	printf("Other struct should have int 8 and name A 29 times\n");
	printf("%i\n", temp2->member);} */

WorkerState * Create_ws() {
	WorkerState * ws = (WorkerState*)malloc(sizeof(WorkerState));
	ws->heb = (OM_DS*)malloc(sizeof(OM_DS));
	ws->eng = (OM_DS*)malloc(sizeof(OM_DS));
	ws->heb->size = 0;
	ws->eng->size = 0;
	return ws;
}

OM_Node * Node_generator(int id) {
	
	OM_Node * OMN = (OM_Node *)malloc(sizeof(OM_Node));
	OMN->next_english = OMN->next_hebrew = NULL;
	OMN->id = id;
	return OMN;
}

int main(int argc, char ** argv) 
{
	/**** Create Stuff to test ******/
	WorkerState * ws = Create_ws();
	OM_Node * OM1 = Node_generator(1);
	OM_Node * OM2 = Node_generator(2);
	OM_Node * OM3 = Node_generator(3);
	OM_Node * OM4 = Node_generator(4);
	OM_Node * OM5 = Node_generator(5);
	OM_Node * OM6 = Node_generator(6);
	OM_Node * OM7 = Node_generator(7);
	OM_Node * OM8 = Node_generator(8);
	/* Potential Current Nodes */
	OM_Node * OM9 = Node_generator(9);
	OM_Node * OM10 = Node_generator(10);
	OM_Node * OM11 = Node_generator(11);
	OM_Node * OM12 = Node_generator(12);
	OM_Node * OM13 = Node_generator(13);

	/*** Write Values ***/
	double dub = 3.14;
	double dub1 = 4.14;
	double dub2 = 5.14;
	double dub3 = 7.14;
	double dub4 = 8.14;
	double dub5 = 9.14;

	/***** Make English List ******
		   12346785
	*******************************/
	OM_DS_insert(ws->eng, NULL, OM1, ENGLISH_ID);
	OM_DS_insert(ws->eng, OM1, OM5, ENGLISH_ID);
	OM_DS_insert(ws->eng, OM1, OM8, ENGLISH_ID);
	OM_DS_insert(ws->eng, OM1, OM7, ENGLISH_ID);
	OM_DS_insert(ws->eng, OM1, OM7, ENGLISH_ID);
	OM_DS_insert(ws->eng, OM1, OM4, ENGLISH_ID);
	OM_DS_insert(ws->eng, OM1, OM3, ENGLISH_ID);
	OM_DS_insert(ws->eng, OM1, OM2, ENGLISH_ID);

	/**** Hebrew 13245687 ********/
	OM_DS_insert(ws->heb, NULL, OM1, HEBREW_ID);
	OM_DS_insert(ws->heb, OM1, OM7, HEBREW_ID);
	OM_DS_insert(ws->heb, OM1, OM8, HEBREW_ID);
	OM_DS_insert(ws->heb, OM1, OM6, HEBREW_ID);
	OM_DS_insert(ws->heb, OM1, OM5, HEBREW_ID);
	OM_DS_insert(ws->heb, OM1, OM4, HEBREW_ID);
	OM_DS_insert(ws->heb, OM1, OM2, HEBREW_ID);
	OM_DS_insert(ws->heb, OM1, OM3, HEBREW_ID);

	/**** No Races, simulate as if all sequential ***/
	void * test_double = create(sizeof(double));
	//
	ws->current_node = OM9;
	OM_DS_insert(ws->eng, OM1, OM9, ENGLISH_ID);
	OM_DS_insert(ws->heb, OM1, OM9, HEBREW_ID);
	WriteTest(ws, test_double, dub);
	//
	ws->current_node = OM10;
	OM_DS_insert(ws->eng, OM9, OM10, ENGLISH_ID);
	OM_DS_insert(ws->heb, OM9, OM10, HEBREW_ID);
	WriteTest(ws, test_double, dub2);
	//
	ws->current_node = OM11;
	OM_DS_insert(ws->eng, OM10, OM11, ENGLISH_ID);
	OM_DS_insert(ws->heb, OM10, OM11, HEBREW_ID);
	WriteTest(ws, test_double, dub3);
	//
	ws->current_node = OM12;
	OM_DS_insert(ws->eng, OM11, OM12, ENGLISH_ID);
	OM_DS_insert(ws->heb, OM11, OM12, HEBREW_ID);
	WriteTest(ws, test_double, dub4);
	//
	ws->current_node = OM13;
	OM_DS_insert(ws->eng, OM12, OM13, ENGLISH_ID);
	OM_DS_insert(ws->heb, OM12, OM13, HEBREW_ID);
	WriteTest(ws, test_double, dub5);




	

	/***** Free Memory of stuff *********/
	free(OM1);
	free(OM2);
	free(OM3);
	free(OM4);
	free(OM5);
	free(OM6);
	free(OM7);
	free(OM8);
	free(OM9);
	free(OM10);
	free(OM11);
	free(OM12);
	free(OM13);
	free(ws->eng);
	free(ws->heb);
	free(ws);

	return 0;
	
}
