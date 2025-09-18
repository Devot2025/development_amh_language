/*
*amherstia language lexser prottype
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lexer_main_amh.h"
#define temp_print_e(...) fprintf(stderr, __VA_ARGS__)

char * stack_scan_line() {
	/*now develope */
	/*
	char* dst_buffer = smart_calloc(char, UCHAR_MAX);
	while (scanf("%256s", dst_buffer)) {

	}
	*/
	return NULL;
}
size_t get_file_all_size(FILE * src_f) {
	fseek(src_f, 0, SEEK_END);
	size_t f_size = ftell(src_f);
	rewind(src_f);
	return f_size;
}
char* start_read_amh_file_code(const char * amh_file_path){
	if (!amh_file_path) {
		temp_print_e("error : file path is null!\n");

		return NULL;
	}
	FILE* amh_f = fopen(amh_file_path, "rb");
	if (!amh_f) {
		temp_print_e("error : failed to open %s file!\n", amh_file_path);
		return NULL;
	}
	size_t amh_f_size = get_file_all_size(amh_f);
	char* amh_buff = smart_calloc(char, amh_f_size + 1);/*'\0'*/

	if (!amh_buff) {
		fclose(amh_f);
		return NULL;
	}
	size_t confiem_amh_f_size = fread(amh_buff, 1, amh_f_size, amh_f);
	if (confiem_amh_f_size != amh_f_size) {
		fclose(amh_f);
		free(amh_buff);
		return NULL; 
	}

	char* dst_amh_code = smart_calloc(char, 1);
	if (!dst_amh_code) {
		fclose(amh_f);
		free(amh_buff);
		return NULL;
	}
	/*sucess code*/
	dst_amh_code = amh_buff;
	dst_amh_code[amh_f_size] = '\0';
	return dst_amh_code;
}
Amh_Lex_Token_List* gen_amh_token_list(uint32_t src_ini_size) {

	Amh_Lex_Token_List* dst_amh_tokens = smart_calloc(Amh_Lex_Token_List, 1);
	if (!dst_amh_tokens)return NULL;
	dst_amh_tokens->stack_token_list = smart_calloc(Amh_Lex_Token, src_ini_size);
	if (!dst_amh_tokens->stack_token_list) {
		free(dst_amh_tokens);
		return NULL;
	}
	dst_amh_tokens->stack_token_list_size = src_ini_size;
	return dst_amh_tokens;
}
void extend_amh_token_list_size(Amh_Lex_Token_List* dst_amh_token_list, uintptr_t now_index) {
	
	if (dst_amh_token_list->stack_token_list_size < now_index)return;

	Amh_Lex_Token * dst_base_token_list = smart_realloc(Amh_Lex_Token, dst_amh_token_list->stack_token_list, dst_amh_token_list->stack_token_list_size * 2);
	if (!dst_base_token_list)return;
	dst_amh_token_list->stack_token_list = dst_base_token_list;
	dst_amh_token_list->stack_token_list_size = dst_amh_token_list->stack_token_list_size * 2;
}
void delete_amh_token_list(Amh_Lex_Token_List** src_amh_token_list) {
	if (!src_amh_token_list) return;         

	if (!*src_amh_token_list) return;               
	if (!(*src_amh_token_list)->stack_token_list) { 
		free(*src_amh_token_list);
		*src_amh_token_list = NULL;                 
		return;
	}

	Amh_Lex_Token_List* tmp_amh_token_list = *src_amh_token_list;

	for (uint32_t i = 0; i < tmp_amh_token_list->stack_token_list_size; ++i) {
		Amh_Lex_Token* amh_token = tmp_amh_token_list->stack_token_list + i;

		free(amh_token->amh_token);
		amh_token->amh_token = NULL;
	}

	free(tmp_amh_token_list->stack_token_list);
	free(tmp_amh_token_list);
	*src_amh_token_list = NULL;
}

#define AMH_TOKEN_KEY_WORD_NUM 100
bool check_to_token_keyword(const char* src_token) {
	const char* keyword_token_list[] = {
		"loop", "if", "else", "a_break", "break", "pub", "pri", "pro",
		"int", "float", "double", "char", "string", "virtuals", "none", "array", "return", "albtype", "switch", NULL
	};

	for (const char** tmp_keyword_token_list = &keyword_token_list[0]; *tmp_keyword_token_list; ++tmp_keyword_token_list) {
		if (simple_strcmp(src_token, *tmp_keyword_token_list))return true;
	}
	return false;
}

