#include "lexer_main_ans.h"
Ans_Lex_Token_List* start_ans_lex_main(const char* ans_file_path) {
	char* ans_code = read_file_bin_data(ans_file_path);
	if (!ans_code)return NULL;
	return start_ans_lex_code(ans_code);
}
Ans_Lex_Token_List* start_ans_lex_code(const char* src_ans_code) {
	Lex_Ans_Mode now_lex_mode = E_Lex_Mode_Normal;
	Str_Buffer_Array* stack_token_word = gen_str_buffer_array(INI_BLOCK_SIZE);
	if (!stack_token_word) {
		return NULL;
	}
	Ans_Lex_Token_List* dst_lex_token_list = gen_amh_token_list(INI_BLOCK_SIZE);
	if (!dst_lex_token_list) {
		delete_str_buffer_array(&stack_token_word);
		return NULL;
	}
	while (*src_ans_code) {
		char tmp_now_ans_code = *src_ans_code;
		switch (now_lex_mode) {
		case E_Lex_Mode_Normal:
			ans_normal_token_process(stack_token_word, dst_lex_token_list, &now_lex_mode, tmp_now_ans_code);
			break;
		case E_Lex_Mode_Operator:
			if (!ans_operator_token_process(stack_token_word, dst_lex_token_list, &now_lex_mode, tmp_now_ans_code))continue;
			break;
		case E_Lex_Mode_Dot:
			ans_dot_token_process(stack_token_word, dst_lex_token_list, &now_lex_mode, tmp_now_ans_code);
			continue;
			break;
		case E_Lex_Mode_D_Str:
			ans_d_str_token_process(stack_token_word, dst_lex_token_list, &now_lex_mode, tmp_now_ans_code);
			break;
		case E_Lex_Mode_D_Str_Next:
			if (ans_d_str_next_process(stack_token_word, dst_lex_token_list, &now_lex_mode, tmp_now_ans_code))continue;
			break;
		case E_Lex_Mode_D_Str_Doll:
			ans_d_str_doll_process(stack_token_word, dst_lex_token_list, &now_lex_mode, tmp_now_ans_code);
			break;
		case E_Lex_Mode_Comment_Start:
			ans_comment_start_process(&now_lex_mode, tmp_now_ans_code);
			break;
		case E_Lex_Mode_Comment_Line:
			ans_comment_line_process(&now_lex_mode, tmp_now_ans_code);
			break;
		case E_Lex_Mode_Comment_Multi_Line_Start:
			ans_comment_mutli_line_process(&now_lex_mode, tmp_now_ans_code);
			break;
		case E_Lex_Mode_Comment_Call:
			ans_comment_call_process(stack_token_word, &now_lex_mode, tmp_now_ans_code);
			break;

		case E_Lex_Mode_Comment_Multi_Line_End:
			ans_comment_multi_line_end_process(&now_lex_mode, tmp_now_ans_code);
			break;
		case E_Lex_Mode_Hash_Process:
			ans_hash_process(stack_token_word, dst_lex_token_list, &now_lex_mode, tmp_now_ans_code);
			break;
		default:
			printf("%u", now_lex_mode);
			assert(false);
			break;
		}
		src_ans_code++;
	}

	if (stack_token_word->str_index > 0) {
		append_lex_token_to_token_list(dst_lex_token_list, stack_token_word, E_Ans_Lex_Token_Type_Error_);
	}

	delete_str_buffer_array(&stack_token_word);

	if (!dst_lex_token_list->stack_token_list_index) {
		free(dst_lex_token_list->stack_tokens);
		dst_lex_token_list->stack_tokens = NULL;
	}
	else {
		Ans_Lex_Token* tmp_ans_lex_token = smart_realloc(Ans_Lex_Token, dst_lex_token_list->stack_tokens, dst_lex_token_list->stack_token_list_index);

		if (tmp_ans_lex_token) dst_lex_token_list->stack_tokens = tmp_ans_lex_token;
	}
	dst_lex_token_list->stack_token_list_size = dst_lex_token_list->stack_token_list_index;
	return dst_lex_token_list;
}
void ans_normal_token_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code) {
	if (byte_match_to_list_8(now_byte_code, "{}();")) {
		append_lex_token_to_token_list(src_lex_token, src_stack_token, E_Ans_Lex_Token_Type_NULL);
		append_str_buff(src_stack_token, now_byte_code);
		append_lex_token_to_token_list(src_lex_token, src_stack_token, E_Ans_Lex_Token_Type_Punchcute);
	}

	else if (byte_match_to_list_8(now_byte_code, "=+-*/%!<>&|~^:")) {
		append_lex_token_to_token_list(src_lex_token, src_stack_token, E_Ans_Lex_Token_Type_NULL);
		append_str_buff(src_stack_token, now_byte_code);
		*src_lex_mode = E_Lex_Mode_Operator;
	}
	else if (now_byte_code == ',') {
		append_lex_token_to_token_list(src_lex_token, src_stack_token, E_Ans_Lex_Token_Type_NULL);
		append_str_buff(src_stack_token, now_byte_code);
		append_lex_token_to_token_list(src_lex_token, src_stack_token, E_Ans_Lex_Token_Type_Operator);
	}
	else if (now_byte_code == '\"') {
		append_lex_token_to_token_list(src_lex_token, src_stack_token, E_Ans_Lex_Token_Type_NULL);
		*src_lex_mode = E_Lex_Mode_D_Str;
	}
	else if (now_byte_code == '.') {
		*src_lex_mode = E_Lex_Mode_Dot;
	}
	else if (now_byte_code == '@') {
		/*token iden*/
		/*comment mode*/
		*src_lex_mode = E_Lex_Mode_Comment_Start;
	}
	else if (now_byte_code == '#') {
		*src_lex_mode = E_Lex_Mode_Hash_Process;
	}
	else if (byte_match_to_list_8(now_byte_code, " \n\r\b	")) {
		append_lex_token_to_token_list(src_lex_token, src_stack_token, E_Ans_Lex_Token_Type_NULL);
	}
	else {
		append_str_buff(src_stack_token, now_byte_code);
	}
}
bool ans_operator_token_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code) {
	const char* two_byte_ope_list[] = {
		"==", "!=", "+=", "-=", "*=","/=", "%=", "<=", ">=", "^=", "~=", "|=", "&=", "::", "++", "--", "<<", ">>"
	};
	append_str_buff(src_stack_token, now_byte_code);

	bool two_byte_ope_check = str_match_to_str_list(src_stack_token->str_buff, two_byte_ope_list, GET_ARRAY_SIZE(two_byte_ope_list));

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
	*src_lex_mode = E_Lex_Mode_Normal;
	Str_Buffer_Array* tmp_str_buff = NULL;
	tmp_str_buff = move_str_buffer_s(src_stack_token, two_byte_ope_check ? 2 : 1);
	append_lex_token_to_token_list(src_lex_token, tmp_str_buff, E_Ans_Lex_Token_Type_Operator);
	delete_str_buffer_array(&tmp_str_buff);

	return two_byte_ope_check;
}
void ans_dot_token_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code) {
	/*seeking to dot next word(bin or alpha).*/

	if (check_to_alphabet_8byte(now_byte_code) || now_byte_code & 0x80) {
		/*decision to none binary*/
		/*begin regist to parent iden.*/
		append_lex_token_to_token_list(src_lex_token, src_stack_token, E_Ans_Lex_Token_Type_NULL);
		/*dot append,ecircle*/
		append_str_buff(src_stack_token, '.');
		/*next regist to dot(operation)*/
		append_lex_token_to_token_list(src_lex_token, src_stack_token, E_Ans_Lex_Token_Type_Operator);
		/*now not regist to children iden.*/
	}
	/*if now word is binary. dot append.*/
	else append_str_buff(src_stack_token, '.');
	/*reset mode*/
	*src_lex_mode = E_Lex_Mode_Normal;
	/*now word is re run to next process.*/
}
void ans_d_str_token_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code) {
	if (now_byte_code == '\"') *src_lex_mode = E_Lex_Mode_D_Str_Next;
	else if (now_byte_code == '\\') *src_lex_mode = E_Lex_Mode_D_Str_Doll;
	else if (now_byte_code == '\n') *src_lex_mode = E_Lex_Mode_Normal;
	else append_str_buff(src_stack_token, now_byte_code);
}
bool ans_d_str_next_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code) {
	if (now_byte_code == '\"') {
		*src_lex_mode = E_Lex_Mode_D_Str;
	}
	else if (now_byte_code != '\n') {
		if (src_stack_token->str_index == 0)append_str_buff(src_stack_token, '\0');
		append_lex_token_to_token_list(src_lex_token, src_stack_token, E_Ans_Lex_Token_Type_Str_Literal);
		*src_lex_mode = E_Lex_Mode_Normal;
		return true;
	}
	return false;
}

