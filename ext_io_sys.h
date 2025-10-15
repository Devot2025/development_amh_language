#ifndef _EXT_IO_SYS_H_
#define _EXT_IO_SYS_H_
#include "str_ext.h"
char* smart_fsystem_gets(FILE* fp) {
	Str_Buffer_Array* dst_str_array = gen_str_buffer_array(100);
	if (!dst_str_array)return NULL;
	int now_char = 0x0;
	while ((now_char = fgetc(fp)) != EOF) {
		if (now_char == 0xa)break;
		append_str_buff(dst_str_array, (char)now_char);
	}
	char* dst_buffer = dst_str_array->str_buff;
	free(dst_str_array);
	return dst_buffer;
}
#endif // !_EXT_IO_SYS_H_