Amh_Lex_Token_Type ckeck_str_of_value(const char* src_str) {
	if (!src_str)return E_Amh_Lex_Token_Type_NULL;

	bool float_check = false;
	
	if (!check_to_digit_8byte(*src_str)) {
		/*check to begin iden.*/

		if (*src_str != '.')return E_Amh_Lex_Token_Type_Iden;
		/*if this find to begin dot. the str decision float literal and check to true float.*/
		float_check = true;
	}
	for (unsigned int i = 0; *(src_str + i); ++i) {
		if (*(src_str + i) == '.') {
			/*find to dot in binary.*/
			/**/
			if (float_check)return E_Amh_Lex_Token_Type_NULL;
			float_check = true;
		}
		else if (!check_to_digit_8byte(*(src_str + i)))return E_Amh_Lex_Token_Type_NULL;

	}
	return E_Amh_Lex_Token_Type_Bin_Literal;
}
Amh_Lex_Token_Type check_to_token_special_literal(const char* src_token) {
	const char* special_literal_str_token[10] = {
		"v_em", "none", NULL
	};
	for (const char** tmp_str_token = special_literal_str_token; *tmp_str_token; ++tmp_str_token)
		if (simple_strcmp(src_token, *tmp_str_token))return E_Amh_Lex_Token_Type_Specail_Literal;
	return E_Amh_Lex_Token_Type_NULL;
}

Amh_Lex_Token_Type decision_token_type_standard(const char * src_token) {
	/*auto base standard */	if (!src_token)return E_Amh_Lex_Token_Type_NULL;
	/*token str check to keyword iden and if true return keyword*/
	Amh_Lex_Token_Type amh_type;
	if (check_to_token_keyword(src_token))return E_Amh_Lex_Token_Type_Keyword;
	else {

		amh_type = check_to_token_special_literal(src_token);

		if (amh_type != E_Amh_Lex_Token_Type_NULL)
			return amh_type;
		amh_type = ckeck_str_of_value(src_token);
		if (amh_type != E_Amh_Lex_Token_Type_NULL)
			return amh_type;
	}
	/*var name or class name or func name...*/
	return E_Amh_Lex_Token_Type_Iden;
}
void append_lex_token_to_token_list(Amh_Lex_Token_List * src_amh_token_list, Str_Buffer_Array * src_stack_token, Amh_Lex_Token_Type src_lex_token_type) {
	if (src_stack_token->str_index == 0)return;
	if (!safety_realloc((void **) & src_amh_token_list->stack_token_list, src_amh_token_list->stack_token_list_index, &src_amh_token_list->stack_token_list_size, sizeof(Amh_Lex_Token))) return;

	(*(src_amh_token_list->stack_token_list + src_amh_token_list->stack_token_list_index)).amh_token = _strdup(src_stack_token->str_buff);
	if (src_lex_token_type == E_Amh_Lex_Token_Type_NULL) src_lex_token_type = decision_token_type_standard(src_stack_token->str_buff);
	(*(src_amh_token_list->stack_token_list + src_amh_token_list->stack_token_list_index)).amh_token_type = src_lex_token_type;
	
	/*reset*/
	str_buffer_empty(src_stack_token);
	src_amh_token_list->stack_token_list_index++;
}
bool amh_token_list_expect_str(Amh_Lex_Token_List * src_amh_token_list, const char* str) {
	/*range check*/
	if (src_amh_token_list->stack_token_list_index >= src_amh_token_list->stack_token_list_size) return false;
	Amh_Lex_Token* tmp_token = src_amh_token_list->stack_token_list + src_amh_token_list->stack_token_list_index;
	if (!tmp_token) return false;
	if (simple_strcmp(tmp_token->amh_token, str)) {
		tmp_token->amh_token = NULL;
		src_amh_token_list->stack_token_list_index++;
		return true;
	}
	return false;
}

void consume_advance_list(Amh_Lex_Token_List* src_amh_token_list) {
	Amh_Lex_Token* tmp_token = &(*(src_amh_token_list->stack_token_list + src_amh_token_list->stack_token_list_index));
	s_free(tmp_token->amh_token);
	tmp_token->amh_token = NULL;
	tmp_token->amh_token_type = 0;
	src_amh_token_list->stack_token_list_index++;
}

