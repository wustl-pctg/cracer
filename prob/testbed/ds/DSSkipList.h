#ifndef __DS_SKIP_LIST__
#define __DS_SKIP_LIST__
 
#include "../framework/c_framework.h"
 
//consts
//static final int _MAX_LEVEL	= 20;
#define _MAX_LEVEL 20

//types
typedef struct Node Node;
struct Node {
		int		_key;
		int		_top_level;
		int		_counter;
		Node* 	_next[_MAX_LEVEL+1];
};

typedef struct DSSkipList {
  //SkipList fields
  Volatile_u64*	_random_seed;
  Node*			    _head;
	Node*     		_tail;
  //Flat Combining fields
	//AtomicInteger	_fc_lock;
	//char							_pad1[CACHE_LINE_SIZE];
	int		    _NUM_REP;
	int		    _REP_THRESHOLD;
	Node*			preds[_MAX_LEVEL + 1];
	Node*			succs[_MAX_LEVEL + 1];
	//SlotInfo*		_saved_remove_node[1024];
} DSSkipList;


static Node* getNewNode(final int inKey) {
	Node* new_node;
  new_node = (Node*) malloc(sizeof(Node));
	new_node->_key			= inKey;
	new_node->_top_level	= _MAX_LEVEL;
	new_node->_counter		= 1;
	return new_node;
}

static Node* getNewNode_at_height(final int inKey, final int height) {
	Node* new_node;
  new_node = (Node*) malloc(sizeof(Node) + height + 1 - _MAX_LEVEL);
	new_node->_key			= inKey;
	new_node->_top_level	= height;
	new_node->_counter		= 1;
	return new_node;
}



//methods
inline_ int DSSkipList_randomLevel(DSSkipList* list) {
	int x = (int)(Volatile_u64_get(list->_random_seed)  & 0xFFFFFFUL);
	int level = 2;
	x ^= x << 13;
	x ^= x >> 17;
	Volatile_u64_set(list->_random_seed,  x ^= x << 5 );
	if ((x & 0x80000001) != 0) {// test highest and lowest bits
		//printf("0\n");  fflush(stdout);
		return 1;
	}
	while (((x >>= 1) & 1) != 0) 
		++level;
	//printf("%d\n",level);  fflush(stdout);
	if(level > (_MAX_LEVEL-1))
		return (_MAX_LEVEL-1);
	else
		return level;
}

inline_ Node* DSSkipList_find(DSSkipList* list, final int key) {
	Node* pPred;
	Node* pCurr;
	Node* found_node;
  int iLevel;
	pPred = list->_head;
  found_node = null;

	for (iLevel = _MAX_LEVEL-1; iLevel >= 0; --iLevel) {

		pCurr = pPred->_next[iLevel];

		while (key > pCurr->_key) {
			pPred = pCurr; 
			pCurr = pPred->_next[iLevel];
		}

		if (null == found_node && key == pCurr->_key) {
			found_node = pCurr;
		}

		list->preds[iLevel] = pPred;
		list->succs[iLevel] = pCurr;
	}

	return found_node;
}


/*inline_ void flat_combining() {

	int num_removed = 0;
	final int top_level = randomLevel();
	for (int iTry=0;iTry<_NUM_REP; ++iTry) {

		int num_changes=0;
		SlotInfo* curr_slot = _tail_slot.get();
		Memory::read_barrier();

		while(null != curr_slot->_next) {

      if ( curr_slot->_deq_pending ) {
        curr_slot = curr_slot->_next;
        continue;
      }

			final int inValue = curr_slot->_req_ans;
			if(inValue > _NULL_VALUE) {
				++num_changes;
				curr_slot->_req_ans = _NULL_VALUE;
				curr_slot->_time_stamp = _NULL_VALUE;

				//ADD ......................................................
				Node* node_found = find(inValue);
				if (null != node_found) {
					++(node_found->_counter);
					curr_slot = curr_slot->_next;
					continue;
				}

				// first link succs ........................................
				// then link next fields of preds ..........................
				Node* new_node = Node::getNewNode(inValue, top_level);
				Node** new_node_next = new_node->_next;
				Node** curr_succ = succs;
				Node** curr_preds = preds;

				for (int level = 0; level < top_level; ++level) {
					*new_node_next = *curr_succ;
					(*curr_preds)->_next[level] = new_node;
					++new_node_next;
					++curr_succ;
					++curr_preds;
				}

				//..........................................................
				curr_slot = curr_slot->_next;
				continue;

			} else if(_DEQ_VALUE == inValue) {
				++num_changes;
        curr_slot->_deq_pending = true;
				//REMOVE ...................................................

				_saved_remove_node[num_removed] = curr_slot;
                                        ++num_removed;
				curr_slot = curr_slot->_next;
				continue;
			} else {
				curr_slot = curr_slot->_next;
				continue;
			}

		} //while on slots

		//if(num_changes < _REP_THRESHOLD)
		//	break;
	}

	//..................................................................
	Node* remove_node = (_head->_next[0]);
	int max_level = -1;
	for (int iRemove=0; iRemove<num_removed; ++iRemove) {

		if ( _tail != remove_node ) {
      SlotInfo* dequeuer = _saved_remove_node[iRemove];
      dequeuer->_req_ans = -remove_node->_key;
      dequeuer->_time_stamp = _NULL_VALUE;
      dequeuer->_deq_pending = false;

			--(remove_node->_counter);
			if(0 == remove_node->_counter) {
				if(remove_node->_top_level > max_level) {
					max_level = remove_node->_top_level;
				}
				remove_node = remove_node->_next[0];
			} 
		}
		else
		{
      SlotInfo* dequeuer = _saved_remove_node[iRemove];
			dequeuer->_req_ans = _NULL_VALUE;
			dequeuer->_time_stamp = _NULL_VALUE;
			dequeuer->_deq_pending = false;
		}
	}

	if(-1 != max_level) {
		Node* pred = _head;
		Node* curr;

		for (int iLevel = (max_level-1); iLevel >= 0; ) {
			curr = pred->_next[iLevel];

			if(0 != curr->_counter) {
				_head->_next[iLevel] = curr;
				--iLevel;
			} else {
				pred = curr; 
				curr = pred->_next[iLevel];
			}
		}
	}
}*/

