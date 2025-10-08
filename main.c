#include <stdio.h>
#include "lexer_main_ans.h"

#include "parser_main_ans.h"
#include "ans_ast_debug_print.h"
int main(void) {
	const char* load_file_name = "example/counter.ans";
	Ans_Lex_Token_List* token_list = start_ans_lex_main(load_file_name);
	ans_token_list_call(token_list);
	Ans_Hash_Process_List* hash_list = gen_hash_process_list(load_file_name);
	Ans_Ast_Nodes* ast_node = start_parser_ans_ast_main(token_list, hash_list);
	start_hash_process_main(&ast_node, hash_list);

	print_ast_ans_node(ast_node);

	delete_ans_token_list(&token_list);
	delete_ans_ast_node(ast_node);
	delete_hash_process_list(hash_list);
	return 0;

}

