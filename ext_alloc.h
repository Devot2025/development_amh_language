#ifndef _EXT_ALLOC_H_
#define _EXT_ALLOC_H_
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define s_free(ptr) (free(ptr), ptr = NULL) 
#define smart_realloc(type, block, size) (type *)realloc(block, sizeof(type) * size)

#define smart_malloc(type, size) (type * )malloc(sizeof(type) * size)
#define smart_calloc(type, size) (type * )calloc(size, sizeof(type))
bool safety_realloc(void** block, uint32_t now_index, uint32_t* now_size, uint32_t type_size);
bool clamp_realloc(void** block, uint32_t clamp_cap, uint32_t* now_cap, uint32_t type_size);

void* dupalloc(void* src_block, uint32_t size);

#define smart_dupalloc(type, block) dupalloc(block, sizeof(type))
#endif // !_EXT_ALLOC_H_