//methods

	/*FCSkipList()
	: _head( Node::getNewNode(INT_MIN) ),
	  _tail( Node::getNewNode(INT_MAX) ),
	  _NUM_REP( Math::Min(2, _NUM_THREADS)),
	  _REP_THRESHOLD((int)(Math::ceil(_NUM_THREADS/(1.7))))
	{
		//initialize head to point to tail .....................................
		for (int iLevel = 0; iLevel < _head->_top_level; ++iLevel)
			_head->_next[iLevel] = _tail;

		//initialize the slots .........................................
		_tail_slot.set(new SlotInfo());
	}*/

DSSkipList* new_DSSkipList() {
  int iLevel;
  DSSkipList* list;
  list = (DSSkipList*)malloc(sizeof(DSSkipList));
  list->_head = getNewNode(INT_MIN);
  list->_tail = getNewNode(INT_MAX);
  //list->_NUM_REP = Min(2, _NUM_THREADS);
  //list->_REP_THRESHOLD((int)(ceil(_NUM_THREADS/(1.7))));
  for(iLevel=0; iLevel < list->_head->_top_level; ++iLevel)
    list->_head->_next[iLevel] = list->_tail;
  //list->_tail_slot.set(new_SlotInfo());
  return list;
} 

	//~FCSkipList() {	}

//methods

//deq ......................................................
/*boolean DSSkipList_add(DSSkipList* list, final int iThread, final int inValue) {
  SlotInfo* my_slot = list->_tls_slot_info.get();
	if(null == my_slot)
		my_slot = get_new_slot();

	SlotInfo* volatile&	my_next = my_slot->_next;
	int volatile& my_re_ans = my_slot->_req_ans;
	my_re_ans = inValue;

	do {
		if (null == my_next)
			enq_slot(my_slot);

		boolean is_cas = false;
		if(lock_fc(_fc_lock, is_cas)) {
			machine_start_fc(iThread);
			flat_combining();
			_fc_lock.set(0);
			machine_end_fc(iThread);
			return true;
		} else {
			Memory::write_barrier();
			while(_NULL_VALUE != my_re_ans && 0 != _fc_lock.getNotSafe()) {
				thread_wait(iThread);
			} 
			Memory::read_barrier();
			if(_NULL_VALUE == my_re_ans) {
				return true;
			}
		}
	} while(true);
}*/

boolean DSSkipList_add(DSSkipList* list, final int iThread, final int inValue) {
  int top_level = DSSkipList_randomLevel(list);	
  int level;
	Node* new_node;
	Node** new_node_next;
	Node** curr_succ; 
	Node** curr_preds; 

  //ADD ......................................................
	Node* node_found;
  node_found = DSSkipList_find(list, inValue);
	if (null != node_found) {
		++(node_found->_counter);
		//curr_slot = curr_slot->_next; 
		//continue;
		System_err_println("Hit condition I haven't figured out in DSSL_add");
	}

	// first link succs ........................................
	// then link next fields of preds ..........................
  new_node = getNewNode_at_height(inValue, top_level);
  new_node_next = new_node->_next;
  curr_succ = list->succs;
  curr_preds = list->preds;

	for (level = 0; level < top_level; ++level) {
	  *new_node_next = *curr_succ;
		(*curr_preds)->_next[level] = new_node;
		++new_node_next;
		++curr_succ;
		++curr_preds;
	}
  return 0;
}
//deq ......................................................
/*int DSSkipList_remove(DSSkipList* list, final int iThread, final int inValue) {
  SlotInfo* my_slot = _tls_slot_info.get();
	if(null == my_slot)
		my_slot = get_new_slot();

	SlotInfo* volatile&	my_next = my_slot->_next;
	int volatile& my_re_ans = my_slot->_req_ans;
	my_re_ans = _DEQ_VALUE;

	do {
		if(null == my_next)
			enq_slot(my_slot);

		boolean is_cas = false;
		if(lock_fc(_fc_lock, is_cas)) {
			machine_start_fc(iThread);
			flat_combining();
			_fc_lock.set(0);
			machine_end_fc(iThread);
			return -(my_re_ans);
		} else {
			Memory::write_barrier();
			while(_DEQ_VALUE == my_re_ans && 0 != _fc_lock.getNotSafe()) {
				thread_wait(iThread);
			}
			Memory::read_barrier();
			if(_DEQ_VALUE != my_re_ans) {
				return -(my_re_ans);
			}
		}
	} while(true);
}*/

//peek ......................................................................
int DSSkipList_contain(DSSkipList* list, final int iThread, final int inValue) {
	return 0; //_NULL_VALUE;
}

//methods

int DSSkipList_size(DSSkipList* list) {
	return 0;
}

final char* DSSkipList_name(DSSkipList* list) {
	return "DSSkipList";
}

#endif
