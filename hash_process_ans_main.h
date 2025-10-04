#ifndef _HASH_PROCESS_ANS_MAIN_H_
#define _HASH_PROCESS_ANS_MAIN_H_
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ext_alloc.h"
#include "str_ext.h"
#include "lexer_main_ans.h"
typedef struct Ans_Lex_Token_List Ans_Lex_Token_List;
typedef struct Ans_Ast_Nodes Ans_Ast_Nodes;

#define HASH_PROCESS_STANDARD_ID (1u)
#define HASH_PROCESS_MODULE_ID (1u << 2)
#define HASH_PROCESS_CLASS_ID (1u << 3)
#define HASH_PROCESS_EXTEND_ID (1u << 4)
#define HASH_PROCESS_ACC_ID 1 << 1
#define HASH_PROCESS_PACKAGE_ID 1 << 2
#define HASH_PROCESS_LIB_ID 1 << 3

#define hash_process_ans_si_strap static inline
typedef uint32_t hash_id;
typedef struct Ans_Hash_Process_List {
	hash_id file_modularization_process;
	hash_id file_depency_process;
	char* abstract_host_name;
}Ans_Hash_Process_List;
hash_process_ans_si_strap Ans_Hash_Process_List* gen_hash_process_list(const char* ans_file_name) {
	if (!ans_file_name)return NULL;
	const char* cache_ans_file_name = ans_file_name;
	while (*ans_file_name && *ans_file_name != '.') ans_file_name++;
	Ans_Hash_Process_List* new_hash_process_list = smart_calloc(Ans_Hash_Process_List, 1);
	if (!new_hash_process_list)return new_hash_process_list;

	new_hash_process_list->file_depency_process = HASH_PROCESS_STANDARD_ID;
	new_hash_process_list->file_modularization_process = HASH_PROCESS_STANDARD_ID;
	size_t abstract_name_len = (size_t)(ans_file_name - cache_ans_file_name);
	new_hash_process_list->abstract_host_name = smart_calloc(char, abstract_name_len + 1);
	if (!new_hash_process_list->abstract_host_name) {
		free(new_hash_process_list);
		return NULL;
	}
	memcpy(new_hash_process_list->abstract_host_name, cache_ans_file_name, abstract_name_len);
	return new_hash_process_list;
}
hash_process_ans_si_strap Ans_Hash_Process_List* delete_hash_process_list(Ans_Hash_Process_List* src_hash_list) {
	free(src_hash_list->abstract_host_name);
	free(src_hash_list);
	return NULL;
}
bool standard_hash_process_ans(Ans_Hash_Process_List* src_hash_process_list, Ans_Lex_Token_List* src_token_list);
void start_hash_process_main(Ans_Ast_Nodes** src_ast_node, Ans_Hash_Process_List* src_hash_process_list);
Ans_Ast_Nodes* delete_chain_ans_ast_build(Ans_Ast_Nodes* src_ast_node);
Ans_Ast_Nodes* delete_module_only_build(Ans_Ast_Nodes* src_ast_filed_node, Ans_Ast_Nodes* src_ast_host_node, Ans_Ast_Nodes* src_ast_toplevel_node, Ans_Ast_Nodes* src_ast_class_node);
Ans_Ast_Nodes* change_to_module_hash_process(Ans_Ast_Nodes* src_ast_node, const char* module_name);
Ans_Ast_Nodes* change_to_module_hash_process_helper(Ans_Ast_Nodes* src_ast_node);
#endif // !_HASH_PROCESS_ANS_MAIN_H_
