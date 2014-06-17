#include "RD.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/******************************************************
THIS WAS A PRELIMARY TEST - CHANGED NOW
******************************************************
int main(int argc, char **argv)
{
	int x = 3;
	int testint = 5;
	double d = 2.2;
	double ad = 6.3;
	char c = 'c';
	char testchar = 'a';
	char * testcharptr = "yyyy";
	char * name = "abc";
	
	TestStruct * abc;
	TestStruct * def;
	TestStruct * ghi;
	
	abc = (TestStruct*)malloc(sizeof(TestStruct));
	def = (TestStruct*)malloc(sizeof(TestStruct));
	ghi = (TestStruct*)malloc(sizeof(TestStruct));

	abc->member = x;
	abc->name = name;

	// Location References
	printf("%s\n", "This is int location:");
	printf("%p\n", &x);

	printf("%s\n", "This is double location:");
	printf("%p\n", &d);
	
	printf("%s\n", "This is char location:");
	printf("%p\n", &c);

	printf("%s\n", "This is the char * location:");
	printf("%p\n", &name);
	
	// Read Tests
	printf("\n%s\n", " == Now starting ReadTests == ");
	int * testlocInt = (int *) ReadTest(&x);
	double * testlocDub = (double *) ReadTest(&d);
	char * testlocChar = (char *) ReadTest(&c);
	char * testlocCharArr = (char *) ReadTest(&name);
	
	printf("testlocInt: %p\n", testlocInt);
	printf("testlocDub: %p\n", testlocDub);
	printf("testlocChar: %p\n", testlocChar);
	printf("testlocCharArr: %p\n", testlocCharArr);
	
	printf("\nDone with ReadTests\n");
	

	// Write Tests
	printf("\n == Now starting WriteTests ==\n");

	WriteTest(&x,(void*)&testint , sizeof(int));
	printf("x: %d\t <-should be 5\n", x);

	WriteTest(&d, (const void*)&ad, sizeof(double));
	printf("d: %f\t <-should be 6.3\n", d);

	WriteTest(&c,(void*)&testchar,sizeof(char));
	printf("c: %c\t <-should be a\n", c);

	WriteTest(&name, (void*)&testcharptr,sizeof(char*));
	printf("name: %s\t <-should be yyyy\n", name);
	
	printf("\nDone with WriteTests\n");


	
	//Struct tests
	printf("\n == Starting Struct Tests ==\n");
	
	printf("abc location: %p\n", &abc);
	printf("member: %p\n", &(abc->member));
	printf("name: %p\n", &(abc->name));
	
	printf("\nReadTest\n");
//	def = ReadTest(&abc);
//	def->member = *(int*) ReadTest(&(abc->member));
//	def->name = (char*)ReadTest(&(abc->name));
//	printf("def: %p\n", &def);
//	printf("def->member: %p\n", &(def->member));
//  printf("def->name: %p\n", &(def->name));
	
	assert(&abc == ReadTest(&abc));
	 
	printf("\nReadTests Done\n");
	
	printf("\nWriteTest\n");
	
	WriteTest(&ghi, &abc, sizeof(TestStruct));
	printf("ghi->member: %i\n", ghi->member);
	printf("ghi->name: %s\n", ghi->name);
	
	
	printf("EndTests");
	

	
  	return 0;
}


************************************************/


void Int_test()
{
	int temp = 5;
	void * x_int = create(sizeof(int));
	TestStruct * manualCast = (TestStruct*)x_int;
	
	printf("Data before write: %p\n", &(manualCast->data) );

	WriteTest(x_int, (void*)&temp);
	printf("Data after write: %p\n", &(manualCast->data) );
	//	assert(&temp == ReadTest(x_int));

	int y = *(int *)ReadTest(x_int);
	printf("Y should be 5: %i\n", y);
	
	
}

void Double_test()
{
	double temp = 6.33;
	
	void * x_double = create(sizeof(double));
	TestStruct * manualCast = (TestStruct*)x_double;
	
	printf("Data before write of double: %p\n",&(manualCast->data) );
	WriteTest(x_double, (void*)&temp);
	printf("Data after write: %p\n", &(manualCast->data) );
	//	assert(&temp == ReadTest(x_double));

    double y = *(double *)ReadTest(x_double);
	printf("Y should be 6.33: %f\n", y);
	
	
}

void CharPtr_test()
{
	char * temp = "Hello World";
	
	void * x_charPtr = create(11*sizeof(char));
	TestStruct * manualCast = (TestStruct*)x_charPtr;
	
	printf("Data before write: %p\n",&(manualCast->data) );
	WriteTest(x_charPtr, (void*)temp);
	printf("Data after write: %p\n",&(manualCast->data) );
	//	assert(temp == ReadTest(x_charPtr));

	char * y = (char*)ReadTest(x_charPtr);
	printf("Y should be \"Hello World\": %s\n", y);
	
}

void Struct_test()
{
	IntStruct * temp = (IntStruct*)malloc(sizeof(IntStruct));
	IntStruct * temp2 = (IntStruct*)malloc(sizeof(IntStruct));
	int tempInt = 8;
	char* tempCP = "AAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	
	temp->member = 3;
	temp->name = "This is my really name"; //22 characters
	
	void * x_intStruct = create(sizeof(IntStruct));
	
	TestStruct * manualCast = (TestStruct*)x_intStruct;
	
	//printf("Data before struct write: %p\n", &(manualCast->data) );
	WriteTest(x_intStruct, (void*)temp);
	//printf("Data after write: %p\n", &(manualCast->data) );
	//	assert(temp == ReadTest(x_charPtr));

	IntStruct * y = (IntStruct*)ReadTest(x_intStruct);
	
	printf("Y->member should be 3: %i\n", y->member);
	printf("Y->name should be \"This is my really name\": %s\n", y->name);

	printf("\nNow a test where the writes are done on a by-member basis\n");
	
	temp2->member;
	(char*)ReadTest(temp2_member);
	
	void * temp2_member = create(sizeof(int));
	//	(void*)(temp2->name) = create(29*sizeof(char));
	
	WriteTest(z_int, (void*)&tempInt);
	//	WriteTest((void*)(temp2->name), (void*)tempCP);
	temp2->member = *(int*)ReadTest(z_int);
	
	
	printf("Other struct should have int 8 and name A 29 times\n");
	printf("%i\n", temp2->member);
	//	printf("%s\n", temp2->name);
	
}


int main(int argc, char ** argv) 
{
	/* These work
	Int_test();
	
	Double_test();
	
	CharPtr_test();

	*/

	Struct_test();
	
	
	return 0;
	
}
