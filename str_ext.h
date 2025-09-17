#ifndef _STR_EXT_H_
#define _STR_EXT_H_
#define str_ext_si_strap static inline

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
typedef struct Str_Buffer_Array {
	char* str_buff;
	uint32_t str_size;
	uint32_t str_index;
}Str_Buffer_Array;

Str_Buffer_Array* gen_str_buffer_array(uint32_t ini_size);
Str_Buffer_Array* move_str_buffer_s(Str_Buffer_Array* src_str_buffer, uint32_t src_size);
void delete_str_buffer_array(Str_Buffer_Array** src_str_buffer);
void append_str_buff(Str_Buffer_Array* src_str, const char src_byte);
void str_buffer_empty(Str_Buffer_Array* src_str_buffer);
void str_buffer_back_remove(Str_Buffer_Array* src_str_buffer);
char* str_dup_ext(const char* src_str);


str_ext_si_strap void insert_str_buff(Str_Buffer_Array* src_str, const char src_byte) {
	(*(src_str->str_buff + src_str->str_index)) = src_byte;
}





str_ext_si_strap bool check_to_digit_8byte(char src_byte) {
	return  '0' <= src_byte && src_byte <= '9';
}
str_ext_si_strap bool simple_strcmp(const char* src_1, const char* src_2) {
	return strcmp(src_1, src_2) == 0;
}

bool byte_match_to_list_8(const char src_byte, const char* src_byte_list);

bool str_match_to_str_list(const char* src_str, const char** src_str_list, uintptr_t list_len);
#endif // !_STR_EXT_H_
