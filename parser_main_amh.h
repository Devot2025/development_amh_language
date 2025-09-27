#ifndef _PARSER_MAIN_AMH_H_
#define _PARSER_MAIN_AMH_H_
#include "lexer_main_amh.h"
#define amh_ast_parser_is_strap static inline
#define AMH_PARSER_INDEX_CHECK(list)
typedef struct Amh_Ast_Nodes {
	struct Amh_Ast_Nodes* left;
	union {
		void* op;
		char* op_c;
		size_t op_t;
	};
	struct Amh_Ast_Nodes* right;
}Amh_Ast_Nodes;
typedef struct Amh_Abstract_Host_Ast_Chain {
	Amh_Ast_Nodes* amh_block;
	struct Amh_Abstract_Host_Ast_Chain* amh_block_next;
}Amh_Abstract_Host_Ast_Chain;
#define GEN_ENUM(name) name
#define GEN_ENUM_STRING(name) #name
#define TOKEN_TYPE_LIST(GEN_MODE)\
GEN_MODE(E_Amh_Type_NULL),\
GEN_MODE(E_Amh_Type_Men_Acc),\
GEN_MODE(E_Amh_Type_While),\
GEN_MODE(E_Amh_Type_For),\
GEN_MODE(E_Amh_Type_For_Each),\
GEN_MODE(E_Amh_Type_Do_While),\
GEN_MODE(E_Amh_Type_Loop),\
GEN_MODE(E_Amh_Type_Loop_Block),\
GEN_MODE(E_Amh_Type_If),\
GEN_MODE(EAmh_Type_Else_If),\
GEN_MODE(E_Amh_Type_Else),\
GEN_MODE(E_Amh_Type_Func_Def),\
GEN_MODE(E_Amh_Type_Func_Use),\
GEN_MODE(E_Amh_Type_Func_Iden),\
GEN_MODE(E_Amh_Type_Class_Instance),\
GEN_MODE(E_Amh_Type_Func_Flat_Args),\
GEN_MODE(E_Amh_Type_Func_Args),\
GEN_MODE(E_Amh_Type_Plus),\
GEN_MODE(E_Amh_Type_Minus),\
GEN_MODE(E_Amh_Type_Mul),\
GEN_MODE(E_Amh_Type_Div),\
GEN_MODE(E_Amh_Type_Assignment),\
GEN_MODE(E_Amh_Type_Plus_Assignment),\
GEN_MODE(E_Amh_Type_Minus_Assignment),\
GEN_MODE(E_Amh_Type_Mul_Assignment),\
GEN_MODE(E_Amh_Type_Div_Assignment),\
GEN_MODE(E_Amh_Type_Less),\
GEN_MODE(E_Amh_Type_Greater),\
GEN_MODE(E_Amh_Type_Less_Equal),\
GEN_MODE(E_Amh_Type_Greater_Equal),\
GEN_MODE(E_Amh_Type_Not_Equal),\
GEN_MODE(E_Amh_Type_Comma),\
GEN_MODE(E_Amh_Type_Var_Change),\
GEN_MODE(E_Amh_Type_Pare),\
GEN_MODE(E_Amh_Type_Statement),\
GEN_MODE(E_Amh_Type_Iden),\
GEN_MODE(E_Amh_Type_Equal),\
GEN_MODE(E_Amh_Type_Host_Return),\
GEN_MODE(E_Amh_Type_Seq_Host_Break),\
GEN_MODE(E_Amh_Type_Seq_Host_Break_All),\
GEN_MODE(E_Amh_Type_None),\
GEN_MODE(E_Amh_Type_Virtuals),\
GEN_MODE(E_Amh_Type_Virtual_Empty),\
GEN_MODE(E_Amh_Type_Bool),\
GEN_MODE(E_Amh_Type_Char),\
GEN_MODE(E_Amh_Type_Int),\
GEN_MODE(E_Amh_Type_Float),\
GEN_MODE(E_Amh_Type_Double),\
GEN_MODE(E_Amh_Type_String),\
GEN_MODE(E_Amh_Type_Array),\
GEN_MODE(E_Amh_Type_Func),\
GEN_MODE(E_Amh_Type_Class_Def),\
GEN_MODE(E_Amh_Type_Class_Member_Def),\
GEN_MODE(E_Amh_Type_Block),\
GEN_MODE(E_Amh_Type_Abstract_Host),\
GEN_MODE(E_Amh_Type_Acc_Abstract_Host),\
GEN_MODE(E_Amh_Type_Seq_Abstract_Host),\
GEN_MODE(E_Amh_Type_Module),\
GEN_MODE(E_Amh_Type_Class),\
GEN_MODE(E_Amh_Type_Unknown),\
GEN_MODE(E_Amh_Hash_Host_Acc),\
GEN_MODE(E_Amh_Hash_Host_Run),\
GEN_MODE(E_Amh_Hash_Host_Imp_Run),\
GEN_MODE(E_Amh_Hash_Syntactic_Sugar_Class),\
GEN_MODE(E_Amh_Hash_Syntactic_Sugar_Module),
struct Amh_Lex_Token_List;

typedef enum Run_Amh_Token_Type {
	TOKEN_TYPE_LIST(GEN_ENUM)
}Run_Amh_Token_Type;

extern const char* debug_type_string[];
void delete_ast_amh_node(Amh_Ast_Nodes* src_amh_ast);

void print_ast_amh_node(const Amh_Ast_Nodes* src_amh_ast);
void* ast_op_memdup(Run_Amh_Token_Type src_token_type, const void* dat, size_t src_size);
void* ast_op_iden_dup(const Amh_Ast_Nodes* src_ast_node);

