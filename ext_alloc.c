
#include "ext_alloc.h"
#define BASE_ALLOC_SIZE 10
bool safety_realloc(void** block, uintptr_t now_index, uintptr_t* now_size, uintptr_t type_size) {
	if (now_index < *now_size)return true;
	uintptr_t block_capacity = *now_size ? (now_index + 1) * 2 : BASE_ALLOC_SIZE;
	void* virtual_block = realloc(*block, block_capacity * type_size);

	if (!virtual_block)return false;
	*now_size = block_capacity;

	*block = virtual_block;
	return true;
}
bool clamp_realloc(void** block, uintptr_t clamp_cap,uintptr_t *now_cap, uintptr_t type_size) {
	void* virtual_block = realloc(*block, clamp_cap * type_size);
	if (!virtual_block)return false;
	*block = virtual_block;
	*now_cap = clamp_cap;
	return true;
}
void* dupalloc(void* src_block, uintptr_t size) {
	void* new_block = malloc(size);
	if (!new_block) return new_block;
	memcpy(new_block, src_block, size);
	return new_block;
}