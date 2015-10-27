#include <inttypes.h>
 
#include "print_addr.h"
#include "mem_access.h"


// get the start and end indices and gtype to use for accesing 
// the readers / writers lists; the gtype is the largest granularity
// that this memory access is aligned with 
enum GrainType_t 
MemAccessList_t::get_mem_index(uint64_t addr, size_t size, 
                               int& start, int& end) {
  om_assert(size < MAX_GRAIN_SIZE || 
      (size == MAX_GRAIN_SIZE && IS_ALIGNED_WITH_GTYPE(addr, EIGHT)));

  start = (int) (addr & (uint64_t)(MAX_GRAIN_SIZE-1));
  end = (int) ((addr + size) & (uint64_t)(MAX_GRAIN_SIZE-1));
  if(end == 0) end = MAX_GRAIN_SIZE;

  enum GrainType_t gtype = mem_size_to_gtype(size);
  if( IS_ALIGNED_WITH_GTYPE(addr, gtype) == false ) { gtype = ONE; }
  om_assert( (end-start) == size );

  return gtype;
}

// Check races on memory represented by this mem list with this read access
// Once done checking, update the mem list with this new read access
void MemAccessList_t::check_races_and_update_with_read(uint64_t inst_addr, 
                              uint64_t addr, size_t mem_size,
                              om_node *curr_estrand, om_node *curr_hstrand) {

  DBG_TRACE(DEBUG_MEMORY, "check race w/ read addr %lx and size %lu.\n",
            addr, mem_size);
  om_assert( addr >= start_addr && 
                       (addr+mem_size) <= (start_addr+MAX_GRAIN_SIZE) );

  // check races with the writers
  // start (inclusive) and end (exclusive) indices covered by this mem access; 
  int start, end; // implicitly initialized once only
  MemAccess_t *writer = NULL;
  const enum GrainType_t gtype = get_mem_index(addr, mem_size, start, end);
  enum GrainType_t min_gtype = gtype;

  om_assert(start >= 0 && start < end && end <= MAX_GRAIN_SIZE);

  if(writer_gtype == UNINIT) {
    goto update; // no writers to check races with 
  } else if( writer_gtype < gtype ) {
    // stride the list using the min granularity of two
    min_gtype = writer_gtype;
  } else if(writer_gtype > gtype) { // find the writer that covers the start
    writer = writers[ get_prev_aligned_index(start, writer_gtype) ];
  }
  om_assert(IS_ALIGNED_WITH_GTYPE(end, min_gtype)); 

  // walk through the indices that this memory access cover to check races
  // against any writer found within this range 
  for(int i = start; i < end; i += gtype_to_mem_size[min_gtype]) {
    if( IS_ALIGNED_WITH_GTYPE(i, writer_gtype) ) {
      // encountered the next writer within the range; update writer
      writer = writers[i]; // can be NULL
    }
    if( writer && writer->races_with(curr_estrand, curr_hstrand) ) {
      report_race(writer->rip, inst_addr, start_addr+i, WR_RACE);
    }
  }

update:
  MemAccess_t *reader = NULL;
  MemAccess_t *new_reader = new MemAccess_t(curr_estrand, curr_hstrand, inst_addr);
  // MemAccess_t *new_lreader = new MemAccess_t(curr_estrand, curr_hstrand, inst_addr);
  // MemAccess_t *new_rreader = new MemAccess_t(curr_estrand, curr_hstrand, inst_addr);
  // bool lreader_used = false;
  // bool rreader_used = false;
  bool reader_used = false;

  // now we update the left-most readers list with this access 
  if( lreader_gtype == UNINIT ) {
    //    pthread_spin_lock(&lreader_lock);
    lreader_gtype = gtype;
    //    pthread_spin_unlock(&lreader_lock);
  } else if( lreader_gtype > gtype ) {
    //    pthread_spin_lock(&lreader_lock);
    break_list_into_smaller_gtype(lreaders, &lreader_gtype, gtype);
    //    pthread_spin_unlock(&lreader_lock);
  }
  // now lreader_gtype = min{ old lreader_gtype, gtype };
  om_assert(lreader_gtype <= gtype);
  om_assert(IS_ALIGNED_WITH_GTYPE(end, lreader_gtype)); 

  // update the readers list with this mem access; same start / end indices
  for(int i = start; i < end; i += gtype_to_mem_size[lreader_gtype]) {
    reader = lreaders[i];
    if(reader == NULL) {
      //      pthread_spin_lock(&lreader_lock);
      new_reader->inc_ref_count();
      lreaders[i] = new_reader;
      reader_used = true;
      //      pthread_spin_unlock(&lreader_lock);
    } else { // potentially update the left-most reader 
      // replace it if 
      // - the new reader is to the left of the old lreader 
      //   (i.e., comes first in serially execution)  OR
      // - there is a path from old lreader to this reader

      //      om_assert(reader->ref_count > (int16_t)0);

      //      pthread_spin_lock(&g_worker_mutexes[self].mut);
      bool is_leftmost = om_precedes(curr_estrand, reader->estrand) 
        || om_precedes(reader->hstrand, curr_hstrand);
      //      pthread_spin_unlock(&g_worker_mutexes[self].mut);
      if(is_leftmost) {
        //        pthread_spin_lock(&lreader_lock);
        if(reader->dec_ref_count() == 0) {
          //          delete reader;
        }
        new_reader->inc_ref_count();
        lreaders[i] = new_reader;
        reader_used = true;
        //        pthread_spin_unlock(&lreader_lock);
      }
    }
  }


  // now we update the right-most readers list with this access 
  if( rreader_gtype == UNINIT ) {
    //    pthread_spin_lock(&rreader_lock);
    rreader_gtype = gtype;
    //    pthread_spin_unlock(&rreader_lock);
  } else if( rreader_gtype > gtype ) {
    //    pthread_spin_lock(&rreader_lock);
    break_list_into_smaller_gtype(rreaders, &rreader_gtype, gtype);
    //    pthread_spin_unlock(&rreader_lock);
  }
  // now rreader_gtype = min{ old rreader_gtype, gtype };
  om_assert(rreader_gtype <= gtype);
  om_assert(IS_ALIGNED_WITH_GTYPE(end, rreader_gtype)); 

  for(int i = start; i < end; i += gtype_to_mem_size[rreader_gtype]) {
    reader = rreaders[i];
    if(reader == NULL) {
      //      pthread_spin_lock(&rreader_lock);
      new_reader->inc_ref_count();
      rreaders[i] = new_reader;
      reader_used = true;
      //      pthread_spin_unlock(&rreader_lock);
    } else { // potentially update the right-most reader 
      // replace it if 
      // - the new reader is to the right of the old rreader 
      //   (i.e., comes later in serially execution)  OR
      // - there is a path from old rreader to this reader
      // but actually the second condition subsumes the first, so if the 
      // first condition is false, there is no point checking the
      // second
      //      om_assert(reader->ref_count > (int16_t)0);
      //      pthread_spin_lock(&g_worker_mutexes[self].mut);
      bool is_rightmost = om_precedes(reader->estrand, curr_estrand);
      //      pthread_spin_unlock(&g_worker_mutexes[self].mut);

      if(is_rightmost) {
        //        pthread_spin_lock(&rreader_lock);
        if(reader->dec_ref_count() == 0) {
          //delete reader;
        }
        new_reader->inc_ref_count();
        rreaders[i] = new_reader;
        reader_used = true;
        //        pthread_spin_unlock(&rreader_lock);
      }
    }
  }


  //  if(new_lreader->ref_count == 0) { 
  if (!reader_used) delete new_reader;
  // if (!lreader_used) {
  //   delete new_lreader; 
  // }
  // //  if(new_rreader->ref_count == 0) { 
  // if (!rreader_used) {
  //   delete new_rreader; 
  // }

}

