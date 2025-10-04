#ifndef _PARSER_MAIN_ANS_H_
#define _PARSER_MAIN_ANS_H_

#include "lexer_main_ans.h"
#include "hash_process_ans_main.h"

#define ans_parser_si_strap static inline
#define Bulk_Gen_Ans_Parser_Token(To_Do_Macro_Func)\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Null),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Block),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Host_Return),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Abstract_Host),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Expr_Statement),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Expr_Pare),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Expr_Comma),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Men_Acc),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Spe_Men_Acc),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Iden_Changer),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Func_Args),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Func_Decl_Args),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Iden),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Var_Decl),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Func_Use),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Func_Decl),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Class_Decl),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Class_Filed_Public),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Add),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Sub),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Mul),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Div),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Mod),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_And),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Or),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Xor),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Not),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Not_Logic),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Left_Shift),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Right_Shift),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Assigment),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Add_Assigment),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Sub_Assigment),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Mul_Assigment),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Div_Assigment),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Mod_Assigment),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Less),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Greater),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Less_Equal),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Greater_Equal),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Not_Equal),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Int_Value),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Float_Value),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Double_Value),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Char_Value),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_String_Value),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Virtuals_Value),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_Virtual_Empty_Value),\
To_Do_Macro_Func(E_Ans_Ast_Token_Type_None_Value),
typedef enum Ans_Ast_Token_Type{
	Bulk_Gen_Ans_Parser_Token(CHANGE_TO_ENUM)
}Ans_Ast_Token_Type;
typedef struct Ans_Ast_Nodes {
	struct Ans_Ast_Nodes* left;
	struct { void* op; Ans_Ast_Token_Type token_type; };
	struct Ans_Ast_Nodes* right;
}Ans_Ast_Nodes;

ans_parser_si_strap Ans_Ast_Nodes* gen_set_op_ans_ast_node(Ans_Ast_Token_Type src_token_type, void * src_op, size_t src_op_size) {
	Ans_Ast_Nodes* new_ans_ast_nodes = smart_calloc(Ans_Ast_Nodes, 1);
	if (!new_ans_ast_nodes)return NULL;
	new_ans_ast_nodes->op = dupalloc(src_op, src_op_size);
	new_ans_ast_nodes->token_type = src_token_type;
	return new_ans_ast_nodes;
}
ans_parser_si_strap void set_up_ast_node_op_datas(Ans_Ast_Nodes* src_ans_ast_node, Ans_Ast_Token_Type src_token_type, const void* src_op, size_t src_op_size) {
	if (src_ans_ast_node->op)free(src_ans_ast_node->op);
	src_ans_ast_node->op = dupalloc(src_op, src_op_size);
	src_ans_ast_node->token_type = src_token_type;
}
ans_parser_si_strap Ans_Ast_Nodes* set_up_ans_ast(Ans_Ast_Nodes* src_left, Ans_Ast_Nodes* src_right, Ans_Ast_Token_Type src_token_type, const void* src_op_data, size_t src_op_size) {
	Ans_Ast_Nodes* new_ans_ast_nodes = smart_malloc(Ans_Ast_Nodes, 1);
	if (!new_ans_ast_nodes)return NULL;
	new_ans_ast_nodes->left = src_left;
	new_ans_ast_nodes->right = src_right;
	new_ans_ast_nodes->op = dupalloc(src_op_data, src_op_size);
	new_ans_ast_nodes->token_type = src_token_type;
	return new_ans_ast_nodes;
}
ans_parser_si_strap void set_up_ans_ast_type(Ans_Ast_Nodes* src_node, Ans_Ast_Token_Type src_type) {
	src_node->token_type = src_type;
}
ans_parser_si_strap void set_up_ans_ast_data(Ans_Ast_Nodes* src_node, const void* src_data, size_t src_size) {
	if (src_node->op)free(src_node->op);
	src_node->op = dupalloc(src_data, src_size);
}

ans_parser_si_strap void set_up_ans_ast_nodes(Ans_Ast_Nodes* src_node, Ans_Ast_Nodes* src_left, Ans_Ast_Nodes* src_right) {
	src_node->left = src_left;
	src_node->right = src_right;
}
ans_parser_si_strap Ans_Ast_Nodes * delete_ans_ast_node(Ans_Ast_Nodes* src_ans_nodes) {
	if (!src_ans_nodes)return NULL;
	delete_ans_ast_node(src_ans_nodes->left);
	if (src_ans_nodes->op)free(src_ans_nodes->op);
	delete_ans_ast_node(src_ans_nodes->right);
	free(src_ans_nodes);
	return NULL;
}

void decision_final_accurate_binary_type(Ans_Ast_Nodes* src_ans_ast_node, const char* src_str);
void decision_final_accurate_specail_value(Ans_Ast_Nodes* src_node, const char* src_now_token_str);
int end_code_ans_ast_block(Ans_Lex_Token_List* src_ans_token_list);

Ans_Ast_Token_Type get_match_ans_lex_ast_multi_type(const Ans_Ast_Token_Type* dst_token_type, const char** src_match_lex_str_list, Ans_Lex_Token_List* src_ans_token_list, uint32_t src_match_list_size);
Ans_Ast_Token_Type expect_add_sub_type(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Token_Type expect_mul_div_mod_type(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Token_Type expect_assigment_type(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Token_Type expect_equals_type(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Token_Type expect_relation_type(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* start_parser_ans_ast_main(Ans_Lex_Token_List* src_ans_token_list, Ans_Hash_Process_List* src_hash_list);
Ans_Ast_Nodes* build_ans_ast_abstract_node(Ans_Lex_Token_List* src_ans_token_list, Ans_Hash_Process_List* src_hash_list);
Ans_Ast_Nodes* build_ans_ast_args_decl(int* end_code, Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_statement(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_shift(Ans_Lex_Token_List* src);
Ans_Ast_Nodes* build_ans_ast_keyword(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_block(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_block_process(int* end_code, Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_iden_decl(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_host_return(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_expr_statement_err(Ans_Lex_Token_List* src_ans_token_list, Ans_Ast_Nodes* src_left);
Ans_Ast_Nodes* build_ans_ast_expr_statement(Ans_Lex_Token_List* src_ans_token_list, Ans_Ast_Nodes * ans_left);
Ans_Ast_Nodes* build_ans_ast_expr_comma(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_assigment(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_bit_or(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_bit_xor(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_bit_and(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_equlas(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_relation(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_add_sub(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_mul_div_mod(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_back_ope(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_function_args(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_function(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_sign_symbol(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_pare(Ans_Lex_Token_List* src_ans_token_list);
Ans_Ast_Nodes* build_ans_ast_basic_token(Ans_Lex_Token_List* src_ans_token_list);
#endif // !_PARSER_MAIN_ANS_H_
