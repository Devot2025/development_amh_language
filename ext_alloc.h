#ifndef _EXT_ALLOC_H_
#define _EXT_ALLOC_H_
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define GET_ARRAY_SIZE(arr) sizeof(arr) / sizeof(arr[0])

#define s_free(ptr) (free(ptr), ptr = NULL) 
#define smart_realloc(type, block, size) (type *)realloc(block, sizeof(type) * size)

#define smart_malloc(type, size) (type * )malloc(sizeof(type) * size)
#define smart_calloc(type, size) size ? (type * )calloc(size, sizeof(type)) : NULL
bool safety_realloc(void** block, uint32_t now_index, uint32_t* now_size, uint32_t type_size);
bool clamp_realloc(void** block, uint32_t clamp_cap, uint32_t* now_cap, uint32_t type_size);

void* dupalloc(const void* src_block, size_t size);

#define smart_dupalloc(type, block) dupalloc(block, sizeof(type))
#endif // !_EXT_ALLOC_H_