// Check races on memory represented by this mem list with this write access
// Also, update the writers list.  
// Very similar to check_races_and_update_with_read function above.
void MemAccessList_t::check_races_and_update_with_write(uint64_t inst_addr,
                              uint64_t addr, size_t mem_size,
                              om_node *curr_estrand, om_node *curr_hstrand) {

  DBG_TRACE(DEBUG_MEMORY, "check race w/ write addr %lx and size %lu.\n",
            addr, mem_size);
  om_assert( addr >= start_addr && 
                       (addr+mem_size) <= (start_addr+MAX_GRAIN_SIZE) );

  int start, end;
  MemAccess_t *writer = NULL;
  const enum GrainType_t gtype = get_mem_index(addr, mem_size, start, end);
  enum GrainType_t min_gtype = gtype;
  om_assert(start >= 0 && start < end && end <= MAX_GRAIN_SIZE);

  if( writer_gtype == UNINIT ) {
    //    pthread_spin_lock(&writer_lock);
    writer_gtype = gtype;
    //    pthread_spin_unlock(&writer_lock);
  } else if( writer_gtype > gtype ) {
    //    pthread_spin_lock(&writer_lock);
    break_list_into_smaller_gtype(writers, &writer_gtype, gtype);
    //    pthread_spin_unlock(&writer_lock);
  }
  // now writer_gtype = min{ old writer_gtype, gtype };
  om_assert(writer_gtype <= gtype); 
  om_assert(IS_ALIGNED_WITH_GTYPE(end, writer_gtype)); 

  MemAccess_t *new_writer = new MemAccess_t(curr_estrand, curr_hstrand, inst_addr);

  // now traverse through the writers list to both report race and update
  for(int i = start; i < end; i += gtype_to_mem_size[writer_gtype]) {
    writer = writers[i];
    if(writer == NULL) {
      //      pthread_spin_lock(&writer_lock);
      new_writer->inc_ref_count();
      writers[i] = new_writer;
      //      pthread_spin_unlock(&writer_lock);
    } else { // last writer exists; possibly report race and replace it
      if( writer->races_with(curr_estrand, curr_hstrand) ) { 
        // report race
        report_race(writer->rip, inst_addr, start_addr+i, WW_RACE);
      }
      //      pthread_spin_lock(&writer_lock);
      // replace the last writer regardless
      if(writer->dec_ref_count() == 0) {
        delete writer;
      }
      // note that ref count is decremented regardless
      new_writer->inc_ref_count();
      writers[i] = new_writer;
      //      pthread_spin_unlock(&writer_lock);
    }
  }
  if(new_writer->ref_count == 0) {
    delete new_writer;
  }

  // Now we detect races with the lreaders
  MemAccess_t *reader = NULL;
  if( lreader_gtype == UNINIT ) {
    goto check_rreaders; // we are done with lreaders
  } else if( lreader_gtype < gtype ) {
    min_gtype = lreader_gtype;
  } else if(lreader_gtype > gtype) {
    reader = lreaders[ get_prev_aligned_index(start, lreader_gtype) ];
  }
  om_assert(IS_ALIGNED_WITH_GTYPE(end, min_gtype)); 

  // traverse through the readers list to report race, using the 
  // min granularity between the lreader_gtype and gtype
  for(int i = start; i < end; i += gtype_to_mem_size[min_gtype]) {
    if( IS_ALIGNED_WITH_GTYPE(i, lreader_gtype) ) {
      reader = lreaders[i];
    }
    if( reader && reader->races_with(curr_estrand, curr_hstrand) ) {
      report_race(reader->rip, inst_addr, start_addr+i, RW_RACE);
    }
  }

check_rreaders:
  // Now we detect races with the lreaders
  if( rreader_gtype == UNINIT ) {
    return; // we are done with lreaders
  } else if( rreader_gtype < gtype ) {
    min_gtype = rreader_gtype;
  } else if(rreader_gtype > gtype) {
    reader = rreaders[ get_prev_aligned_index(start, rreader_gtype) ];
  }
  om_assert(IS_ALIGNED_WITH_GTYPE(end, min_gtype)); 

  // traverse through the readers list to report race, using the 
  // min granularity between the rreader_gtype and gtype
  for(int i = start; i < end; i += gtype_to_mem_size[min_gtype]) {
    if( IS_ALIGNED_WITH_GTYPE(i, rreader_gtype) ) {
      reader = rreaders[i];
    }
    if( reader && reader->races_with(curr_estrand, curr_hstrand) ) {
      report_race(reader->rip, inst_addr, start_addr+i, RW_RACE);
    }
  }
}

