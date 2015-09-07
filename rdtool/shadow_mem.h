#include <cstdio>

#define LOG_KEY_SIZE  3 
#define LOG_TBL_SIZE 21

// macro for address manipulation for shadow mem
#define ADDR_TO_KEY(addr) ((uint64_t) ((uint64_t)addr >> LOG_KEY_SIZE))


template < typename T >
class ShadowMem {
private:
  struct shadow_tbl { T *shadow_entries[1<<LOG_TBL_SIZE]; };

  struct shadow_tbl **shadow_dir;

  T** find_slot(uint64_t key, bool alloc) {
    shadow_tbl *tbl = shadow_dir[key>>LOG_TBL_SIZE];
    if (!alloc && !tbl)
      return NULL;
    else if (!tbl) {
      struct shadow_tbl *new_tbl = new struct shadow_tbl();
      shadow_dir[key>>LOG_TBL_SIZE] = new_tbl;
      tbl = new_tbl;
    }
    T** slot =  &tbl->shadow_entries[key&((1<<LOG_TBL_SIZE) - 1)];
    return slot;
  }

public:
  ShadowMem() {
    shadow_dir = 
      new struct shadow_tbl *[1<<(48 - LOG_TBL_SIZE - LOG_KEY_SIZE)]();
  }

  T* find(uint64_t key) {
    T **slot = find_slot(key, false);
    if (slot == NULL)
      return NULL;
    return *slot;
  }

  //  clear()
  void insert(uint64_t key, T *val) {
    T **slot = find_slot(key, true);
    *slot = val;
  }

  void erase(uint64_t key) {
    T **slot = find_slot(key, false);
    if (slot != NULL)
      *slot = NULL;
  }

};
