#ifndef _LEXER_MAIN_ANS_H_
#define _LEXER_MAIN_ANS_H_
#include "str_ext.h"
#include "ext_alloc.h"
#include "load_file_data.h"
#include <assert.h>
#define CHANGE_TO_STR(src) #src
#define CHANGE_TO_ENUM(src) src
#define ans_lex_si_strap static inline
#define INI_BLOCK_SIZE 100
#define ENUM_STR_OUT_POS 21
typedef enum Lex_Ans_Mode {
	E_Lex_Mode_Normal,
	E_Lex_Mode_Operator,
	E_Lex_Mode_Dot,
	E_Lex_Mode_Three_Dot,
	E_Lex_Mode_D_Str,
	E_Lex_Mode_D_Str_Doll,
	E_Lex_Mode_D_Str_Next,
	E_Lex_Mode_S_Str,
	E_Lex_Mode_S_Str_Doll,
	E_Lex_Mode_Hash_Process,
	E_Lex_Mode_Comment_Start,
	E_Lex_Mode_Comment_Line,
	E_Lex_Mode_Comment_Call,
	E_Lex_Mode_Comment_Multi_Line_Start,
	E_Lex_Mode_Comment_Multi_Line_End,
}Lex_Ans_Mode;
#define Bulk_Gen_Ans_Lex_Token(To_Do_Macro_Func)\
To_Do_Macro_Func(E_Ans_Lex_Token_Type_NULL),\
To_Do_Macro_Func(E_Ans_Lex_Token_Type_Iden),\
To_Do_Macro_Func(E_Ans_Lex_Token_Type_Keyword),\
To_Do_Macro_Func(E_Ans_Lex_Token_Type_Punchcute),\
To_Do_Macro_Func(E_Ans_Lex_Token_Type_Operator),\
To_Do_Macro_Func(E_Ans_Lex_Token_Type_Special_Literal),\
To_Do_Macro_Func(E_Ans_Lex_Token_Type_Value_Literal),\
To_Do_Macro_Func(E_Ans_Lex_Token_Type_Char_Literal),\
To_Do_Macro_Func(E_Ans_Lex_Token_Type_Str_Literal),\
To_Do_Macro_Func(E_Ans_Lex_Token_Type_Hash_Process),\
To_Do_Macro_Func(E_Ans_Lex_Token_Type_Error_Value),\
To_Do_Macro_Func(E_Ans_Lex_Token_Type_Error_),

typedef enum Ans_Lex_Token_Type {
	Bulk_Gen_Ans_Lex_Token(CHANGE_TO_ENUM)
}Ans_Lex_Token_Type;
typedef struct Ans_Lex_Token {
	char* ans_token_str;
	Ans_Lex_Token_Type ans_token_type;
}Ans_Lex_Token;
typedef struct Ans_Lex_Token_List {
	Ans_Lex_Token* stack_tokens;
	uint32_t stack_token_list_size;
	uint32_t stack_token_list_index;
}Ans_Lex_Token_List;
ans_lex_si_strap void ans_token_list_call(Ans_Lex_Token_List* src_ans_token_list) {
	const char* ans_lex_debug_string_token[] = {
	Bulk_Gen_Ans_Lex_Token(CHANGE_TO_STR)
	};
	if (!src_ans_token_list)return;
	if (!src_ans_token_list->stack_tokens)return;
	for (src_ans_token_list->stack_token_list_index = 0; src_ans_token_list->stack_token_list_index < src_ans_token_list->stack_token_list_size; src_ans_token_list->stack_token_list_index++) {
		assert((*(src_ans_token_list->stack_tokens + src_ans_token_list->stack_token_list_index)).ans_token_type <= GET_ARRAY_SIZE(ans_lex_debug_string_token));
		printf("[idx %10lu|type %20s|token %10s]\n", src_ans_token_list->stack_token_list_index,
			ans_lex_debug_string_token[(*(src_ans_token_list->stack_tokens + src_ans_token_list->stack_token_list_index)).ans_token_type] + ENUM_STR_OUT_POS,
			(*(src_ans_token_list->stack_tokens + src_ans_token_list->stack_token_list_index)).ans_token_str);
	}
	src_ans_token_list->stack_token_list_index = 0;
}
ans_lex_si_strap Ans_Lex_Token* peek_ans_lex_token(Ans_Lex_Token_List* src_ans_token_list) {
	if (src_ans_token_list->stack_token_list_index >= src_ans_token_list->stack_token_list_size)return NULL;
	return src_ans_token_list->stack_tokens + src_ans_token_list->stack_token_list_index;
}
ans_lex_si_strap void consume_ans_lex_token(Ans_Lex_Token_List* src_ans_token_list) {
	if (src_ans_token_list->stack_token_list_index >= src_ans_token_list->stack_token_list_size)return;
	Ans_Lex_Token* tmp_ans_token = src_ans_token_list->stack_tokens + src_ans_token_list->stack_token_list_index;
	s_free(tmp_ans_token->ans_token_str);
	tmp_ans_token->ans_token_type = 0;
	src_ans_token_list->stack_token_list_index++;
}

Ans_Lex_Token_List* start_ans_lex_main(const char* ans_file_path);
Ans_Lex_Token_List* start_ans_lex_code(const char* src_ans_code);
void delete_ans_token_list(Ans_Lex_Token_List** src_ans_token_list);
void ans_normal_token_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token_list, Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
bool ans_three_dot_token_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
bool ans_dot_token_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
bool ans_operator_token_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
void ans_s_str_token_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
void ans_s_str_doll_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
bool ans_d_str_next_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
void ans_d_str_token_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
void append_lex_token_to_token_list(Ans_Lex_Token_List* src_ans_token_list, Str_Buffer_Array* src_stack_token, Ans_Lex_Token_Type src_lex_token_type);
void ans_d_str_doll_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
void ans_comment_start_process(Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
void ans_comment_line_process(Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
void ans_comment_mutli_line_process(Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
void ans_comment_multi_line_end_process(Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
void ans_comment_call_process(Str_Buffer_Array* src_stack_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
void ans_hash_process(Str_Buffer_Array* src_stack_token, Ans_Lex_Token_List* src_lex_token, Lex_Ans_Mode* src_lex_mode, const char now_byte_code);
Ans_Lex_Token_List* gen_amh_token_list(uint32_t src_ini_size);
Ans_Lex_Token_Type decision_token_type_standard(const char* src_token);
bool check_to_token_keyword(const char* src_token);
Ans_Lex_Token_Type check_str_of_value(const char* src_str);
Ans_Lex_Token_Type check_to_token_special_literal(const char* src_token);
bool ans_token_list_expect_token_str(Ans_Lex_Token_List* src_token_list, const char* src_token_str);
bool ans_token_list_expect_token_data(Ans_Lex_Token_List* src_token_list, Ans_Lex_Token_Type src_token_type, const char* src_tokem_str);
#endif // !_LEXER_MAIN_ANS_H_