void ans_d_str_doll_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code) {
	if (now_byte_code == 'n') append_str_buff(src_stack_token, '\n');
	else if (now_byte_code == 'b') append_str_buff(src_stack_token, '\b');
	else if (now_byte_code == 'r') append_str_buff(src_stack_token, '\r');
	else if (now_byte_code == 't') append_str_buff(src_stack_token, '\t');
	else if (now_byte_code == '\\') append_str_buff(src_stack_token, '\\');
	else if (now_byte_code == '"') append_str_buff(src_stack_token, '\"');
	else append_str_buff(src_stack_token, now_byte_code);
	*src_lex_mode = E_Lex_Mode_D_Str;
}
void ans_comment_start_process(Lex_Ans_Mode* src_lex_mode, const char now_byte_code) {
	if (now_byte_code == '!') *src_lex_mode = E_Lex_Mode_Comment_Call;
	else if (now_byte_code == ':') *src_lex_mode = E_Lex_Mode_Comment_Multi_Line_Start;
	else if (now_byte_code == '\n') *src_lex_mode = E_Lex_Mode_Normal;
	else  *src_lex_mode = E_Lex_Mode_Comment_Line;
}
void ans_comment_line_process(Lex_Ans_Mode* src_lex_mode, const char now_byte_code) {
	if (now_byte_code == '\n') *src_lex_mode = E_Lex_Mode_Normal;
}
void ans_comment_mutli_line_process(Lex_Ans_Mode* src_lex_mode, const char now_byte_code) {
	if (now_byte_code == ':')*src_lex_mode = E_Lex_Mode_Comment_Multi_Line_End;
}
void ans_comment_multi_line_end_process(Lex_Ans_Mode* src_lex_mode, const char now_byte_code) {
	if (now_byte_code == '@')*src_lex_mode = E_Lex_Mode_Normal;
	else *src_lex_mode = E_Lex_Mode_Comment_Multi_Line_Start;
}
void ans_hash_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code) {
	if (now_byte_code == '\n') {
		append_lex_token_to_token_list(src_lex_token, src_stack_token, E_Ans_Lex_Token_Type_Hash_Process);
		*src_lex_mode = E_Lex_Mode_Normal;
	}
	else if (byte_match_to_list_8(now_byte_code, "\n\r\b	 ")) {
		append_lex_token_to_token_list(src_lex_token, src_stack_token, E_Ans_Lex_Token_Type_Hash_Process);
	}
	else append_str_buff(src_stack_token, now_byte_code);
}
void ans_comment_call_process(Str_Buffer_Array* src_stack_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code) {
	if (now_byte_code == '\n') {
		*src_lex_mode = E_Lex_Mode_Normal;
		printf("[log/com_call] %s\n", src_stack_token->str_buff);
		str_buffer_empty(src_stack_token);
	}
	else append_str_buff(src_stack_token, now_byte_code);
}
Ans_Lex_Token_List* gen_amh_token_list(uint32_t src_ini_size) {
	Ans_Lex_Token_List* dst_ans_token_list = smart_calloc(Ans_Lex_Token_List, 1);
	if (!dst_ans_token_list)return NULL;
	dst_ans_token_list->stack_tokens = smart_calloc(Ans_Lex_Token, src_ini_size);
	if (!dst_ans_token_list->stack_tokens) {
		free(dst_ans_token_list);
		return NULL;
	}
	dst_ans_token_list->stack_token_list_size = src_ini_size;
	return dst_ans_token_list;
}
void extend_amh_token_list_size(Ans_Lex_Token_List* src_ans_token_list, uint32_t now_index) {
	if (src_ans_token_list->stack_token_list_size < now_index)return;

	Ans_Lex_Token* tmp_token_list = smart_realloc(Ans_Lex_Token, src_ans_token_list->stack_tokens, src_ans_token_list->stack_token_list_size * 2);
	if (!tmp_token_list)return;
	src_ans_token_list->stack_tokens = tmp_token_list;
	src_ans_token_list->stack_token_list_size = src_ans_token_list->stack_token_list_size * 2;
}
void delete_ans_token_list(Ans_Lex_Token_List** src_ans_token_list) {
	if (!src_ans_token_list) return;

	if (!*src_ans_token_list) return;
	if (!(*src_ans_token_list)->stack_tokens) {
		free(*src_ans_token_list);
		*src_ans_token_list = NULL;
		return;
	}

	Ans_Lex_Token_List* tmp_ans_tokens = *src_ans_token_list;

	for (uint32_t i = 0; i < tmp_ans_tokens->stack_token_list_size; ++i) {
		Ans_Lex_Token* ans_token = tmp_ans_tokens->stack_tokens + i;

		free(ans_token->ans_token_str);
		ans_token->ans_token_str = NULL;
	}

	free(tmp_ans_tokens->stack_tokens);
	free(tmp_ans_tokens);
	*src_ans_token_list = NULL;
}

