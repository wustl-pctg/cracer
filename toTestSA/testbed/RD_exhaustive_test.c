#include <cilk-lib.cilkh>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#define READ_ARG 	0
#define WRITE_ARG 	1

/**\union general union to test on*/
typedef union test_union_gen_u {
	int x, y, z;

	void *a, *b, *c;

} test_union_gen;

/**\struct General struct to test
*/
typedef struct test_struct_gen_s {

	int * a, b;

	void * c;

	double *d, e;

	float *f, g;

	test_union_gen *u, v;

	size_t size;
} test_struct_gen;

/**\fn Assign_struct_random_vars
 * \brief Assign random variables and allocate variables for members of struct
*/
inline void Assign_struct_random_vars(test_struct_gen * s){

	/// Assign a
       s->a 	= malloc(sizeof(int)); 
	*s->a = (int) rand();	 
	/// Assign b
	s->b	=  (int) rand();
	/// Allocate c
	s->c 	= malloc(sizeof(1));
	/// Assign d
	s->d 	= malloc(sizeof(double));
	*s->d 	= (double) rand();
	/// Assign e
	s->e 	= (double) rand();
	/// Assign f
	s->f 	= malloc(sizeof(float));
	*s->f 	= (float) rand();
	/// Assign g
	s->g 	= (float) rand();
	/// Assign u
	s->u 	= malloc(sizeof(test_union_struct));
	assignUnionRandomVars(s->u);
	/// Assign v
	assignUnionRandomVars(&s->v);
	/// Assign size
	s->size = sizeof(*s);
	
}

/**\fn Assign_union_random_vars
 * \brief Assign random variables and allocate variables for members of union
 */
inline void Assign_union_random_vars(test_union_gen * u){

    	/// Assign a
	u->a = (int)rand();
    	/// Assign b
	u->b = (int)rand();
    	/// Assign c
	u->c = (int)rand();

    	/// Allocate x
	u->x = malloc(sizeof(rand() * 8));
    	/// Allocate y
	u->y = malloc(sizeof(rand() * 8));
    	/// Allocate z
	u->z = malloc(sizeof(rand() * 8));
	
}

/** \fn Free_test_union_members
 *  \brief Free all members of the union that were dynamically allocated
 */
inline void Free_test_union_members(test_union_gen *u){
	/// Free a
	free(u->a);
	/// Free b
	free(u->b);
	/// Free c
	free(u->c);
}

/** \fn FreeTestStructMembers
 *  \brief Free all members of the struct that were dynamically allocated
 */
inline void Free_test_struct_member(test_struct_gen *s){
	/// Free a
	free(s->a);
	/// Free c
	free(s->c);
	/// Free d
	free(s->d);
	/// Free f
	free(s->f);
	/// Free u
	free(s->u);
}

/// A spawned write helper function
cilk void cilk_write_func(void *rd_ds, test_struct_gen * s, int * result){
	/// Write to rd_ds
	WRITE_b(rd_ds, s, results);
	
	/// Check test struct equal to passed in race detect data structure
	assert(Check_test_struct_equal(rd_ds, s));
}

/// A spawned read helper function
cilk void cilk_read_func(void *rd_ds, test_struct_gen * s, int * result){
    	/// Read from rd_ds and store in tmp rd_ds
	*s = READ_b(rd_ds, test_struct_gen, results);

	/// Compare to test_Struct passed in
	assert(Check_test_struct_equal(rd_ds, s));
}

/// Functions used by rd_parent_child_test
cilk void seq_parent_child_spawned(void * rd_ds, const int op1, const int op2 ){
	
}

cilk void par_parent_child_spawned(void * rd_ds, const int op1, const int op2 ){
	
}