void amh_token_list_call(Amh_Lex_Token_List* src_amh_token_list) {
	const char* amh_token_type_str[] = {
		"ope", "punch", "bin literal", "spe literal", "s str", "d str", "standard", "keyword", "iden", "hashpro", "null"
	};
	if (!src_amh_token_list->stack_token_list)return;
	for (src_amh_token_list->stack_token_list_index = 0; src_amh_token_list->stack_token_list_index < src_amh_token_list->stack_token_list_size; src_amh_token_list->stack_token_list_index++) {
		printf("[idx %10lu|type %10s|token %10s]\n", src_amh_token_list->stack_token_list_index,
			amh_token_type_str[(*(src_amh_token_list->stack_token_list + src_amh_token_list->stack_token_list_index)).amh_token_type],
			(*(src_amh_token_list->stack_token_list + src_amh_token_list->stack_token_list_index)).amh_token);
	}
	src_amh_token_list->stack_token_list_index = 0;
}
const char* get_comsume_amh_token_str(Amh_Lex_Token_List* src_amh_token_list) {

	Amh_Lex_Token* tmp_token = src_amh_token_list->stack_token_list + src_amh_token_list->stack_token_list_index;
	const char* dst_token = str_dup_ext(tmp_token->amh_token);
	s_free(tmp_token->amh_token);
	tmp_token->amh_token_type = 0;
	src_amh_token_list->stack_token_list_index++;
	return dst_token;
}