void append_lex_token_to_token_list(Ans_Lex_Token_List* src_ans_token_list, Str_Buffer_Array* src_stack_token, Ans_Lex_Token_Type src_lex_token_type) {
	if (src_stack_token->str_index == 0)return;
	if (!safety_realloc((void**)&src_ans_token_list->stack_tokens, src_ans_token_list->stack_token_list_index, &src_ans_token_list->stack_token_list_size, sizeof(Ans_Lex_Token))) return;

	(*(src_ans_token_list->stack_tokens + src_ans_token_list->stack_token_list_index)).ans_token_str = simple_strdup(src_stack_token->str_buff);

	if (src_lex_token_type == E_Ans_Lex_Token_Type_NULL) src_lex_token_type = decision_token_type_standard(src_stack_token->str_buff);
	(*(src_ans_token_list->stack_tokens + src_ans_token_list->stack_token_list_index)).ans_token_type = src_lex_token_type;

	/*reset*/
	str_buffer_empty(src_stack_token);
	src_ans_token_list->stack_token_list_index++;
}

Ans_Lex_Token_Type decision_token_type_standard(const char* src_token) {
	/*auto base standard */	if (!src_token)return E_Ans_Lex_Token_Type_NULL;
	/*token str check to keyword iden and if true return keyword*/
	Ans_Lex_Token_Type dst_amh_type;
	if (check_to_token_keyword(src_token))return E_Ans_Lex_Token_Type_Keyword;
	else {
		dst_amh_type = check_to_token_special_literal(src_token);

		if (dst_amh_type != E_Ans_Lex_Token_Type_NULL)
			return dst_amh_type;
		dst_amh_type = check_str_of_value(src_token);
		if (dst_amh_type != E_Ans_Lex_Token_Type_NULL)
			return dst_amh_type;
	}
	/*var name or class name or func name...*/
	return E_Ans_Lex_Token_Type_Iden;
}
bool check_to_token_keyword(const char* src_token) {
	const char* keyword_token_list[] = {
		"loop", "if", "else", "break_a", "break", "pbl", "prv", "prt","fld", "mod", "obj", "ext","cls"
		"int", "float", "double", "char", "string", "virtuals", "none", "array", "return", "albtype", "switch", NULL
	};

	for (const char** tmp_keyword_token_list = &keyword_token_list[0]; *tmp_keyword_token_list; ++tmp_keyword_token_list) {
		if (simple_strcmp(src_token, *tmp_keyword_token_list))return true;
	}
	return false;
}

