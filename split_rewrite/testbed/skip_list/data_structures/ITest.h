#ifndef __ITEST__
#define __ITEST__

#include "../framework/c_framework.h"

#define lock_fc(x,b) ( 0 == x.getNotSafe() &&  0 == x.getNotSafe() &&  0 == x.getNotSafe() && (b=x.compareAndSet(0, 0xFF)) )

extern int _gNumThreads;

typedef struct CasInfo {
		int _failed;
		int _succ;
		int _ops;
} CasInfo; 

CasInfo* new_CasInfo() {
		CasInfo* info;
    info = (CasInfo*)malloc(sizeof(CasInfo));
    info->_failed	= 0;
		info->_succ	  = 0;
		info->_ops	  = 0;
    return info; 
}

void CasInfo_reset(CasInfo* info) {
			info->_failed	= 0;
			info->_succ	  = 0;
			info->_ops	  = 0;
}
	
//list inner types ------------------------------
typedef struct SlotInfo {
	int volatile		_req_ans;		//here 1 can post the request and wait for answer
	int volatile		_time_stamp;	//when 0 not connected
	volatile SlotInfo* 	_next;			//when null not connected
	void*			_custem_info;
  bool      _deq_pending;
} SlotInfo;

SlotInfo* new_SlotInfo() {
	SlotInfo* info;
  info = (SlotInfo*) malloc(sizeof(SlotInfo));
  info->_req_ans     = _NULL_VALUE;
  info->_time_stamp  = 0;
	info->_next        = null;
	info->_custem_info = null;
  info->_deq_pending = false;
}

typedef struct ITest {
  static int _num_post_read_write;
	static final int _MAX_THREADS	= 1024;
	static final int _NULL_VALUE	= 0;
	static final int _DEQ_VALUE		= (INT_MIN+2);
	static final int _OK_DEQ		= (INT_MIN+3);
	//constants -----------------------------------
	final int		_NUM_THREADS;
	final boolean	_IS_USE_CONDITION;

	CasInfo		_cas_info_ary[_MAX_THREADS];
	int				_cpu_cash_contamination[8*1024*1024];
	int				_iTry[_MAX_THREADS];


	//list fields -----------------------------------
  CCP::ThreadLocal<SlotInfo*>	_tls_slot_info;
	CCP::AtomicReference<SlotInfo>	_tail_slot;
	int volatile					_timestamp;


} ITest;

//ITest helper function -----------------------------
inline void ITest_init_architecture_specific(ITest* test) { }
inline void ITest_machine_start_fc(ITest* test, final int iThread) { }
inline void ITest_machine_end_fc(ITest* test, final int iThread) { }
inline void ITest_thread_wait
  (ITest* test, final int iThread, final boolean is_short=false) 
{ 
  if(test->_NUM_THREADS < 8) {
	  CCP::Thread::sleep(0,1);
	} else {
	  CCP::Thread::yield();
	}
}
	
//list helper function --------------------------
SlotInfo* ITest_get_new_slot(ITest* test) {
	SlotInfo* my_slot= new SlotInfo();
	test->_tls_slot_info.set(my_slot);

	SlotInfo* curr_tail;
	do {
		curr_tail = test->_tail_slot.get();
		my_slot->_next = curr_tail;
	} while(false == test->_tail_slot.compareAndSet(curr_tail, my_slot));

	return my_slot;
}

void ITest_enq_slot(ITest* test, SlotInfo* p_slot) {
	SlotInfo* curr_tail;
	do {
		curr_tail = test->_tail_slot.get();
		test->p_slot->_next = curr_tail;
	} while(false == test->_tail_slot.compareAndSet(curr_tail, test->p_slot));
}

void ITest_enq_slot_if_needed(ITest* test, SlotInfo* p_slot) {
	if(null == p_slot->_next) {
		ITest_enq_slot(test, p_slot);
	}
}
	
ITest* new_ITest_def(){
  return new_ITest(_gNumThreads, false);
} 

ITest* new_ITest(final int num_threads, final boolean is_use_condition) {
  ITest* test;
  test = (ITest*)malloc(sizeof(ITest));
  test->_NUM_THREADS = num_threads;
  test->_IS_USE_CONDITION = is_use_condition;
  return test;  
}
	
//virtual ~ITest() {}
	
void ITest_cas_reset(ITest* test, final int iThread) {
	CasInfo_reset(test->_cas_info_ary[test->_MAX_THREADS]);
}

void ITest_print_cas(ITest* test) {
}

void ITest_print_custom(ITest* test) {
	int failed = 0;
	int succ = 0;
	int ops = 0;
  int i;

	for (i=0; i<test->_NUM_THREADS; ++i) {
		failed += test->_cas_info_ary[i]._failed;
		succ += test->_cas_info_ary[i]._succ;
		ops += test->_cas_info_ary[i]._ops;
	}
	printf(" 0 0 0 0 0 0 ( %d, %d, %d, %d )", ops, succ, failed, failed+succ);
}

int ITest_post_computation(ITest* test, final int iThread) {
	int sum=1;
  unsigned long i;
	if(test->_num_post_read_write > 0) {
		++test->_iTry[iThread];
		unsigned long start_indx = 
      ((unsigned long)(test->_iTry[iThread] * (iThread+1) * 17777675))%
      (7*1024*1024);

		for (i=start_indx; i<start_indx+test->_num_post_read_write; ++i) {
			sum += test->_cpu_cash_contamination[i];
			test->_cpu_cash_contamination[i] =  sum;
		}
	}
	return sum;
}
	
//..........................................................................
boolean ITest_add(ITest* test, final int iThread, final int inValue) = 0;
int ITest_remove(ITest* test, final int iThread, final int inValue) = 0;
int ITest_contain(ITest* test, final int iThread, final int inValue) = 0;

//..........................................................................
int ITest_size(ITest* test) = 0;
final char* ITest_name(ITest* test) = 0;
#endif