MemAccessList_t::MemAccessList_t(uint64_t addr, bool is_read, 
                                 MemAccess_t *acc, size_t mem_size) 
  : start_addr( ALIGN_BY_PREV_MAX_GRAIN_SIZE(addr) ), 
    lreader_gtype(UNINIT), rreader_gtype(UNINIT), writer_gtype(UNINIT) {

  for(int i=0; i < MAX_GRAIN_SIZE; i++) {
    lreaders[i] = rreaders[i] = writers[i] = NULL;
  }

  int start, end;
  const enum GrainType_t gtype = get_mem_index(addr, mem_size, start, end);

  om_assert(start >= 0 && start < end && end <= MAX_GRAIN_SIZE);

  MemAccess_t **l;
  if(is_read) {
    lreader_gtype = rreader_gtype = gtype;
    for(int i=start; i < end; i += gtype_to_mem_size[gtype]) {
        acc->inc_ref_count();
        lreaders[i] = rreaders[i] = acc;
    }
  } else {
    writer_gtype = gtype;
    for(int i=start; i < end; i += gtype_to_mem_size[gtype]) {
        acc->inc_ref_count();
        writers[i] = acc;
    }
  }

  if(pthread_spin_init(&lreader_lock, PTHREAD_PROCESS_PRIVATE) ||  
     pthread_spin_init(&rreader_lock, PTHREAD_PROCESS_PRIVATE) ||
      pthread_spin_init(&writer_lock, PTHREAD_PROCESS_PRIVATE) ) {
    fprintf(stderr, 
            "spin_lock initialization for MemAccessList_t failed.  Exit.\n");
    exit(1);
  }
}

MemAccessList_t::~MemAccessList_t() {
  MemAccess_t *acc;
  if(lreader_gtype != UNINIT) {
    for(int i=0; i < MAX_GRAIN_SIZE; i+=gtype_to_mem_size[lreader_gtype]) {
      acc = lreaders[i]; 
      if(acc && acc->dec_ref_count() == 0) {
        delete acc;
        lreaders[i] = 0;
      }
    }
  }

  if(rreader_gtype != UNINIT) {
    for(int i=0; i < MAX_GRAIN_SIZE; i+=gtype_to_mem_size[rreader_gtype]) {
      acc = rreaders[i]; 
      if(acc && acc->dec_ref_count() == 0) {
        delete acc;
        rreaders[i] = 0;
      }
    }
  }

  if(writer_gtype != UNINIT) {
    for(int i=0; i < MAX_GRAIN_SIZE; i+=gtype_to_mem_size[writer_gtype]) {
      acc = writers[i]; 
      if(acc && acc->dec_ref_count() == 0) {
        delete acc;
        writers[i] = 0;
      }
    }
  }
  pthread_spin_destroy(&lreader_lock);
  pthread_spin_destroy(&rreader_lock);
  pthread_spin_destroy(&writer_lock);
}