Ans_Lex_Token_Type check_str_of_value(const char* src_str) {
	if (!src_str)return E_Ans_Lex_Token_Type_NULL;

	bool float_check = false;

	if (!check_to_digit_8byte(*src_str)) {
		/*check to begin iden.*/

		if (*src_str != '.')return E_Ans_Lex_Token_Type_Iden;
		/*if this find to begin dot. the str decision float literal and check to true float.*/
		float_check = true;
	}
	for (unsigned int i = 0; *(src_str + i); ++i) {
		if (*(src_str + i) == '.') {
			/*find to dot in binary.*/
			/**/
			if (float_check)return E_Ans_Lex_Token_Type_Error_Value;
			float_check = true;
		}
		else if (!check_to_digit_8byte(*(src_str + i)))return E_Ans_Lex_Token_Type_Error_Value;
	}
	return E_Ans_Lex_Token_Type_Value_Literal;
}
Ans_Lex_Token_Type check_to_token_special_literal(const char* src_token) {
	const char* special_literal_str_token[] = {
		"v_em", "none", "_______none", NULL
	};
	for (const char** tmp_str_token = special_literal_str_token; *tmp_str_token; ++tmp_str_token)
		if (simple_strcmp(src_token, *tmp_str_token))return E_Ans_Lex_Token_Type_Special_Literal;
	return E_Ans_Lex_Token_Type_NULL;
}
bool ans_token_list_expect_token_str(Ans_Lex_Token_List* src_token_list, const char* src_token_str) {
	if (src_token_list->stack_token_list_index >= src_token_list->stack_token_list_size) return false;
	if (!src_token_list->stack_tokens)return false;
	Ans_Lex_Token* tmp_token = src_token_list->stack_tokens + src_token_list->stack_token_list_index;
	if (simple_strcmp(tmp_token->ans_token_str, src_token_str)) {
		s_free(tmp_token->ans_token_str);
		src_token_list->stack_token_list_index++;
		return true;
	}
	else return false;
}