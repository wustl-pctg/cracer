#ifndef RD_AND_OM_H
#define RD_AND_OM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENGLISH_ID 10
#define HEBREW_ID 11

struct RD_Memory_Struct_s;

typedef struct RD_Memory_Struct_s RD_Memory_Struct;

/*! FWD Declarations for all our functions */

void * RD_structure_create(CilkWorkerState * const ws, size_t size);

void RD_free(CilkWorkerState * const ws, void * mem);

void Race_detect_write(CilkWorkerState * const ws, void * memPtr, const void * writeValue, const char* func_name, const int line_num);

void * Race_detect_read(CilkWorkerState * const ws, void * memPtr, const char* func_name, const int line_num);

//! Macro for initializing RD_Memory_Struct by specified type
#define RD_INIT(type, sizefactor) RD_structure_create(_cilk_ws, sizefactor*sizeof(type))

//! Macro for reading relevant RD memory holder's data and for detecting races
#define READ(mem, type) *(type*)Race_detect_read(_cilk_ws, mem, __FUNCTION__, __LINE__)

//! Macro for writing to relevant RD memory holder's data and for detecting races
#define WRITE(mem, mem_to_copy) Race_detect_write(_cilk_ws, mem, mem_to_copy, __FUNCTION__, __LINE__)

//! ************ For boolean checking *****************
#define READ_b(mem, type, result) *(type*)Race_detect_read_b(_cilk_ws, mem, __FUNCTION__, __LINE__, result)

#define WRITE_b(mem, mem_to_copy, result) Race_detect_write_b(_cilk_ws, mem, mem_to_copy, __FUNCTION__, __LINE__, result)

void Race_detect_write_b(CilkWorkerState * const ws, void * memPtr, const void * writeValue, const char* func_name, const int line_num, int *);

void * Race_detect_read_b(CilkWorkerState * const ws, void * memPtr, const char* func_name, const int line_num, int *);
















#endif /* RD_AND_OM_H */
