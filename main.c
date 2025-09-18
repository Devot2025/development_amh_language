#include <stdio.h>
#include "lexer_main_amh.h"
#include "parser_main_amh.h"
#define __ABSTRACT__WINDOW__OS__HOST__ "WinOS"
int main(void) {
	Amh_Lex_Token_List * tmp = start_read_amh_lex_main(__ABSTRACT__WINDOW__OS__HOST__, "test.amh");
	amh_token_list_call(tmp);
	Amh_Ast_Nodes * tmp_ast = start_amh_parser_main(tmp);
	delete_amh_token_list(&tmp);
	printf("abs host -> %s\n", __ABSTRACT__WINDOW__OS__HOST__);
	print_ast_amh_node(tmp_ast, 0);

	delete_ast_amh_node(tmp_ast);
	return 0;
}