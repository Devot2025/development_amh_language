#ifndef _LEXSER_MAIN_AMHERSTIA_H_
#define _LEXSER_MAIN_AMHERSTIA_H_

#include "str_ext.h"
#include "ext_alloc.h"
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
	E_Amh_Lex_Comment,
	E_Amh_Lex_Comment_Line_Start,
	E_Amh_Lex_Comment_Line_End,
	E_Amh_Lex_Comment_Call,
	E_Amh_Lex_Pripro,
	E_Amh_Lex_Pripro_Space,
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
	E_Amh_Lex_Token_Type_Pripro_File_Include,
	E_Amh_Lex_Token_Type_Pripro_File_Module,
	E_Amh_Lex_Token_Type_Pripro_File_Class,
	E_Amh_Lex_Token_Type_Pripro_Imp_Run,
	E_Amh_Lex_Token_Type_Pripro_Imp_Run_Call,
	E_Amh_Lex_Token_Type_NULL,
}Amh_Lex_Token_Type;

#define INIT_AMH_TOKEN_LIST_SIZE 100
#define INIT_HOST_NUM 10


#define AMH_FOMRAT_RUN ".amh"
#define AMH_FOMRAT_LIB ".alb"
#define Normal_Run 0
#define Imp_Run 1
#define Imp_Run_Call 2

typedef struct Amh_Lex_Token {
	char* amh_token;
	Amh_Lex_Token_Type amh_token_type;
}Amh_Lex_Token;
typedef struct Amh_Lex_Token_List {
	Amh_Lex_Token * stack_token_list;
	uintptr_t stack_token_list_size;
	uintptr_t stack_token_list_index;
	int run_mode;
}Amh_Lex_Token_List;
typedef struct Amh_Abstract_host{
	char** host_name;
	uintptr_t now_host_index;
	uintptr_t host_size;
}Amh_Abstract_Host;

Amh_Lex_Token_Type ckeck_str_of_value(const char* src_str);
char* start_read_amh_file_code(const char*);
Amh_Lex_Token_List * start_lex_amh_code(const char* src_amh_code);
Amh_Lex_Token_List * start_read_amh_lex_main(const char* amh_file_path);
bool amh_token_list_expect_str(Amh_Lex_Token_List* src_amh_token_list, const char* str);
Amh_Abstract_Host* gen_abstract_host(const char* first_abstract_host_name, uintptr_t src_host_num) {
	if (!first_abstract_host_name) return NULL;
	if (src_host_num == 0) src_host_num = INIT_HOST_NUM;
	Amh_Abstract_Host* dst_amh_abstract_host = smart_calloc(Amh_Abstract_Host, 1);
	
	if (!dst_amh_abstract_host)return NULL;
	
	dst_amh_abstract_host->host_name = smart_calloc(char *, src_host_num);
	
	if (!dst_amh_abstract_host->host_name) {
		free(dst_amh_abstract_host);
		return NULL;
	}
	dst_amh_abstract_host->host_size = src_host_num;
	dst_amh_abstract_host->host_name[dst_amh_abstract_host->now_host_index] = strdup(first_abstract_host_name);
	if (!dst_amh_abstract_host->host_name[dst_amh_abstract_host->now_host_index]) {
		free(dst_amh_abstract_host->host_name);
		free(dst_amh_abstract_host);
		return NULL;
	}
	return dst_amh_abstract_host;
}
void consume_advance_list(Amh_Lex_Token_List* src_amh_token_list);

inline const Amh_Lex_Token* get_now_amh_token(Amh_Lex_Token_List* src_amh_token_list) {
	return src_amh_token_list->stack_token_list + src_amh_token_list->stack_token_list_index;
}

inline const Amh_Lex_Token_Type get_now_amh_token_type(Amh_Lex_Token_List* src_amh_token_list) {
	return (*(src_amh_token_list->stack_token_list + src_amh_token_list->stack_token_list_index)).amh_token_type;
}

inline const char* get_now_amh_token_str(Amh_Lex_Token_List* src_amh_token_list) {
	return (*(src_amh_token_list->stack_token_list + src_amh_token_list->stack_token_list_index)).amh_token;
}
const char* get_comsume_amh_token_str(Amh_Lex_Token_List* src_amh_token_list);

/*amh lex token append to list.*/
void delete_amh_token_list(Amh_Lex_Token_List** src_amh_token_list);
void amh_token_list_call(Amh_Lex_Token_List* src_amh_token_list);

void append_lex_token_to_token_list(Amh_Lex_Token_List* src_amh_token_list, Str_Buffer_Array* src_stack_token, Amh_Lex_Token_Type src_lex_token_type);
#endif // !_LEXSER_MAIN_AMHERSTIA_H_
