#include "amh_in_src.h"
char * std_in_toolchain_line() {
	char * dst_buff = smart_calloc(char, STANDARD_STR_BUFFER);
	if (!dst_buff)return NULL;
	/*getline*/
	int is_in = scanf("%100s", dst_buff);
	char next_in = getc(stdin);
	if (is_in < 0 || next_in == EOF || next_in == '\n') {
		s_free(dst_buff);
		return NULL;
	}
	ungetc(next_in, stdin);
	return dst_buff;
}
char* std_in_line() {
	char* dst_buff = smart_calloc(char, STANDARD_STR_BUFFER + 1);
	uintptr_t buff_len = STANDARD_STR_BUFFER + 1;
	uintptr_t buff_idx = 0;
	if (!dst_buff)return NULL;
	char* tmp_buff = NULL;
	while ((tmp_buff = std_in_toolchain_line())) {
		uintptr_t tmp_buff_len = strlen(tmp_buff);
		if (tmp_buff_len + buff_idx + 1 >= buff_len) {

			if (!safety_realloc(&dst_buff, buff_idx + tmp_buff_len, &buff_len, sizeof(char))) {
				free(tmp_buff);
				dst_buff[buff_idx] = '\0';
				return dst_buff;
			}
		}
		memcpy(dst_buff + buff_idx, tmp_buff, tmp_buff_len);

		buff_idx += tmp_buff_len;
		dst_buff[buff_idx] = '\0';

		free(tmp_buff);
	}
	clamp_realloc(&dst_buff, buff_idx + 1, &buff_len, sizeof(char));
	dst_buff[buff_idx] = '\0';

	return dst_buff;
}