Amh_Abstract_Host* gen_abstract_host(const char* first_abstract_host_name, uint32_t src_host_num) {
	if (!first_abstract_host_name) return NULL;
	if (src_host_num == 0) src_host_num = INIT_HOST_NUM;
	Amh_Abstract_Host* dst_amh_abstract_host = smart_calloc(Amh_Abstract_Host, 1);

	if (!dst_amh_abstract_host)return NULL;

	dst_amh_abstract_host->host_name = smart_calloc(char*, src_host_num);

	if (!dst_amh_abstract_host->host_name) {
		free(dst_amh_abstract_host);
		return NULL;
	}
	dst_amh_abstract_host->host_size = src_host_num;
	dst_amh_abstract_host->host_name[dst_amh_abstract_host->now_host_index] = _strdup(first_abstract_host_name);
	if (!dst_amh_abstract_host->host_name[dst_amh_abstract_host->now_host_index]) {
		free(dst_amh_abstract_host->host_name);
		free(dst_amh_abstract_host);
		return NULL;
	}
	return dst_amh_abstract_host;
}
Amh_Lex_Token_List* start_lex_amh_code(const char* amh_file_name, const char* src_amh_code) {

	Amh_Lex_Mode now_lex_mode = E_Amh_Lex_Hash_Process_Space;
	Amh_Lex_Token_List* amh_token = gen_amh_token_list(INIT_AMH_TOKEN_LIST_SIZE);
	Str_Buffer_Array* stack_token = gen_str_buffer_array(100);

	if (!stack_token || !amh_token) return NULL;
	while (*src_amh_code) {

		char tmp_code_byte = *src_amh_code;
		/*check and realloc*/
		if (now_lex_mode == E_Amh_Lex_Normal) {
			/*normal read mode*/

			if (byte_match_to_list_8(tmp_code_byte, ";(){}")) {
				append_lex_token_to_token_list(amh_token, stack_token, E_Amh_Lex_Token_Type_NULL);
				append_str_buff(stack_token, tmp_code_byte);
				append_lex_token_to_token_list(amh_token, stack_token, E_Amh_Lex_Token_Type_Punchcute);

			}
			else if (byte_match_to_list_8(tmp_code_byte, "+-*/=><:")) {
				append_lex_token_to_token_list(amh_token, stack_token, E_Amh_Lex_Token_Type_NULL);
				/*regist to befor token,stack now byte token,chage to operation lex mode.*/
				append_str_buff(stack_token, tmp_code_byte);
				now_lex_mode = E_Amh_Lex_Operation;
			}
			else if (tmp_code_byte == ',') {

				append_lex_token_to_token_list(amh_token, stack_token, E_Amh_Lex_Token_Type_NULL);
				append_str_buff(stack_token, tmp_code_byte);
				append_lex_token_to_token_list(amh_token, stack_token, E_Amh_Lex_Operation);
			}
			else if (tmp_code_byte == '.') {
				/*now not stack dot*/
				now_lex_mode = E_Amh_Lex_Dot;
			}
			else if (tmp_code_byte == '"') {
				append_lex_token_to_token_list(amh_token, stack_token, E_Amh_Lex_Token_Type_NULL);
				now_lex_mode = E_Amh_Lex_D_Str_Literal;
			}
			else if (tmp_code_byte == '`') {

			}
			else if (tmp_code_byte == '\'') {
				append_lex_token_to_token_list(amh_token, stack_token, E_Amh_Lex_Token_Type_NULL);
				now_lex_mode = E_Amh_Lex_S_Str_Literal;
			}
			else if (tmp_code_byte == '#') {
				now_lex_mode = E_Amh_Lex_Hash_Process;
			}
			else if (tmp_code_byte == '@') {
				/*token iden*/
				/*comment mode*/
				now_lex_mode = E_Amh_Lex_Comment_Start;
			}
			else if (tmp_code_byte == ' ') {
				append_lex_token_to_token_list(amh_token, stack_token, E_Amh_Lex_Token_Type_NULL);
			}
			else if (!byte_match_to_list_8(tmp_code_byte, "\r\b\n")) {
				append_str_buff(stack_token, tmp_code_byte);
			}

		}
		else if (now_lex_mode == E_Amh_Lex_Operation) {
			const char* two_byte_ope_list[] = {
				"<=",">=","==","*=","+=","-=","/=", "::", "++", "--", "**", "//", "+{", "-}"
			};
			append_str_buff(stack_token, tmp_code_byte);
			bool two_byte_ope_check = str_match_to_str_list(stack_token->str_buff, two_byte_ope_list, 14);

			/*
			* if not match two byte operator code...
			* one byte operator token regist to the list.
			* corrent token is analysis of next 	to expect normal mode
			* exmaple
			* begin stack '+'
			* now word ';'
			* tmp stack "+;"
			* not match -> normal mode run(';' not regist to stack regist)...
			*/
			now_lex_mode = E_Amh_Lex_Normal;
			Str_Buffer_Array* tmp_str_buff = NULL;
			tmp_str_buff = move_str_buffer_s(stack_token, two_byte_ope_check ? 2 : 1);

			if (!two_byte_ope_check) {

				append_lex_token_to_token_list(amh_token, tmp_str_buff, E_Amh_Lex_Token_Type_Operation);
				delete_str_buffer_array(&tmp_str_buff);

				continue;
			}
			else {
				append_lex_token_to_token_list(amh_token, tmp_str_buff, E_Amh_Lex_Token_Type_Operation);
				delete_str_buffer_array(&tmp_str_buff);
			}
		}
		else if (now_lex_mode == E_Amh_Lex_Dot) {
			/*seeking to dot next word(bin or alpha).*/

			if (!check_to_digit_8byte(tmp_code_byte)) {
				/*decision to none binary*/
				/*begin regist to parent iden.*/
				append_lex_token_to_token_list(amh_token, stack_token, E_Amh_Lex_Token_Type_NULL);
				/*dot append,ecircle*/
				append_str_buff(stack_token, '.');
				/*next regist to dot(operation)*/
				append_lex_token_to_token_list(amh_token, stack_token, E_Amh_Lex_Token_Type_Operation);
				/*now not regist to children iden.*/
			}
			/*if now word is binary. dot append.*/
			else append_str_buff(stack_token, '.');
			/*reset mode*/
			now_lex_mode = E_Amh_Lex_Normal;
			/*now word is re run to next process.*/
			continue;
		}
		else if (now_lex_mode == E_Amh_Lex_D_Str_Literal) {
			if (tmp_code_byte == '"') {
				append_lex_token_to_token_list(amh_token, stack_token, E_Amh_Lex_Token_Type_D_Str_Literal);
				/*re mode normal */
				now_lex_mode = E_Amh_Lex_Normal;
			}
			else if (tmp_code_byte == '\\') {

				now_lex_mode = E_Amh_Lex_D_Str_Doll;

			}
			else append_str_buff(stack_token, tmp_code_byte);

		}
		else if (now_lex_mode == E_Amh_Lex_D_Str_Doll) {
			if (tmp_code_byte == 'n') append_str_buff(stack_token, '\n');
			else if (tmp_code_byte == 'b') append_str_buff(stack_token, '\b');
			else if (tmp_code_byte == 'r') append_str_buff(stack_token, '\r');
			else if (tmp_code_byte == 't') append_str_buff(stack_token, '\t');
			else if (tmp_code_byte == '\\') append_str_buff(stack_token, '\\');
			else if (tmp_code_byte == '"') append_str_buff(stack_token, '\"');
			else append_str_buff(stack_token, tmp_code_byte);
			now_lex_mode = E_Amh_Lex_D_Str_Literal;
		}

		else if (now_lex_mode == E_Amh_Lex_Hash_Process) {
			if (tmp_code_byte == '\n')now_lex_mode = E_Amh_Lex_Normal;
		}
		else if (now_lex_mode == E_Amh_Lex_Comment_Start) {

			if (tmp_code_byte == '!') now_lex_mode = E_Amh_Lex_Comment_Call;
			else if (tmp_code_byte == ':') now_lex_mode = E_Amh_Lex_Comment_Line_Start;
			else if (tmp_code_byte == '\n') now_lex_mode = E_Amh_Lex_Normal;
			else  now_lex_mode = E_Amh_Lex_Comment;
		}
		else if (now_lex_mode == E_Amh_Lex_Comment) {
			if(tmp_code_byte == '\n')now_lex_mode = E_Amh_Lex_Normal;
		}
		else if (now_lex_mode == E_Amh_Lex_Comment_Call) {
			/*out to log*/
			if (tmp_code_byte == '\n') {
				now_lex_mode = E_Amh_Lex_Normal;
				printf("[log/com_call] %s\n", stack_token->str_buff);
				str_buffer_empty(stack_token);

			}
			else append_str_buff(stack_token, tmp_code_byte);
		}
		else if (now_lex_mode == E_Amh_Lex_Comment_Line_Start) {
			if (tmp_code_byte == ':') now_lex_mode = E_Amh_Lex_Comment_Line_End;
		}
		else if (now_lex_mode == E_Amh_Lex_Comment_Line_End) {
			if (tmp_code_byte == '@') now_lex_mode = E_Amh_Lex_Normal;
			else now_lex_mode = E_Amh_Lex_Comment_Line_Start;
		}
		else if (now_lex_mode ==E_Amh_Lex_Hash_Process_Space) {

			/*
			* file start ~  hashpro def(#) end
			*/
			if (tmp_code_byte == '#') {
				now_lex_mode = E_Amh_Lex_Hash_Process_Space_Analysis;
			}
			else {
				now_lex_mode = E_Amh_Lex_Normal;
				continue;
			}

		}
		else if (now_lex_mode == E_Amh_Lex_Hash_Process_Space_Analysis) {
			if (tmp_code_byte == '\n') {
				now_lex_mode = E_Amh_Lex_Normal;
				append_lex_token_to_token_list(amh_token ,stack_token, E_Amh_Lex_Token_Type_Hash_Process);
				str_buffer_empty(stack_token);

			}
			else if (!byte_match_to_list_8(tmp_code_byte, "\b\r\t ")) append_str_buff(stack_token, tmp_code_byte);

		}

		src_amh_code++;

	}


	delete_str_buffer_array(&stack_token);

	if (!amh_token->stack_token_list_index) {
		free(amh_token->stack_token_list);
		amh_token->stack_token_list = NULL;

	}
	else {
		Amh_Lex_Token* tmp_amh_lex_token = smart_realloc(Amh_Lex_Token, amh_token->stack_token_list, amh_token->stack_token_list_index);

		if (tmp_amh_lex_token) {

			amh_token->stack_token_list = tmp_amh_lex_token;
		}
	}
	amh_token->stack_token_list_size = amh_token->stack_token_list_index;

	return amh_token;
}

Amh_Lex_Token_List * start_read_amh_lex_main(const char * abstract_host, const char * amh_file_path) {
	char* amh_code = start_read_amh_file_code(amh_file_path);
	if (!amh_code)return NULL;
	Amh_Lex_Token_List* amh_token_list = start_lex_amh_code(amh_file_path, amh_code);
	amh_token_list->abstract_host = amh_file_path;
	free(amh_code);
	return amh_token_list;
}