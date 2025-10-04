#include "str_ext.h"

void delete_str_buffer_array(Str_Buffer_Array** src_str_buffer) {
	if (!src_str_buffer || !*src_str_buffer)return;
	free((*src_str_buffer)->str_buff);
	free(*src_str_buffer);
	*src_str_buffer = NULL;
}
void append_str_buff(Str_Buffer_Array* src_str, const char src_byte) {
	if (!safety_realloc((void**)&src_str->str_buff, src_str->str_index + 2, &src_str->str_size, sizeof(char))) return;

	insert_str_buff(src_str, src_byte);
	(*(src_str->str_buff + src_str->str_index + 1)) = '\0';
	src_str->str_index++;
}
Str_Buffer_Array* move_str_buffer_array(Str_Buffer_Array* src_str_buffer) {
	Str_Buffer_Array* new_str_buff = smart_dupalloc(Str_Buffer_Array, src_str_buffer);
	if (!new_str_buff)return NULL;
	/*reset src buff and index.*/
	char* rebuf = smart_calloc(char, src_str_buffer->str_size);
	/*if failed to allocated reset buf. free new str_buff and return NULL.*/
	if (!rebuf) { free(new_str_buff); return NULL; }
	src_str_buffer->str_buff = rebuf;
	src_str_buffer->str_index = 0;
	return new_str_buff;
}

Str_Buffer_Array* gen_str_buffer_array(uint32_t ini_size) {
	Str_Buffer_Array* base_str;
	base_str = smart_calloc(Str_Buffer_Array, 1);
	if (!base_str)return NULL;
	base_str->str_buff = smart_calloc(char, ini_size + 1);
	if (!base_str->str_buff) {
		free(base_str);
		return NULL;
	}
	base_str->str_size = ini_size + 1;
	return base_str;
}

void str_buffer_empty(Str_Buffer_Array* src_str_buffer) {
	memset(src_str_buffer->str_buff, 0, src_str_buffer->str_size * sizeof(char));
	src_str_buffer->str_index = 0;
}
void str_buffer_back_remove(Str_Buffer_Array* src_str_buffer) {
	src_str_buffer->str_buff[src_str_buffer->str_index] = '\0';
	if (src_str_buffer->str_index > 0)src_str_buffer->str_index--;
}
char* str_dup_ext(const char* src_str) {
	if (!src_str)return NULL;
	size_t len_s = strlen(src_str);
	char* dst_str = smart_calloc(char, len_s + 1);
	if (dst_str)memcpy(dst_str, src_str, len_s);
	return dst_str;
}
Str_Buffer_Array* move_str_buffer_s(Str_Buffer_Array* src_str_buffer, uint32_t src_size) {
	if (!src_str_buffer)return NULL;
	if (src_str_buffer->str_size < src_size + 1)return NULL;
	Str_Buffer_Array* new_str_buffer = smart_calloc(Str_Buffer_Array, 1);
	if (!new_str_buffer) {
		return NULL;
	}
	new_str_buffer->str_buff = dupalloc(src_str_buffer->str_buff, (sizeof(char) * src_size) + 1);

	if (!new_str_buffer->str_buff) {
		free(new_str_buffer);
		return NULL;
	}
	new_str_buffer->str_index = src_size;
	insert_str_buff(new_str_buffer, '\0');
	new_str_buffer->str_size = src_size + 1;

	str_buffer_empty(src_str_buffer);
	return new_str_buffer;
}
bool byte_match_to_list_8(const char src_byte, const char* src_byte_list) {
	while (*src_byte_list) {
		if (src_byte == *src_byte_list)return true;
		src_byte_list++;
	}
	return false;
}
bool str_match_to_str_list(const char* src_str, const char** src_str_list, uintptr_t list_len) {
	for (unsigned int i = 0; i < list_len; ++i) {
		if (simple_strcmp(src_str, *(src_str_list + i)))return true;
	}
	return false;
}