#ifndef _LOAD_FILE_DATA_H_
#define _LOAD_FILE_DATA_H_
#include <stdio.h>
#include "ext_alloc.h"
#define temp_print_e(...) fprintf(stderr, __VA_ARGS__)
#define load_file_data_si_strap static inline
load_file_data_si_strap size_t get_file_all_size(FILE* src_f) {
	fseek(src_f, 0, SEEK_END);
	size_t f_size = ftell(src_f);
	rewind(src_f);
	return f_size;
}
load_file_data_si_strap char* read_file_bin_data(const char* file_path) {
	if (!file_path) {
		temp_print_e("error : file path is null!\n");

		return NULL;
	}
	FILE* __f = fopen(file_path, "rb");
	if (!__f) {
		temp_print_e("error : failed to open %s file!\n", file_path);
		return NULL;
	}
	size_t file_size = get_file_all_size(__f);
	char* dst_file_buff = smart_calloc(char, file_size + 1);/*'\0'*/

	if (!dst_file_buff) {
		fclose(__f);
		return NULL;
	}
	size_t confiem_f_size = fread(dst_file_buff, 1, file_size, __f);
	if (confiem_f_size != file_size) {
		fclose(__f);
		free(dst_file_buff);
		return NULL;
	}
	dst_file_buff[confiem_f_size] = '\0';
	return dst_file_buff;
}
#endif // !_LOAD_FILE_DATA_H_