Run_Amh_Token_Type multi_amh_token_expect(Amh_Lex_Token_List* src_lex_list, const char** src_token_str_list, const Run_Amh_Token_Type* src_token_type_list, uintptr_t src_list_size);
Run_Amh_Token_Type assigments_amh_token_expect(Amh_Lex_Token_List* src_lex_list);
Run_Amh_Token_Type add_sub_amh_token_expect(Amh_Lex_Token_List* src_lex_list);
Run_Amh_Token_Type mul_div_amh_token_expect(Amh_Lex_Token_List* src_lex_list);


Amh_Ast_Nodes* start_amh_parser_main(Amh_Lex_Token_List * src_lex_list);
Amh_Abstract_Host_Ast_Chain* build_ast_amh_abstract_host_node(Amh_Lex_Token_List* src_lex_list);
Amh_Ast_Nodes* build_ast_amh_statement(Amh_Ast_Nodes* src_node);
Amh_Ast_Nodes* build_ast_amh_statement_error(Amh_Lex_Token_List* src_lex_list, Amh_Ast_Nodes* src_node);
Amh_Ast_Nodes* build_ast_abstract_amh_host(Amh_Lex_Token_List* src_lex_list);
Amh_Ast_Nodes* build_ast_amh_block_recursive(Amh_Lex_Token_List* src_lex_list);
Amh_Ast_Nodes* build_ast_amh_comma(Amh_Lex_Token_List* src_lex_list);
Amh_Ast_Nodes* build_ast_amh_assigment(Amh_Lex_Token_List* src_lex_list);
Amh_Ast_Nodes* build_ast_amh_equals(Amh_Lex_Token_List* src_lex_list);
Amh_Ast_Nodes* build_ast_amh_relation(Amh_Lex_Token_List* src_lex_list);

Amh_Ast_Nodes* build_ast_amh_add_sub(Amh_Lex_Token_List* src_lex_list);
Amh_Ast_Nodes* build_ast_amh_return(Amh_Lex_Token_List* src_lex_list);
Amh_Ast_Nodes* build_ast_amh_mul_div(Amh_Lex_Token_List* src_lex_list);
Amh_Ast_Nodes* build_ast_amh_pare(Amh_Lex_Token_List* src_lex_list);
Amh_Ast_Nodes* build_ast_amh_back_ope(Amh_Lex_Token_List* src_lex_list);
Amh_Ast_Nodes* build_ast_amh_use_args(Amh_Lex_Token_List* src_lex_list);
Amh_Ast_Nodes* build_ast_amh_use_function(Amh_Lex_Token_List* src_lex_list);

Amh_Ast_Nodes* build_ast_amh_basic_token(Amh_Lex_Token_List* src_lex_list);


amh_ast_parser_is_strap bool ast_op_type_cmp(Amh_Ast_Nodes* src_amh_ast, Run_Amh_Token_Type src_token_type) {
	return memcmp(src_amh_ast->op, &src_token_type, sizeof(Run_Amh_Token_Type)) == 0;
}
amh_ast_parser_is_strap bool ast_op_memcmp_token_type(Amh_Ast_Nodes* src_ast_nodes, Run_Amh_Token_Type src_token_type) {
	return memcmp(src_ast_nodes->op, &src_token_type, sizeof(Run_Amh_Token_Type)) == 0;
}
amh_ast_parser_is_strap const char* acc_iden_token_string(const Amh_Ast_Nodes* src_amh_ast) { return src_amh_ast->op_c + sizeof(Run_Amh_Token_Type); }

amh_ast_parser_is_strap void delete_ast_absrtact_host_chain(Amh_Abstract_Host_Ast_Chain** src_amh_abstract_host_chain) {
	if (!src_amh_abstract_host_chain || !*src_amh_abstract_host_chain)return;
	Amh_Abstract_Host_Ast_Chain* tmp_amh_abstract_host_chain = *src_amh_abstract_host_chain;
	while (tmp_amh_abstract_host_chain) {
		delete_ast_amh_node(tmp_amh_abstract_host_chain->amh_block);
		Amh_Abstract_Host_Ast_Chain* tmp_next_chain = tmp_amh_abstract_host_chain->amh_block_next;
		free(tmp_amh_abstract_host_chain);
		tmp_amh_abstract_host_chain = tmp_next_chain;
	}
	*src_amh_abstract_host_chain = NULL;
}

amh_ast_parser_is_strap int amh_token_block_process_check(Amh_Lex_Token_List* src_lex_list) {
	if (amh_token_list_expect_str(src_lex_list, "}"))return -1;
	if (src_lex_list->stack_token_list_index >= src_lex_list->stack_token_list_size)return 0;
	return 1;
}
amh_ast_parser_is_strap Amh_Ast_Nodes* amh_token_host_chain(Amh_Ast_Nodes* src_chain, Amh_Ast_Nodes* src_node) {

	if (!src_chain) {
		Amh_Ast_Nodes* amh_next = smart_calloc(Amh_Ast_Nodes, 1);
		if (!amh_next)return src_chain;
		amh_next->left = src_chain;
		amh_next->op = ast_op_memdup(E_Amh_Type_Abstract_Host, NULL, 0);
		amh_next->right = src_node;
	}
	else {
		Amh_Ast_Nodes* amh_next = smart_calloc(Amh_Ast_Nodes, 1);
		if (!amh_next)return src_chain;
		amh_next->op = ast_op_memdup(E_Amh_Type_Abstract_Host, NULL, 0);
		amh_next->right = src_node;
		src_chain->left = amh_next;
		return amh_next;
	}
}

amh_ast_parser_is_strap Run_Amh_Token_Type get_ast_node_type(const Amh_Ast_Nodes* n) {
	if (!n || !n->op) return E_Amh_Type_NULL;
	return *(const Run_Amh_Token_Type*)(n->op);
}

#endif // !_PARSER_MAIN_AMH_H_
