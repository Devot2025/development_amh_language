#ifndef _LEXER_MAIN_AMH_H_
#define _LEXER_MAIN_AMH_H_

#include "str_ext.h"
#include "ext_alloc.h"
#include <stdint.h>
#define lex_amh_is_strap static inline
/*
* amherstia lex mode
* 
*/
typedef enum Amh_Lex_Mode {
	E_Amh_Lex_Normal,
	E_Amh_Lex_S_Str_Literal,
	E_Amh_Lex_D_Str_Literal,
	E_Amh_Lex_D_Str_Doll,
	E_Amh_Lex_Bin_Literal,
	E_Amh_Lex_Comment_Start,
	E_Amh_Lex_Comment,
	E_Amh_Lex_Comment_Line_Start,
	E_Amh_Lex_Comment_Line_End,
	E_Amh_Lex_Comment_Call,
	E_Amh_Lex_Hash_Process,
	E_Amh_Lex_Hash_Process_Space,
	E_Amh_Lex_Hash_Process_Space_Analysis,
	E_Amh_Lex_Dot,
	E_Amh_Lex_Operation,
}Amh_Lex_Mode;
typedef enum Amh_Lex_Token_Type {
	E_Amh_Lex_Token_Type_Operation,
	E_Amh_Lex_Token_Type_Punchcute,
	E_Amh_Lex_Token_Type_Bin_Literal,
	E_Amh_Lex_Token_Type_Specail_Literal,
	E_Amh_Lex_Token_Type_S_Str_Literal,
	E_Amh_Lex_Token_Type_D_Str_Literal,
	E_Amh_Lex_Token_Type_Standard,
	E_Amh_Lex_Token_Type_Keyword,
	E_Amh_Lex_Token_Type_Iden,
	E_Amh_Lex_Token_Type_Hash_Process,
	E_Amh_Lex_Token_Type_NULL,
}Amh_Lex_Token_Type;

#define INIT_AMH_TOKEN_LIST_SIZE 100
#define INIT_HOST_NUM 10


#define AMH_FOMRAT_FILE ".amh"/*file*/
#define AMH_FOMRAT_LIB ".alb"/*lib*/

typedef struct Amh_Lex_Token {
	char* amh_token;
	Amh_Lex_Token_Type amh_token_type;
}Amh_Lex_Token;
typedef struct Amh_Lex_Token_List {
	Amh_Lex_Token * stack_token_list;
	uint32_t stack_token_list_size;
	uint32_t stack_token_list_index;
}Amh_Lex_Token_List;
typedef struct Amh_Abstract_host{
	char** host_name;
	uint32_t now_host_index;
	uint32_t host_size;
}Amh_Abstract_Host;

typedef struct Amh_Code_Node {
	char* amh_code;
	char* regist_amh_code;
	struct Amh_Code_Node* next_amh_code;
}Amh_Code_Node;

Amh_Lex_Token_Type ckeck_str_of_value(const char* src_str);
char* start_read_amh_file_code(const char* amh_file_path);
Amh_Lex_Token_List * start_lex_amh_code(const char * amh_file_name, const char * src_amh_code);
Amh_Lex_Token_List * start_read_amh_lex_main(const char * abstract_host, const char* amh_file_path);
bool amh_token_list_expect_str(Amh_Lex_Token_List* src_amh_token_list, const char* str);
void consume_advance_list(Amh_Lex_Token_List* src_amh_token_list);
Amh_Abstract_Host* gen_abstract_host(const char* first_abstract_host_name, uint32_t src_host_num);
lex_amh_is_strap const Amh_Lex_Token* get_now_amh_token(Amh_Lex_Token_List* src_amh_token_list) {
	return src_amh_token_list->stack_token_list + src_amh_token_list->stack_token_list_index;
}

lex_amh_is_strap const Amh_Lex_Token_Type get_now_amh_token_type(Amh_Lex_Token_List* src_amh_token_list) {
	return (*(src_amh_token_list->stack_token_list + src_amh_token_list->stack_token_list_index)).amh_token_type;
}

lex_amh_is_strap const char* get_now_amh_token_str(Amh_Lex_Token_List* src_amh_token_list) {
	return (*(src_amh_token_list->stack_token_list + src_amh_token_list->stack_token_list_index)).amh_token;
}
const char* get_comsume_amh_token_str(Amh_Lex_Token_List* src_amh_token_list);

/*amh lex token append to list.*/
void delete_amh_token_list(Amh_Lex_Token_List** src_amh_token_list);
void amh_token_list_call(Amh_Lex_Token_List* src_amh_token_list);

void append_lex_token_to_token_list(Amh_Lex_Token_List* src_amh_token_list, Str_Buffer_Array* src_stack_token, Amh_Lex_Token_Type src_lex_token_type);
#endif // !_LEXER_MAIN_AMH_H_
