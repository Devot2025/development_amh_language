#ifndef _STR_EXT_H_
#define _STR_EXT_H_
#define str_ext_si_strap static inline

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ext_alloc.h"

#define str_buffer_array_is_strap static inline

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
str_ext_si_strap char* marge_str_and_value_to_str(const char * src_str, const void * src_value_mem, int src_type) {
	if (!src_str || !src_value_mem)return NULL;
	if (src_type == 0x0) {
		/*char*/
		char char_mem = *(char*)src_value_mem;
		int dst_str_len =snprintf(NULL, 0, "%s%c", src_str, char_mem);
		char* dst_str = smart_malloc(char, dst_str_len + 1);
		snprintf(dst_str, dst_str_len + 1, "%s%c", src_str, char_mem);
		return dst_str;
	}
	else if (src_type == 0x1) {

		int int_mem = *(int*)src_value_mem;
		int dst_str_len = snprintf(NULL, 0, "%s%d", src_str, int_mem);
		char* dst_str = smart_malloc(char, dst_str_len + 1);
		snprintf(dst_str, dst_str_len + 1, "%s%d", src_str, int_mem);

		return dst_str;
	}
	else if (src_type == 0x2) {

		float float_mem = *(float*)src_value_mem;
		int dst_str_len = snprintf(NULL, 0, "%s%f", src_str, float_mem);
		char* dst_str = smart_malloc(char, dst_str_len + 1);
		snprintf(dst_str, dst_str_len + 1, "%s%f", src_str, float_mem);
		return dst_str;
	}

	else if (src_type == 0x3) {

		double double_mem = *(double*)src_value_mem;
		int dst_str_len = snprintf(NULL, 0, "%s%lf", src_str, double_mem);
		char* dst_str = smart_malloc(char, dst_str_len + 1);
		snprintf(dst_str, dst_str_len + 1, "%s%lf", src_str, double_mem);
		return dst_str;
	}

	else if (src_type == 0x4) {

		char * str_mem = (char*)src_value_mem;
		int dst_str_len = snprintf(NULL, 0, "%s%s", src_str, str_mem);
		char* dst_str = smart_malloc(char, dst_str_len + 1);
		snprintf(dst_str, dst_str_len + 1, "%s%s", src_str, str_mem);
		return dst_str;
	}
	return NULL;
}

str_ext_si_strap char* marge_value_and_str_to_str(const char* src_str, const void* src_value_mem, int src_type) {
	if (!src_str || !src_value_mem)return NULL;
	if (src_type == 0x0) {
		/*char*/
		char char_mem = *(char*)src_value_mem;
		int dst_str_len = snprintf(NULL, 0, "%c%s", char_mem, src_str);
		char* dst_str = smart_malloc(char, dst_str_len + 1);
		snprintf(dst_str, dst_str_len + 1, "%c%s", char_mem, src_str);
		return dst_str;
	}
	else if (src_type == 0x1) {

		int int_mem = *(int*)src_value_mem;
		int dst_str_len = snprintf(NULL, 0, "%d%s", int_mem, src_str);
		char* dst_str = smart_malloc(char, dst_str_len + 1);
		snprintf(dst_str, dst_str_len + 1, "%d%s", int_mem, src_str);
		return dst_str;
	}
	else if (src_type == 0x2) {

		float float_mem = *(float*)src_value_mem;
		int dst_str_len = snprintf(NULL, 0, "%f%s", float_mem, src_str);
		char* dst_str = smart_malloc(char, dst_str_len + 1);
		snprintf(dst_str, dst_str_len + 1, "%f%s", float_mem, src_str);
		return dst_str;
	}

	else if (src_type == 0x3) {

		double double_mem = *(double*)src_value_mem;
		int dst_str_len = snprintf(NULL, 0, "%lf%s", double_mem, src_str);
		char* dst_str = smart_malloc(char, dst_str_len + 1);
		snprintf(dst_str, dst_str_len + 1, "%lf%s", double_mem, src_str);
		return dst_str;
	}

	else if (src_type == 0x4) {

		char* str_mem = (char*)src_value_mem;
		int dst_str_len = snprintf(NULL, 0, "%s%s", src_str, str_mem);
		char* dst_str = smart_malloc(char, dst_str_len + 1);
		snprintf(dst_str, dst_str_len + 1, "%s%s", src_str, str_mem);
		return dst_str;
	}
	return NULL;
}
str_ext_si_strap char* int_to_str_change(int src_int_value) {
	int require_size = snprintf(NULL, 0, "%d", src_int_value);
	char* dst_str_buffer = smart_malloc(char, (size_t)require_size);
	if (!dst_str_buffer)return NULL;
	snprintf(dst_str_buffer, (size_t)require_size, "%d", src_int_value);
	return dst_str_buffer;
}
str_buffer_array_is_strap char get_str_buff_char(const Str_Buffer_Array* src_str_buffer, uint32_t src_index) {
	return src_index > src_str_buffer->str_index ? '\0' : (*(src_str_buffer->str_buff + src_index));

}
str_buffer_array_is_strap char get_str_buff_beg_char(const Str_Buffer_Array* src_str_buffer) {
	return get_str_buff_char(src_str_buffer, 0);
}
str_buffer_array_is_strap size_t simple_strlen(const char* src_str) { return src_str ? strlen(src_str) : 0; }
str_ext_si_strap char* simple_strdup(const char* src_str) {
	if (!src_str)return NULL;
	size_t src_str_len = strlen(src_str);
	return dupalloc(src_str, src_str_len + 1);
}

str_ext_si_strap void insert_str_buff(Str_Buffer_Array* src_str, const char src_byte) {
	(*(src_str->str_buff + src_str->str_index)) = src_byte;
}

str_ext_si_strap size_t ext_strlen_add_null(const char* src_str) {
	return src_str ? strlen(src_str) + 1 : 0;
}

str_ext_si_strap bool check_to_digit_8byte(char src_byte) {
	return  '0' <= src_byte && src_byte <= '9';
}
str_ext_si_strap bool check_to_alphabet_8byte(char src_byte) {
	return ('a' <= src_byte && src_byte <= 'z') || ('A' <= src_byte && src_byte <= 'Z') || src_byte == '_';
}
str_ext_si_strap bool simple_strcmp(const char* src_1, const char* src_2) {
	return strcmp(src_1, src_2) == 0;
}
bool byte_match_to_list_8(const char src_byte, const char* src_byte_list);

bool str_match_to_str_list(const char* src_str, const char** src_str_list, uintptr_t list_len);
#endif // !_STR_EXT_H_
