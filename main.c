#include <stdio.h>
#include "ext_io_sys.h"
#include "lexer_main_ans.h"
#include "parser_main_ans.h"
#include "ans_ast_debug_print.h"
#include "ir_main_ans.h"
#include "run_main_ans.h"

const char * dmp_ans_args(char ** src_args, int src_argc) {
	if (src_argc > 1)
		return *(src_args + src_argc - 1);
	else return "example/test.ans";
}
int main(int argc, char ** args) {
	//printf("%s\n", smart_fsystem_gets(stdin));

	const char* load_file_name = dmp_ans_args(args, argc);

	Ans_Lex_Token_List* token_list = start_ans_lex_main(load_file_name);
	ans_token_list_call(token_list);
	Ans_Hash_Process_List* hash_list = gen_hash_process_list(load_file_name);
	Ans_Ast_Nodes* ast_node = start_parser_ans_ast_main(token_list, hash_list);
	start_hash_process_main(&ast_node, hash_list);

	print_ast_ans_node(ast_node);
	//Ans_Bir_Chain * abir_chain = start_ans_ir_main(ast_node);
	//start_print_abir_insruction(abir_chain,true);
	//start_ini_run_program(abir_chain);

	//delete_ans_bir_chain(abir_chain);
	delete_ans_token_list(&token_list);
	delete_ans_ast_node(ast_node);
	delete_hash_process_list(hash_list);
	return 0;

}