inline void seq_parent_child_c(void * rd_ds, const int op1, const int op2 ){
	/// Create var to store race detection result
	int race_detect_result1 = -1, race_detect_results2 = -1;

	/// Create tmp variables to assign to ds
	test_struct_gen tmp_s;
	test_union_gen tmp_u;
	
	/// Give struct and union random variables and allocate memory to members
	Assign_struct_random_vars(&tmp_s);
	Assign_union_random_vars(&tmp_u);

	/// Execute first operation	
	/// Races possible
	if (op1 == READ_ARG){
		
		/// Spawn a function that calls read
		spawn cilk_read_func(rd_ds, &tmp_s, &race_detect_result1);
	}
	else	{
	    	/// Spawn a function that calls write
		spawn cilk_write_func(rd_ds, &tmp_s, &race_detect_results1);
	}

	/// Execute second operation
	/// Races possible
	if (op2 == READ_ARG){
		/// Read into tmp structure
		tmp_s = READ_b(rd_ds,test_struct_gen, &race_detect_result2);
	}
	else	{
	    	/// Write into tmp structure
		WRITE_b(rd_ds, &tmp_s, &race_detect_results2); 
	}

	//Sync to end parallel threads
	sync;

	/// Check if our race conditions were as expected
	
	if (op1 == READ_ARG && op2 == READ_ARG)
	    	assert(race_detect_results1 || race_detect_Results2 == 0);
	else if (op1 == READ_ARG && op2 == WRITE_ARG
	/// Free dynamically allocated members of test structs.
	Free_test_union_members(&tmp_u);
	Free_test_struct_members(&tmp_s);
}
/// End functions used by rd_parent_child_test
	
/**\fn rd_parent_child_test
*\brief Tests the operations op1 in the parent and op2 in the child function. First runs sequentially then in parallel.
*\param op1 either 0 to do a read first or 1 to do a write in the parent
*\param op2 either 0 to do a read second or 1 to do a write in the child
*/
cilk void rd_parent_child_test(){
	
	
}

/// Functions used by rd_same_function_test
cilk void seq_same_func_spawned(void * rd_ds, const int op1, const int op2 ){
	seq_same_func_c(rd_ds, op1, op2);	
}

inline void seq_same_func_c(void * rd_ds, const int op1, const int op2 ){
	/// Create var to store race detection result
	int race_detect_result = 0;

	/// Create tmp variables to assign to ds
	test_struct_gen tmp_s;
	test_union_gen tmp_u;
	
	/// Give struct and union random variables and allocate memory to members
	Assign_struct_random_vars(&tmp_s);
	Assign_union_random_vars(&tmp_u);

	/// Execute first operation	
	/// No races expected

	if (op1 == READ_ARG){
		/// Read from tmp structure
		tmp_s = READ_b(rd_ds,test_struct_gen, &race_detect_result);
		/// Check test struct equal to passed in race detect data structure
		assert(Check_test_struct_equal(rd_ds, tmp_s));
		/// Assert that no race happened
		assert(race_detect_result == 0);
	}
	else	{
		WRITE_b(rd_ds, &tmp_s); 
		/// Check test struct equal to passed in race detect data structure
		assert(Check_test_struct_equal(rd_ds, tmp_s));
		/// Assert that no race happened
		assert(race_detect_result == 0);
	}

	/// Execute second operation
	/// No races expected
	if (op2 == READ_ARG){
		/// Read from tmp structure
		tmp_s = READ_b(rd_ds,test_struct_gen, &race_detect_result);
		/// Check test struct equal to passed in race detect data structure
		assert(Check_test_struct_equal(rd_ds, tmp_s));
		/// Assert that no race happened
		assert(race_detect_result == 0);
		tmp_s = READ_b(rd_ds,test_struct_gen, &race_detect_result);
	}
	else	{
		WRITE_b(rd_ds, &tmp_s); 
		/// Check test struct equal to passed in race detect data structure
		assert(Check_test_struct_equal(rd_ds, tmp_s));
		/// Assert that no race happened
		assert(race_detect_result == 0);
	}

	/// Free dynamically allocated members of test structs.
	Free_test_union_members(&tmp_u);
	Free_test_struct_members(&tmp_s);
}


/// End functions used by rd_same_function_test


/**\fn rd_same_function_test
*\brief Tests the operations op1 and op2 in that order in the same cilk function. This should be a serial operation -- no races are expected.
*\param op1 either 0 to do a read first or 1 to do a write first
*\param op2 either 0 to do a read second or 1 to do a write second
*/
cilk void rd_same_function_test(){

	/// Declare the four types of sequential race detect variables
	void * seq_r_r, * seq_r_w, *seq_w_r, *seq_w_w;
	
	/// Declare the four types of parallel race detect variables
	void * par_r_r, * par_r_w, *par_w_r, *par_w_w;
	
	/// Instantiate all of the race detect structures 
	seq_r_r = RD_INIT(test_struct_gen);
	seq_r_w = RD_INIT(test_struct_gen);
	seq_w_r = RD_INIT(test_struct_gen);
	seq_w_w = RD_INIT(test_struct_gen);
	par_r_r = RD_INIT(test_struct_gen);
	par_r_w = RD_INIT(test_struct_gen);
	par_w_r = RD_INIT(test_struct_gen);
	par_w_w = RD_INIT(test_struct_gen);

	/// Run sequentially read/read
	spawn seq_same_func_spawned(seq_r_r, READ_ARG, READ_ARG);sync;
	seq_same_func_c(seq_r_r, READ_ARG, READ_ARG);

	/// Run sequentially read/write
	spawn seq_same_func_spawned(seq_r_w, READ_ARG, WRITE_ARG);sync;
	seq_same_func_c(seq_r_r, READ_ARG, WRITE_ARG);

	/// Run sequentially write/read
	spawn seq_same_func_spawned(seq_r_r, WRITE_ARG, READ_ARG);sync;
	seq_same_func_c(seq_r_r, WRITE_ARG, READ_ARG);

	/// Run sequentially write/write
	spawn seq_same_func_spawned(seq_r_r, WRITE_ARG, WRITE_ARG);sync;
	seq_same_func_c(seq_r_r, WRITE_ARG, WRITE_ARG);

	/// Run parallel read/read
	spawn par_same_func_spawned(par_r_r, READ_ARG, READ_ARG);sync;
	par_same_func_c(par_r_r, READ_ARG, READ_ARG);

	/// Run parallel read/write
	spawn par_same_func_spawned(par_r_w, READ_ARG, WRITE_ARG);sync;
	par_same_func_c(par_r_r, READ_ARG, WRITE_ARG);

	/// Run parallel write/read
	spawn par_same_func_spawned(par_r_r, WRITE_ARG, READ_ARG);sync;
	par_same_func_c(par_r_r, WRITE_ARG, READ_ARG);

	/// Run parallel write/write
	spawn par_same_func_spawned(par_r_r, WRITE_ARG, WRITE_ARG);sync;
	par_same_func_c(par_r_r, WRITE_ARG, WRITE_ARG);


	/// Free all race detect data structures
	RD_free(_cilk_ws, seq_r_r);
	RD_free(_cilk_ws, seq_r_w);
	RD_free(_cilk_ws, seq_w_r);
	RD_free(_cilk_ws, seq_w_w);
	RD_free(_cilk_ws, par_r_r);
	RD_free(_cilk_ws, par_r_w);
	RD_free(_cilk_ws, par_w_r);
	RD_free(_cilk_ws, par_w_w);
}

/// A spawned write helper function
cilk void spawn_rd_cousin_write(void *rd_ds, test_struct_gen * s, int result){
	/// Write to rd_ds
	WRITE_b(rd_ds, s, &result);
	
	/// Check test struct equal to passed in race detect data structure
	assert(Check_test_struct_equal(rd_ds, s));
}

/// A spawned read helper function
cilk void spawn_rd_cousin_read(void *rd_ds, test_struct_gen * s, int result){
    /// Read from rd_ds and store in tmp rd_ds
	*s = READ_b(rd_ds, test_struct_gen, &result);

	/// Compare to test_Struct passed in
	assert(Check_test_struct_equal(rd_ds, s));
}


/// End rd_same_function_test
/// Start functions for rd_cousin_test

cilk void cousin_sequential(const int op1, const int op2, void * mem_ptr, test_struct_gen * s) {

	/// Booleans to use to check if races are detected
	int race_detected1, race_detected2;

	/// Spawn op1, then sync
	if(op1) {
		spawn spawn_rd_cousin_write(mem_ptr, s, race_detected1);
	} else {
		spawn spawn_rd_cousin_read(mem_ptr, s, race_detected1);
	} sync;
	
	/// Spawn op2, then sync
	if(op2) {
		spawn spawn_rd_cousin_write(mem_ptr, s, race_detected2);
	} else {
		spawn spawn_rd_cousin_write(mem_ptr, s, race_detected2);
	} sync;
}

cilk void cousin_parallel(const int op1, const int op2, void * mem_ptr, test_struct_gen * s) {

	/// Booleans to use to check if races are detected
	int race_detected1, race_detected2;

	/// Spawn op1 *DON'T SYNC*
	if(op1) {
		spawn spawn_rd_cousin_write(mem_ptr, s,race_detected1);
	} else {
		spawn spawn_rd_cousin_read(mem_ptr, s, race_detected1);
	}
	
	/// Spawn op2
	if(op2) {
		spawn spawn_rd_cousin_write(mem_ptr, s, race_detected2);
	} else {
		spawn spawn_rd_cousin_write(mem_ptr, s, race_detected2);
	}

	/// Now Sync
	sync;
}



cilk void rd_cousin_test() {


	/// Declare the four types of race detect variables
	/// These will be used in cousins that are sequential
	void * seq_r_r, * seq_r_w, *seq_w_r, *seq_w_w;
	
	/// Declare the four types of parallel race detect variables
	/// These will be used in cousins that are parallel
	void * par_r_r, * par_r_w, *par_w_r, *par_w_w;

	/// Declare various empty structs for passing into the functions
	
	
	/// Instantiate all of the race detect structures 
	seq_r_r = RD_INIT(test_struct_gen);
	seq_r_w = RD_INIT(test_struct_gen);
	seq_w_r = RD_INIT(test_struct_gen);
	seq_w_w = RD_INIT(test_struct_gen);
	par_r_r = RD_INIT(test_struct_gen);
	par_r_w = RD_INIT(test_struct_gen);
	par_w_r = RD_INIT(test_struct_gen);
	par_w_w = RD_INIT(test_struct_gen);













	/// Free all race detect data structures
	RD_free(_cilk_ws, seq_r_r);
	RD_free(_cilk_ws, seq_r_w);
	RD_free(_cilk_ws, seq_w_r);
	RD_free(_cilk_ws, seq_w_w);
	RD_free(_cilk_ws, par_r_r);
	RD_free(_cilk_ws, par_r_w);
	RD_free(_cilk_ws, par_w_r);
	RD_free(_cilk_ws, par_w_w);


}








cilk int main(int argv, char* argc){
	srand(time(NULL));
    	/// Case 1:	
	/// This function tests each combination of read/write
	/// within the same spawned function. None of these should
	/// have any races within them, as they are all executed serially.
	spawn rd_same_function_test();

	/// Case 2:
	/// These functions tests each combination of read/write
	/// within a parent function and its child, in both the serial and
	/// parallel case.	
	spawn rd_parent_child_test();

	/// Case 3:
	/// These functions test each combination of read/write
	/// within a parent function and its grandchildren and great grandchildren
	/// in both the serial and parallel case.
	spawn rd_gparent_gchild_test();

	/// Case 4:
	/// These functions test each combination of read/write
	/// within a parent function and its reat grandchildren
	/// in both the serial and parallel case.
	spawn rd_ggparent_ggchild_test();
	
	
	/// Case 5:
	/// These functions test each combination of read/write
	/// within cousin nodes and sibling nodes
	/// in both the serial and parallel case.
	spawn rd_cousin_test();
	
	
	sync;	
	printf("Test completed fully as expected. No errors.\n");
	return;
}